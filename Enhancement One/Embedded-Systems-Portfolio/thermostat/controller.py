# Thermostat Controller (State Machine + Orchestration)
# 
# This module contains the core domain logic for the thermostat system. The controller:
# - Maintains the thermostat operating mode using a finite state machine (off/heat/cool)
# - Applies control rules using sensor readings and a configurable setpoint
# - Delegates all hardware access to a Hardware Abstraction Layer (HAL)
# - Runs a background display/telemetry loop on a dedicated daemon thread
# 
# Key design principle:
# - Keep business logic here; keep hardware-specific operations behind HAL methods.

from statemachine import StateMachine, State
from threading import Thread, Event
from datetime import datetime
from math import floor

class ThermostatController(StateMachine):
    # ---------------------------
    # State Machine Declarations
    # ---------------------------
    # 'statemachine' library uses class-level State definitions to build the FSM.
    off = State(initial=True)  # default startup state
    heat = State()
    cool = State()

    # Single event that cycles states in a predictable order.
    # This keeps UI/inputs simple: one "mode" button advances the mode.
    cycle = (off.to(heat) | heat.to(cool) | cool.to(off))

    def __init__(self, hal, cfg, debug=False):
        # Initialize controller dependencies and runtime state.
        # 
        # Args:
        #     hal: Hardware Abstraction Layer instance. Must provide sensor, LEDs, LCD, and serial methods.
        #     cfg: Configuration object containing constants (timing, pin mappings, setpoint bounds, etc.).
        #     debug: Enables human-readable logging for bring-up and troubleshooting.

        super().__init__()
        self.hal = hal      # hardware boundary (all I/O should flow through this dependency)
        self.cfg = cfg      # single source of truth for timing/threshold constants
        self.debug = debug  # low-overhead runtime tracing

        # Setpoint lives in controller state because it is part of "business logic" not hardware state.
        self.setPoint = cfg.DEFAULT_SETPOINT

        # Thread coordination primitives:
        # - _stop allows clean shutdown without busy-waiting
        # - _thread holds the background loop thread handle
        self._stop = Event()
        self._thread = None

    # ---------- Button actions ----------
    # These are designed to be bound directly to GPIO event callbacks.
    def processTempStateButton(self):
        if self.debug: print("Cycling Temperature State")

        # Advance the state machine: off -> heat -> cool -> off
        self.cycle()

        # Immediately update indicators so user sees feedback without delay.
        self.updateLights()

    def processTempIncButton(self):
        if self.debug: print("Increasing Set Point")
        # Clamp setpoint to avoid runaway values and keep UI behavior predictable.
        self.setPoint = min(self.setPoint + 1, self.cfg.MAX_SETPOINT)
        self.updateLights()

    def processTempDecButton(self):
        if self.debug: print("Decreasing Set Point")
        # Clamp setpoint to avoid invalid values below supported range.
        self.setPoint = max(self.setPoint - 1, self.cfg.MIN_SETPOINT)
        self.updateLights()

    # ---------- Core helpers ----------
    # Read temperature (Fahrenheit) from HAL with defensive fallback.
    # 
    #     Rationale:
    #     - Sensors can fail transiently (I2C glitch, wiring, initialization order).
    #     - The controller must remain responsive even when hardware reads fail.
    #     - Fallback to setpoint is a conservative "safe" choice to prevent extreme behavior.

    def safe_temp_f(self) -> float:
        try:
            # HAL returns temperature in Fahrenheit (controller remains unit-consistent at this boundary).
            return float(self.hal.read_temp_f())
        except Exception as e:
            # Do not crash the controller due to a sensor read issue; log and continue.
            if self.debug: print(f"[WARN] Sensor read failed: {e}")
            return float(self.setPoint)

    # Apply LED outputs based on:
    #     - Current FSM state (off/heat/cool)
    #     - Current temperature vs. setpoint
    #     - Configuration timing for blink/fade behavior
    # 
    #     This method is called:
    #     - Immediately on state/setpoint changes (button actions)
    #     - Periodically by the background loop to keep indicators fresh
    def updateLights(self):
        # Floor to reduce jitter around threshold boundaries (prevents rapid toggling near setpoint).
        temp = floor(self.safe_temp_f())

        if self.debug:
            # These prints make behavior traceable during live demos and troubleshooting.
            print(f"State: {self.current_state.id}")
            print(f"SetPoint: {self.setPoint}")
            print(f"Temp: {temp}")

        try:
            # Off state: no heating/cooling indication.
            if self.current_state.id == "off":
                self.hal.leds_off()
            
            # Heat state: red indicator reflects whether heating is "required".
            elif self.current_state.id == "heat":
                if temp < self.setPoint:
                    # Below setpoint: blink/fade indicates active heating demand.
                    self.hal.red_blink(self.cfg.BLINK_ON, self.cfg.BLINK_OFF, self.cfg.FADE_IN, self.cfg.FADE_OUT)
                else:
                    # At/above setpoint: solid indicates target is satisfied.
                    self.hal.red_solid()
            
            # Cool state: blue indicator reflects whether cooling is "required".
            elif self.current_state.id == "cool":
                if temp > self.setPoint:
                    # Above setpoint: blink/fade indicates active cooling demand.
                    self.hal.blue_blink(self.cfg.BLINK_ON, self.cfg.BLINK_OFF, self.cfg.FADE_IN, self.cfg.FADE_OUT)
                else:
                    # At/below setpoint: solid indicates target is satisfied.
                    self.hal.blue_solid()
        except Exception as e:
            # LED failures should not crash the control loop (GPIO permission, hardware disconnect).
            if self.debug: print(f"[WARN] LED update failed: {e}")

    # Build a compact telemetry payload.
    # 
    #     Format: "<state>, <temp_f>, <setpoint>"
    #     Designed for low-bandwidth serial/UART transport and easy parsing on a receiver.
    def status_string(self) -> str:
        return f"{self.current_state.id}, {self.safe_temp_f():.2f}, {self.setPoint}"

    # ---------- Thread lifecycle ----------
    # Start the controller background loop.
    # 
    #     The loop is daemonized so the program can exit if main thread terminates,
    #     but 'stop()' is still used for a clean shutdown and hardware cleanup.
    def start(self):
        self._thread = Thread(target=self._display_loop, daemon=True)
        self._thread.start()

    # Request controller shutdown and attempt to cleanly release resources.
    # 
    # Behavior:
    # - Signals the background loop to stop using an Event
    # - Joins the thread with a timeout to avoid hanging
    # - Calls HAL.close() to release device handles (LCD, UART, GPIO, etc.)
    def stop(self):
        self._stop.set()
        if self._thread:
            # Timeout prevents indefinite blocking if thread is stuck in a hardware call.
            self._thread.join(timeout=2.0)
        try:
            # HAL is responsible for leaving the hardware in a safe state.
            self.hal.close()
        except Exception:
            # Never raise during shutdown; best-effort cleanup is acceptable here.
            pass

    # Background loop responsible for:
    #     - Updating LCD display text (time + alternating status line)
    #     - Refreshing LED state periodically
    #     - Sending serial telemetry periodically
    # 
    #     The loop uses Event.wait() to provide a sleep that can be interrupted by stop().
    def _display_loop(self):
        counter = 0  # coarse seconds counter used for periodic actions
        alt = 0      # controls alternating between display modes

        while not self._stop.is_set():
            # Capture current timestamp for display; controller treats time as a presentation concern here.
            now = datetime.now()
            line1 = now.strftime("%m/%d %H:%M:%S")

            # Alternate the second line to show both real-time temperature and system state/setpoint.
            if alt < 5:
                line2 = f"Temp: {self.safe_temp_f():.1f}"
            else:
                line2 = f"{self.current_state.id} Set:{self.setPoint}"
            alt = (alt + 1) % 10

            try:
                # HAL abstracts LCD update (controller remains display-device agnostic).
                self.hal.display(line1, line2)
            except Exception as e:
                # Display failures should not terminate the loop. Log and continue.
                if self.debug: print(f"[WARN] Display update failed: {e}")

            # Refresh lights occasionally to keep indicators consistent if external factors change.
            if counter % self.cfg.LIGHT_REFRESH_EVERY_SEC == 0:
                self.updateLights()

            # Send status periodically for external monitoring/logging.
            if counter % self.cfg.SERIAL_SEND_INTERVAL_SEC == 0:
                try:
                    self.hal.serial_send(self.status_string())
                except Exception as e:
                    # Serial may fail if device is absent or permissions change; keep system running.
                    if self.debug: print(f"[WARN] Serial send failed: {e}")

            counter += 1

            # Sleep in an interruptible way: wakes early when stop() sets the event.
            self._stop.wait(self.cfg.DISPLAY_REFRESH_SEC)

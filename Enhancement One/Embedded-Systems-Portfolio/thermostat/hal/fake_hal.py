# Fake Hardware Abstraction Layer (HAL)
# 
# This module provides a simulated HAL implementation used for testing and demos.
# It enables running the ThermostatController without any Raspberry Pi hardware by
# providing deterministic sensor readings and recording output operations.
# 
# Primary goals:
# - Deterministic input (repeatable temperature sequences)
# - Observability (record last LED state, display output, and serial messages)
# - API parity with the real HAL implementation (RpiHAL)

from __future__ import annotations
from collections import deque


# Fake hardware implementation for testing ThermostatController logic
# without Raspberry Pi hardware.
#
# What this class simulates:
# - Sensor: read_temp_f() cycles through a predefined list of temperatures.
# - LEDs: stores the most recent LED "state" as a string for easy assertions.
# - Display: stores the most recent LCD lines.
# - Serial: captures outbound messages in a list to validate telemetry behavior.
#
# Design principles:
# - Simple: behavior is intentionally minimal and predictable.
# - Deterministic: same inputs yield the same outputs every run.
# - Inspectable: state is stored in plain Python fields for debugging/tests.

# Fake HAL used for validation of controller logic.
# 
#     This class is intended to be dependency-injected into ThermostatController
#     during tests or demonstrations. It does not perform any I/O; it only records
#     what would have been sent to hardware.
# 
#     Attributes:
#         temps (deque[float]): Circular buffer of temperature readings.
#         led_state (str): Last LED state requested by controller (e.g., "red_solid").
#         last_display (tuple[str, str]): Last two lines written to the LCD.
#         serial_out (list[str]): List of telemetry messages sent via serial_send().
#         closed (bool): Flag indicating whether close() was called.
class FakeHAL:

    # Initialize fake HAL with an optional temperature sequence.
    # 
    #     Args:
    #         temps: Iterable of temperature readings (Fahrenheit). If omitted,
    #                defaults to a single constant reading of 72.0°F.
    # 
    #     Implementation detail:
    #     - Uses deque + rotate to cycle readings without index bookkeeping.
    def __init__(self, temps=None):
        # Store temps as floats to ensure consistent numeric behavior.
        self.temps = deque([float(t) for t in (temps or [72.0])])

        # Output observability fields (controller writes here instead of hardware).
        self.led_state = "off"
        self.last_display = ("", "")
        self.serial_out = []

        # Cleanup tracking (useful for verifying proper shutdown behavior).
        self.closed = False

    # --- Sensor ---
    # Return a simulated temperature reading in Fahrenheit.
    # 
    #     Behavior:
    #     - Returns the current head element of the deque.
    #     - Rotates left so the next call returns the next value.
    #     - Cycles indefinitely (repeatable pattern).
    def read_temp_f(self) -> float:
        # Rotate through temp readings to simulate changing environment.
        temp = self.temps[0]
        self.temps.rotate(-1)
        return temp

    # --- LEDs ---
    # Simulate turning off all LEDs by recording the requested state.
    def leds_off(self) -> None:
        self.led_state = "off"

    # Simulate steady red LED output by recording the requested state.
    def red_solid(self) -> None:
        self.led_state = "red_solid"

    # Simulate steady blue LED output by recording the requested state.
    def blue_solid(self) -> None:
        self.led_state = "blue_solid"

    # Simulate red LED blinking/fading by recording timing parameters.
    # 
    #     These parameters are captured so tests can confirm that the controller
    #     issued the correct blink policy based on configuration and state.
    def red_blink(self, on: float, off: float, fade_in: float, fade_out: float) -> None:
        self.led_state = f"red_blink(on={on},off={off},fade_in={fade_in},fade_out={fade_out})"

    # Simulate blue LED blinking/fading by recording timing parameters.
    # 
    #     Captures the parameters for easy validation in tests and demos.
    def blue_blink(self, on: float, off: float, fade_in: float, fade_out: float) -> None:
        self.led_state = f"blue_blink(on={on},off={off},fade_in={fade_in},fade_out={fade_out})"

    # --- Display ---
    # Simulate an LCD update by storing the last written lines.
    # 
    #     This provides observability for verifying:
    #     - Formatting correctness
    #     - Alternating display behavior
    #     - State/setpoint visibility
    def display(self, line1: str, line2: str) -> None:
        self.last_display = (line1, line2)

    # --- Serial ---
    # Simulate UART transmission by appending messages to serial_out.
    # 
    #     This enables tests to validate:
    #     - Payload formatting (state, temp, setpoint)
    #     - Send frequency (when driven by controller loop timing)
    def serial_send(self, message: str) -> None:
        self.serial_out.append(message)

    # --- Cleanup ---
    # Mark the HAL as closed.
    # 
    #     Used to confirm the controller performs a clean shutdown sequence.
    def close(self) -> None:
        self.closed = True

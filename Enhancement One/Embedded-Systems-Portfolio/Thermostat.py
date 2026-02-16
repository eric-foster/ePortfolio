# Thermostat Entry Point
# 
# This module is the executable entry point for the embedded thermostat application.
# It wires together:
# 
# - Configuration (ThermostatConfig)
# - Hardware Abstraction Layer (RpiHAL)
# - Controller / Business logic (ThermostatController)
# - Physical input bindings (GPIO buttons)
# 
# Design goals:
# - Keep this file small and orchestration-focused (no business logic here).
# - Depend on interfaces/contracts (HAL + controller) rather than direct device APIs.
# - Provide a single, predictable place to start/stop the system cleanly.
#
#------------------------------------------------------------------
# Change History
#------------------------------------------------------------------
# Version   |   Description
#------------------------------------------------------------------
#    1          Initial Development
#------------------------------------------------------------------
#    2          Refactored to use HAL and Controller pattern
#------------------------------------------------------------------

# Thermostat.py (entry point)

from time import sleep
from gpiozero import Button

from thermostat.config import ThermostatConfig
from thermostat.hal.rpi_hal import RpiHAL
from thermostat.controller import ThermostatController

# Application bootstrap.
# 
#     Responsibilities:
#     1) Load config and construct the HAL (hardware-facing adapter).
#     2) Construct the controller (domain logic / state machine).
#     3) Bind GPIO button events to controller commands.
#     4) Keep the process alive until interrupted, then perform a clean shutdown.
# 
#     Notes:
#     - GPIO button callbacks run in gpiozero-managed background threads.
#     - Controller.stop() should be idempotent and safe to call once on exit.
def main():
    # Centralized configuration keeps pins, timing, and thresholds out of logic.
    cfg = ThermostatConfig()

    # Hardware adapter for Raspberry Pi. All sensor/LED/LCD/UART operations live behind this layer.
    hal = RpiHAL(cfg)

    # Controller owns the thermostat state machine and high-level behavior.
    # `debug=True` enables operational logs helpful for bring-up on real hardware.
    controller = ThermostatController(hal, cfg, debug=True)

    # Start any controller-managed background work (display loop, telemetry loop, etc.).
    controller.start()

    # Ensure indicators immediately reflect current state and temperature.
    # (Useful on boot to avoid "stale" LED state.)
    controller.updateLights()

    # --- Physical Inputs (GPIO Buttons) ---
    # Buttons are intentionally configured here (I/O wiring belongs at the boundary).
    # The controller methods should remain hardware-agnostic, receiving no GPIO specifics.
    greenButton = Button(cfg.BTN_STATE_PIN) # cycles thermostat mode: off -> heat -> cool -> off
    redButton = Button(cfg.BTN_UP_PIN)      # increase setpoint
    blueButton = Button(cfg.BTN_DOWN_PIN)   # decrease setpoint

    # Bind button press events to controller actions.
    # gpiozero triggers these in a background context; handler methods must be quick and thread-safe.
    greenButton.when_pressed = controller.processTempStateButton
    redButton.when_pressed = controller.processTempIncButton
    blueButton.when_pressed = controller.processTempDecButton

    # --- Main Loop ---
    # Keep the process alive. Hardware interaction is managed by the controller/HAL.
    # A minimal loop reduces overhead and ensures Ctrl+C is handled reliably.
    try:
        while True:
            sleep(1)
    except KeyboardInterrupt:
        # Graceful shutdown: stop background threads, release hardware resources, and exit.
        # The controller is responsible for HAL cleanup and stopping any worker threads.
        print("Cleaning up. Exiting...")
        controller.stop()

if __name__ == "__main__":
    # Standard Python entrypoint guard:
    # - Allows importing this module without side effects (useful for tests/tools).
    # - Ensures main() runs only when executed directly.
    main()

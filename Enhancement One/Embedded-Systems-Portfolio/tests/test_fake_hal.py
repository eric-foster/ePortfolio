# Fake HAL Demonstration / Integration Test
# 
# This script exercises the ThermostatController using a Fake Hardware Abstraction Layer (HAL).
# It allows validation of controller behavior without requiring physical hardware.
# 
# Purpose:
# - Demonstrate correct state transitions (off → heat → cool)
# - Validate LED behavior based on temperature vs. setpoint
# - Verify display updates and serial telemetry output
# - Prove that the controller logic is hardware-agnostic and testable
# 
# NOTE:
# This is intentionally written as a runnable script (not a formal unit test)
# to support step-by-step observation during development.

from time import sleep

from thermostat.config import ThermostatConfig
from thermostat.hal.fake_hal import FakeHAL
from thermostat.controller import ThermostatController

# Run a simulated end-to-end thermostat session using FakeHAL.
# 
#     This function:
#     - Injects a deterministic sequence of temperatures
#     - Drives controller state changes manually
#     - Observes LED state, display output, and serial messages
#     - Cleans up controller resources at the end
# 
#     This mirrors how the real system behaves on Raspberry Pi hardware,
#     but without any GPIO, I2C, or UART dependencies.
def run_fake_hal_demo():
    # Load centralized configuration (setpoint bounds, timing constants, etc.)
    cfg = ThermostatConfig()

    # Simulated temperature sequence.
    # Starts below the default setpoint to trigger HEAT blinking,
    # then gradually increases to satisfy the setpoint condition.
    hal = FakeHAL(temps=[68.0, 69.0, 70.0, 71.0, 72.0, 73.0])

    # Instantiate the controller with FakeHAL instead of real hardware.
    # debug=True enables verbose output for demonstration purposes.
    controller = ThermostatController(hal, cfg, debug=True)

    # Start the controller background thread (display + telemetry loop).
    controller.start()

    print("\n--- Initial State ---")
    # Controller should start in the OFF state by default.
    print("State:", controller.current_state.id)
    print("LED:", hal.led_state)

    # Simulate pressing the mode button once: off -> heat
    controller.processTempStateButton()  # off -> heat
    print("\n--- After switching to HEAT ---")
    print("State:", controller.current_state.id)

    # Allow time for background loop to update LEDs, display, and serial output.
    sleep(2)

    # Inspect FakeHAL internal state to verify behavior.
    print("LED:", hal.led_state)
    print("Last display:", hal.last_display)
    print("Serial messages:", hal.serial_out[:2])

    # Increase the setpoint twice.
    # This forces the controller to re-evaluate heating demand.
    controller.processTempIncButton()
    controller.processTempIncButton()
    print("\n--- After increasing setpoint twice ---")
    print("SetPoint:", controller.setPoint)

    sleep(2)

    # LED and display should reflect the updated control logic.
    print("LED:", hal.led_state)
    print("Last display:", hal.last_display)

    # Simulate pressing the mode button again: heat -> cool
    controller.processTempStateButton()
    print("\n--- After switching to COOL ---")
    print("State:", controller.current_state.id)

    sleep(2)

    # Validate cooling indicator behavior.
    print("LED:", hal.led_state)
    print("Last display:", hal.last_display)

    # Request a clean shutdown of the controller.
    # This stops the background thread and closes the HAL.
    controller.stop()
    print("\n--- Shutdown ---")
    print("HAL closed:", hal.closed)

if __name__ == "__main__":
    # Entry point allows this file to be executed directly:
    # 'python tests/test_fake_hal.py'
    run_fake_hal_demo()

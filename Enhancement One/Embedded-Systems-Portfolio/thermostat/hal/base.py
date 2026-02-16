# Hardware Abstraction Layer (HAL) Interface Definition
# 
# This module defines the formal contract between the thermostat controller
# and any hardware-specific implementation.
# 
# Key purpose:
# - Allow the controller to remain completely hardware-agnostic
# - Enable multiple interchangeable implementations (real hardware, fake/simulated, future platforms)
# - Support static type checking and clearer architectural boundaries
# 
# This interface is implemented using typing.Protocol to enable
# duck-typed structural subtyping without requiring inheritance.

from typing import Protocol

# Thermostat Hardware Abstraction Layer contract.
# 
#     Any concrete HAL implementation (Raspberry Pi, FakeHAL, future platforms)
#     must provide all methods defined here with compatible signatures.
# 
#     The controller relies exclusively on this interface and should never import
#     hardware-specific libraries directly.
class ThermostatHAL(Protocol):
    # Read the current ambient temperature in degrees Fahrenheit.
    #
    #     Returns:
    #         float: Current temperature reading.
    # 
    #     Raises:
    #         Implementation-defined exceptions if the sensor read fails.
    #         The controller is responsible for handling failures defensively.
    def read_temp_f(self) -> float: ...

    # ---------- LED Outputs ----------
    # Turn all LED indicators off.
    # 
    #     Used when the thermostat is in the OFF state or during shutdown.
    def leds_off(self) -> None: ...

    # Activate the red LED in a steady (solid) state.
    # 
    #     Indicates that the heating setpoint has been reached or exceeded.
    def red_solid(self) -> None: ...

    # Activate the blue LED in a steady (solid) state.
    # 
    #     Indicates that the cooling setpoint has been satisfied.
    def blue_solid(self) -> None: ...

    # Blink/fade the red LED to indicate active heating demand.
    # 
    #     Args:
    #         on (float): Time in seconds the LED remains on per cycle.
    #         off (float): Time in seconds the LED remains off per cycle.
    #         fade_in (float): Fade-in duration in seconds.
    #         fade_out (float): Fade-out duration in seconds.
    def red_blink(self, on: float, off: float, fade_in: float, fade_out: float) -> None: ...

    # Blink/fade the blue LED to indicate active cooling demand.
    # 
    #     Args:
    #         on (float): Time in seconds the LED remains on per cycle.
    #         off (float): Time in seconds the LED remains off per cycle.
    #         fade_in (float): Fade-in duration in seconds.
    #         fade_out (float): Fade-out duration in seconds.
    def blue_blink(self, on: float, off: float, fade_in: float, fade_out: float) -> None: ...

    # ---------- Display Output ----------
    # Update the LCD display.
    # 
    #     Args:
    #         line1 (str): First line of text (typically date/time).
    #         line2 (str): Second line of text (status, temperature, or setpoint).
    # 
    #     Implementations should handle truncation or padding as required
    #     by the physical display.
    def display(self, line1: str, line2: str) -> None: ...

    # ---------- Serial / Telemetry ----------
    # Send a status message over a serial interface.
    # 
    #     Args:
    #         message (str): Serialized thermostat state payload.
    # 
    #     Intended for external monitoring, logging, or integration
    #     with other systems.
    def serial_send(self, message: str) -> None: ...

    # ---------- Resource Cleanup ----------
    # Release all hardware resources and return the system to a safe state.
    # 
    #     Implementations should:
    #     - Stop LED activity
    #     - Clear or power down the display
    #     - Close serial ports
    #     - Release GPIO resources
    def close(self) -> None: ...

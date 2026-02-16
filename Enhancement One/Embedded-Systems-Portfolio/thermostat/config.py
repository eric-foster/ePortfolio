# Thermostat Configuration
# 
# This module defines all static configuration values used by the thermostat system.
# Configuration is centralized here to:
# 
# - Avoid "magic numbers" scattered throughout the codebase
# - Make system behavior easy to tune without modifying logic
# - Support immutability and predictability at runtime
# - Improve testability by allowing controlled configuration injection
# 
# The configuration is implemented as a frozen dataclass to prevent
# accidental mutation after initialization.

from dataclasses import dataclass

# Immutable configuration container for the thermostat system.
# 
#     All values in this class are intended to be read-only at runtime.
#     If system tuning is required, values should be modified here
#     rather than inside controller or hardware logic.
@dataclass(frozen=True)
class ThermostatConfig:
    # ---------------------------
    # GPIO Pin Assignments
    # ---------------------------
    # Physical pin mappings for LEDs and input buttons.
    # These values are hardware-specific but logically belong in configuration,
    # not in controller or HAL logic.
    RED_LED_PIN: int = 18
    BLUE_LED_PIN: int = 23
    BTN_STATE_PIN: int = 24
    BTN_UP_PIN: int = 12
    BTN_DOWN_PIN: int = 25

    # ---------------------------
    # Temperature Behavior
    # ---------------------------
    # Default and allowable bounds for the thermostat setpoint.
    # Bounds are enforced by the controller to prevent invalid or unsafe values.
    DEFAULT_SETPOINT: int = 72
    MIN_SETPOINT: int = 50
    MAX_SETPOINT: int = 90

    # ---------------------------
    # Timing Configuration
    # ---------------------------
    # Controls refresh rates and periodic behavior throughout the system.
    # All timing values are expressed in seconds for consistency.
    DISPLAY_REFRESH_SEC: float = 1.0
    SERIAL_SEND_INTERVAL_SEC: int = 30
    LIGHT_REFRESH_EVERY_SEC: int = 10

    # ---------------------------
    # LED Animation Parameters
    # ---------------------------
    # Timing values used when blinking/fading LEDs to indicate active heating/cooling.
    # These are passed directly to the HAL and interpreted by the hardware layer.
    BLINK_ON: float = 1.0
    BLINK_OFF: float = 1.0
    FADE_IN: float = 0.5
    FADE_OUT: float = 0.5

    # ---------------------------
    # Serial Communication
    # ---------------------------
    # UART configuration used for periodic telemetry output.
    # These values must align with the external system consuming the data.
    SERIAL_PORT: str = "/dev/ttyS0"
    SERIAL_BAUD: int = 115200
    SERIAL_TIMEOUT: int = 1

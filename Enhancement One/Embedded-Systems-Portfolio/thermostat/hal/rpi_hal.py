# Raspberry Pi Hardware Abstraction Layer (HAL)
# 
# This module provides the concrete Raspberry Pi implementation of the
# ThermostatHAL interface. It encapsulates all direct interactions with:
# 
# - GPIO (LEDs)
# - I2C temperature sensor
# - Character LCD
# - UART / Serial communication
# 
# Design intent:
# - Isolate hardware-specific code from controller logic
# - Allow safe replacement with FakeHAL for testing
# - Centralize device initialization and cleanup

import serial
import board
import digitalio
import adafruit_ahtx0
import adafruit_character_lcd.character_lcd as characterlcd
from gpiozero import PWMLED

# Raspberry Pi implementation of the Thermostat HAL.
# 
#     This class owns all hardware resources and is responsible for:
#     - Initializing devices
#     - Translating abstract controller commands into hardware actions
#     - Leaving the system in a safe state on shutdown
class RpiHAL:
    # Initialize all hardware peripherals using values from configuration.
    # 
    #     Args:
    #         cfg: ThermostatConfig instance containing pin mappings,
    #              serial settings, and other hardware-related constants.
    def __init__(self, cfg):
        # ---------------------------
        # Temperature Sensor (I2C)
        # ---------------------------
        # Shared I2C bus used for environmental sensors.
        self.i2c = board.I2C()
        self.sensor = adafruit_ahtx0.AHTx0(self.i2c)

        # ---------------------------
        # Serial (UART)
        # ---------------------------
        # Used for periodic telemetry output to an external system.
        self.ser = serial.Serial(
            port=cfg.SERIAL_PORT,
            baudrate=cfg.SERIAL_BAUD,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=cfg.SERIAL_TIMEOUT
        )

        # ---------------------------
        # LED Outputs
        # ---------------------------
        # PWMLED allows fade/blink animations without manual timing loops.
        self.red = PWMLED(cfg.RED_LED_PIN)
        self.blue = PWMLED(cfg.BLUE_LED_PIN)

        # ---------------------------
        # LCD Display Wiring
        # ---------------------------
        # Explicit pin mapping matches physical wiring on the Raspberry Pi.
        # These pins are intentionally kept here (hardware boundary).
        self.lcd_rs = digitalio.DigitalInOut(board.D17)
        self.lcd_en = digitalio.DigitalInOut(board.D27)
        self.lcd_d4 = digitalio.DigitalInOut(board.D5)
        self.lcd_d5 = digitalio.DigitalInOut(board.D6)
        self.lcd_d6 = digitalio.DigitalInOut(board.D13)
        self.lcd_d7 = digitalio.DigitalInOut(board.D26)

        # Initialize a 16x2 monochrome character LCD.
        self.lcd = characterlcd.Character_LCD_Mono(
            self.lcd_rs, self.lcd_en,
            self.lcd_d4, self.lcd_d5, self.lcd_d6, self.lcd_d7,
            16, 2
        )

        # Clear display on startup to avoid showing stale data.
        self.lcd.clear()

    # --- Sensor ---
    # Read the ambient temperature from the sensor and convert to Fahrenheit.
    # 
    #     Returns:
    #         float: Temperature in degrees Fahrenheit.
    def read_temp_f(self) -> float:
        c = self.sensor.temperature
        return (9.0 / 5.0) * c + 32.0

    # --- LEDs ---
    # Turn off all LED indicators.
    # 
    #     Used when the thermostat is in OFF mode or during shutdown.
    def leds_off(self) -> None:
        self.red.off()
        self.blue.off()

    # Illuminate the red LED steadily.
    # 
    #     Indicates heating target has been reached or exceeded.
    def red_solid(self) -> None:
        self.blue.off()
        self.red.on()

    # Illuminate the blue LED steadily.
    # 
    #     Indicates cooling target has been satisfied.
    def blue_solid(self) -> None:
        self.red.off()
        self.blue.on()

    # Blink/fade the red LED to indicate active heating demand.
    # 
    #     Args:
    #         on: LED on duration (seconds)
    #         off: LED off duration (seconds)
    #         fade_in: Fade-in duration (seconds)
    #         fade_out: Fade-out duration (seconds)
    def red_blink(self, on, off, fade_in, fade_out) -> None:
        self.blue.off()
        self.red.blink(on_time=on, off_time=off, fade_in_time=fade_in, fade_out_time=fade_out)

    # Blink/fade the blue LED to indicate active cooling demand.
    # 
    #     Args:
    #         on: LED on duration (seconds)
    #         off: LED off duration (seconds)
    #         fade_in: Fade-in duration (seconds)
    #         fade_out: Fade-out duration (seconds)
    def blue_blink(self, on, off, fade_in, fade_out) -> None:
        self.red.off()
        self.blue.blink(on_time=on, off_time=off, fade_in_time=fade_in, fade_out_time=fade_out)

    # --- Display ---
    # Update the LCD display with two lines of text.
    # 
    #     Args:
    #         line1: First line (typically date/time)
    #         line2: Second line (temperature, state, or setpoint)
    def display(self, line1: str, line2: str) -> None:
        self.lcd.clear()
        self.lcd.message = f"{line1}\n{line2}"

    # --- Serial ---
    # Send a single telemetry message over the serial interface.
    # 
    #     Args:
    #         message: Serialized thermostat state payload
    def serial_send(self, message: str) -> None:
        self.ser.write(message.encode())
        self.ser.write(b"\n")

    # --- Cleanup ---
    # Release all hardware resources and leave the system in a safe state.
    # 
    #     This method is designed to be:
    #     - Idempotent (safe to call once)
    #     - Best-effort (cleanup failures should not crash shutdown)
    def close(self) -> None:
        # Clear display if possible.
        try:
            self.lcd.clear()
        except Exception:
            pass
        
        # Deinitialize all LCD GPIO pins.
        for pin in [self.lcd_rs, self.lcd_en, self.lcd_d4, self.lcd_d5, self.lcd_d6, self.lcd_d7]:
            try:
                pin.deinit()
            except Exception:
                pass

        # Ensure LEDs are turned off.
        try:
            self.leds_off()
        except Exception:
            pass

        # Close serial port.
        try:
            self.ser.close()
        except Exception:
            pass

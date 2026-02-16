# Embedded Thermostat Control System

A modular, hardware-abstracted embedded thermostat control system designed for Raspberry Pi environments. This project demonstrates professional embedded software engineering practices, including layered architecture, hardware abstraction, finite state machines, and testability without physical hardware.

---

## Overview

This project implements a **thermostat control system** using a **finite state machine (FSM)** architecture and a **Hardware Abstraction Layer (HAL)** to decouple control logic from hardware-specific implementations.

The system is designed to be:
- Maintainable
- Testable without hardware
- Extensible to other embedded platforms
- Aligned with modern embedded and computing systems design practices

---

## Key Features

- Finite State Machine–based control logic (`off`, `heat`, `cool`)
- Hardware Abstraction Layer (HAL) for clean separation of concerns
- Raspberry Pi hardware implementation
- Fake HAL for simulation and testing
- Configurable temperature setpoint
- Button-driven state transitions
- LCD-based system feedback
- Periodic serial telemetry output
- Threaded display management for responsiveness

---

## System Behavior

### Thermostat States

| State | Description |
|-----|------------|
| `off` | System idle, LEDs off |
| `heat` | Heating logic active (red LED indicator) |
| `cool` | Cooling logic active (blue LED indicator) |

### Functional Flow
1. Temperature is sampled from a sensor
2. Current state and setpoint are evaluated
3. Output indicators are updated accordingly
4. System status is displayed on an LCD
5. Periodic telemetry is sent over UART

---

## Architecture

### High-Level Design

```
+------------------+
|   Thermostat.py |
|  (Entry Point)  |
+--------+--------+
         |
         v
+---------------------+
| Controller Layer    |
| thermostat/         |
| controller.py       |
+----------+----------+
           |
           v
+---------------------+
| Hardware Abstraction|
| thermostat/hal/     |
| base.py             |
| rpi_hal.py          |
| fake_hal.py         |
+---------------------+
```

### Design Principles

- Separation of concerns
- Dependency inversion
- Interface-driven development
- Hardware-independent testing
- Embedded-safe design patterns

---

## Project Structure

```
Embedded-Systems-Portfolio/
│
├── Thermostat.py              # Application entry point
├── README.md                  # Project documentation
├── requirements.txt           # Python dependencies
│
├── thermostat/                # Core application package
│   ├── __init__.py
│   ├── config.py              # Centralized configuration
│   ├── controller.py          # FSM & control logic
│   └── hal/                   # Hardware abstraction layer
│       ├── __init__.py
│       ├── base.py            # HAL interface definition
│       ├── rpi_hal.py         # Raspberry Pi HAL
│       └── fake_hal.py        # Simulated HAL for testing
│
├── tests/                     # Automated tests
│   ├── __init__.py
│   └── test_fake_hal.py       # Fake HAL unit tests
│
└── docs/                      # Design documentation
    ├── Smart Thermostat Prototype Architecture.pdf
    └── Thermostat State Machine.drawio.pdf
```

---

## Hardware Abstraction Layer (HAL)

The HAL isolates all hardware-dependent logic behind a defined interface, allowing the controller to operate independently of physical devices.

### Benefits
- Enables unit testing without hardware
- Simplifies future platform migration
- Improves maintainability
- Reduces coupling between system layers

### HAL Implementations

| File | Purpose |
|----|--------|
| `base.py` | Defines the HAL interface |
| `rpi_hal.py` | Real Raspberry Pi hardware implementation |
| `fake_hal.py` | Simulated HAL for testing and development |

---

## Testing

Testing is performed using the **Fake HAL**, enabling deterministic validation of system behavior without requiring physical hardware.

### Covered Areas
- Temperature sampling
- LED state transitions
- Button interactions
- Controller-HAL integration

### Run Tests

```bash
python -m tests.test_fake_hal
```

> Tests must be executed from the project root directory to ensure proper package resolution.

---

## Setup & Installation

### Clone the Repository

```bash
git clone <repository-url>
cd <folder path>
```

### Create a Virtual Environment

```bash
python3 -m venv .venv
source .venv/bin/activate
```

### Install Dependencies

```bash
pip install -r requirements.txt
```

> On Raspberry Pi OS, Python is externally managed. Virtual environments are required for non-system packages.

---

## Running the Application

### With Raspberry Pi Hardware

```bash
sudo .venv/bin/python Thermostat.py
```

### With Fake HAL (Simulation / Testing)

Configure the controller to use `FakeHAL` instead of the Raspberry Pi HAL.

---

## Design Trade-Offs

| Design Decision | Rationale |
|---------------|-----------|
| HAL abstraction | Small overhead for major gains in testability |
| FSM architecture | Improves clarity and correctness |
| Threaded display | Enables responsive UI updates |
| Config isolation | Simplifies tuning and future expansion |

---

## Documentation

Additional system documentation is available in the `docs/` directory, including:
- Architecture diagrams
- State machine diagrams
- Control flow visualizations

These artifacts support long-term maintainability and system comprehension.

---

## Future Enhancements

Potential improvements include:
- Persistent configuration storage
- Network-based telemetry
- Power-aware scheduling
- Fault detection and recovery
- Support for additional embedded platforms

---

## Author

**Eric Foster**

Focus areas:
- Embedded Software / Software Engineering  
- Computing Systems

---

## License

This project is provided for educational and portfolio purposes.  
Reuse or modification should include attribution.

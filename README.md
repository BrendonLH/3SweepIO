# Warehouse Sweeper Robot

An ESP32-powered warehouse sweeper robot focused on wireless control, localization, telemetry, and future autonomous navigation.

This project combines embedded systems, robotics, wireless communication, IoT dashboards, custom electronics, and 3D-printed mechanical design into a single platform.

---

# Project Goals

The long-term goal is to develop a low-profile autonomous warehouse sweeper capable of:

- Navigating warehouse environments
- Tracking its position using UWB localization
- Correcting drive drift using wheel encoders
- Reporting telemetry to a dashboard
- Operating in Manual, Semi-Autonomous, and Autonomous modes
- Performing cleaning tasks safely around warehouse traffic

---

# Current Status

## Completed

- ESP32 robot controller
- ESP32-C3 handheld controller
- ESP-NOW wireless communication
- BTS7960 motor control
- Main battery power system
- Fuse protection
- Power switch integration
- Safety timeout system
- Optical encoder hardware design
- Encoder sensor validation
- Status LED connection indicator

## In Progress

- Encoder pulse counting
- Drift correction system
- UWB anchor setup
- Trilateration proof of concept
- Dashboard architecture
- System documentation

## Planned

- Battery monitoring
- RGB status indicators
- SVG warehouse map
- Position visualization
- Semi-autonomous navigation
- Autonomous sweeping
- TPU tire system
- Docking station

---

# Documentation

## Core Systems

- [Robot System](docs/robot.md)
- [Controller System](docs/controller.md)
- [Pylon System](docs/pylon.md)

## Diagrams

- [Robot Diagram](docs/diagrams/robot.drawio)
- [Controller Diagram](docs/diagrams/controller.drawio)
- [Pylon Diagram](docs/diagrams/pylon.drawio)

---

# Repository Structure

```text
src/           Active production firmware
include/       Header files
lib/           Custom libraries
docs/          Documentation and diagrams
archive/       Historical experiments and prototype code

platformio.ini PlatformIO configuration
README.md      Project overview
```

---

# Hardware

## Robot

- ESP32
- BTS7960 Motor Driver
- 12V Battery System
- Fuse Protection
- Main Power Switch
- Optical Wheel Encoders
- UWB Localization Module
- Status Indicator LED

## Controller

- ESP32-C3
- Joystick Input
- Control Buttons
- ESP-NOW Communication

## Positioning System

- UWB Anchors (Pylons)
- Trilateration Engine
- Dashboard Position Tracking

---

# Software Stack

## Embedded Development

- PlatformIO
- Arduino Framework
- ESP32

## Source Control

- Git
- GitHub

## Documentation

- Markdown
- Draw.io

## Mechanical Design

- Onshape
- Bambu Studio

## Dashboard & IoT

- Node-RED
- MQTT (Future)
- SVG Visualization

---

# Development Workflow

1. Design hardware in Onshape
2. Document architecture in Draw.io
3. Develop firmware in PlatformIO
4. Commit changes to Git
5. Push updates to GitHub
6. Update documentation
7. Test and validate

---

# Version Roadmap

## V1

Manual wireless robot platform

Features:

- Wireless control
- Safety timeout
- Encoder feedback
- UWB positioning
- Dashboard proof of concept

## V2

Enhanced robot platform

Features:

- TPU tires
- Battery monitoring
- RGB status indicators
- Improved chassis
- Enhanced telemetry

## V3

Autonomous warehouse sweeper

Features:

- Autonomous navigation
- Path planning
- Docking station
- Autonomous sweeping
- Obstacle avoidance

---

# Archive

Historical experiments and retired prototypes are stored in:

```text
archive/
```

See:

```text
archive/README.md
```

for details regarding archived code and project history.

---

# Author

Brendon Hampton

Warehouse automation, robotics, embedded systems, IoT, and software development.

---

# License

To be determined.
# Archive

This folder contains retired experiments, prototype code, test sketches, and historical versions of subsystems used during development of the Warehouse Sweeper Robot.

Files stored here are preserved for reference and learning purposes but are **not considered active production code**.

## Purpose

The archive exists to:

- Preserve previous work
- Retain useful test code
- Document design evolution
- Provide reference implementations
- Avoid losing solved problems

## Organization

### encoder/

Encoder tests, pulse counting experiments, sensor validation code, and tuning prototypes.

### motors/

Motor driver tests, PWM experiments, drive control prototypes, and movement testing.

### controller/

Controller firmware experiments, joystick testing, button handling, and communication prototypes.

### uwb/

Distance measurement tests, anchor experiments, trilateration prototypes, and positioning research.

### misc/

Temporary utilities, one-off experiments, debugging tools, and miscellaneous development files.

## Notes

Archived files may:

- Use outdated pin assignments
- Reference removed hardware
- Depend on old libraries
- Fail to compile without modification

Before reusing archived code, review and validate it against the current project architecture.

## Philosophy

Production code belongs in `src/`.

Historical code belongs in `archive/`.

When in doubt, archive it instead of deleting it.
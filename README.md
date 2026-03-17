# ESP32 Brushed DC Motor Firmware Skeleton (ESP-IDF)

Minimal, production-structured starter project for a brushed DC motor controller on ESP32.

## Goals

- Modular architecture with isolated responsibilities:
  - `board`: board/pin initialization and hardware map.
  - `motor`: low-level motor driver (PWM + direction GPIO).
  - `control`: high-level control command pipeline (open-loop for now).
  - `safety`: system checks and fault-latching placeholders.
- Implement one motor in open-loop mode now.
- Keep extension points for encoder and current sensing.

## Project layout

```text
.
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   └── main.c
└── components/
    ├── board/
    │   ├── CMakeLists.txt
    │   ├── include/board.h
    │   └── src/board.c
    ├── control/
    │   ├── CMakeLists.txt
    │   ├── include/control.h
    │   └── src/control.c
    ├── motor/
    │   ├── CMakeLists.txt
    │   ├── include/motor.h
    │   └── src/motor.c
    └── safety/
        ├── CMakeLists.txt
        ├── include/safety.h
        └── src/safety.c
```

## Current implementation

- One motor channel (`MOTOR_1`) driven by:
  - LEDC PWM output pin (`MOTOR1_PWM_GPIO`)
  - Direction output pin (`MOTOR1_DIR_GPIO`)
- Open-loop command path:
  - main loop generates a command (`speed`, `direction`)
  - control component validates/clamps and forwards to motor
  - motor component applies PWM duty + direction pin level
- Safety check hook called periodically.

## Build and flash (ESP-IDF)

```bash
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## Next steps (placeholders already present)

1. **Encoder feedback**
   - Add encoder driver (PCNT/RMT/GPIO ISR) in a dedicated component.
   - Feed measured speed into `control` for closed-loop PI/PID.
2. **Current sensing**
   - Add ADC sampling + filtering.
   - Add over-current checks in `safety` and fault latching.
3. **Safety hardening**
   - Brownout/undervoltage handling.
   - Watchdog integration.
   - Fault codes + telemetry.
4. **Scalability**
   - Expand board map + motor instances for multi-motor systems.

## Notes

- Pin values are example defaults; update `components/board/include/board.h` per hardware.
- PWM frequency and resolution are conservative defaults and should be tuned for your motor driver.

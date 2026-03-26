# Shushan TV — IoT Ceiling Recliner Controller

ESP-IDF firmware for an ESP32-based motor controller that raises/lowers a TV
mounted on a ceiling recliner via a 24 V linear actuator.

## Hardware

| Component              | Detail                                    |
|------------------------|-------------------------------------------|
| MCU                    | ESP32 DevKit V1 (USB-C)                   |
| Power                  | 24 V in → buck to 5 V → LDO to 3.3 V     |
| Actuator               | DHLA8000L 24 V linear actuator            |
| Motor drive            | SPDT relay pair (extend / retract)        |
| Limit sensing          | Hall-effect sensor (GL800/XL800), x2      |
| Remote control         | Generic 433 MHz receiver (1 data output)  |
| Connectivity (planned) | BLE, Wi-Fi                                |

## Project layout

```text
.
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   ├── main.c              ← entry point (Phase A: hello-world)
│   └── app_types.h         ← shared enums (command_t, motor_state_t)
└── components/
    ├── board/               ← pin map + GPIO init
    ├── hello_world/         ← Phase A bring-up (LED blink + button)
    ├── motor_ctrl/          ← relay-based actuator state machine
    ├── limit_inputs/        ← hall sensor reading + debounce
    ├── command_bus/         ← FreeRTOS queue routing commands
    └── safety/              ← limit enforcement + timeout protection
```

## Development phases

1. **Phase A — Bring-up** (current): blink LED, serial log, button test
2. **Phase B — Motor control**: relay state machine, limits, safety
3. **Phase C — 433 MHz remote**: decode protocol, map buttons
4. **Phase D — BLE**: generic phone app control
5. **Phase E — Wi-Fi / IoT**: local web UI or Blynk, OTA

## Build and flash

```bash
idf.py set-target esp32
idf.py build
idf.py -p COMx flash monitor
```

Replace `COMx` with your actual serial port (e.g. `COM3`).

## Pin assignments

Edit `components/board/include/board.h` to match your PCB.
Default provisional assignments:

| Signal          | GPIO | Notes                     |
|-----------------|------|---------------------------|
| Relay Extend    | 18   | Output, active-high       |
| Relay Retract   | 19   | Output, active-high       |
| Limit Extend    | 21   | Input, active-low, pull-up|
| Limit Retract   | 22   | Input, active-low, pull-up|
| 433 MHz Data    | 23   | Input                     |
| Built-in LED    | 2    | DevKit V1 blue LED        |
| BOOT Button     | 0    | Active-low, pull-up       |

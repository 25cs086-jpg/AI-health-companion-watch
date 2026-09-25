# Hardware Components

## Suggested prototype components

| Component | Purpose |
|---|---|
| ESP32 | Main controller and wireless connectivity |
| MAX30102 | Heart-rate sensing |
| MPU6050 | Motion, acceleration and fall detection |
| GPS module | Location during an emergency |
| Push button | User emergency / "I'm OK" response |
| Vibration motor | Haptic feedback |
| Buzzer | Audible feedback |
| Battery + power circuit | Portable power |

## Important electrical note

Do not connect a vibration motor directly to an ESP32 GPIO. Use an appropriate transistor/MOSFET driver, flyback protection where applicable, and a suitable external supply.

Sensor modules may have different voltage requirements. Check the exact breakout-board documentation before wiring.

## Example I2C wiring

For common ESP32 boards:

- MAX30102 SDA → GPIO 21
- MAX30102 SCL → GPIO 22
- MPU6050 SDA → GPIO 21
- MPU6050 SCL → GPIO 22
- Common GND

Both I2C devices can share SDA/SCL if their addresses do not conflict.

Example additional pins:

- Emergency/OK button → GPIO 27
- Vibration driver → GPIO 26
- Buzzer → GPIO 25
- GPS RX/TX → hardware serial pins selected for your board

Verify pin availability on your exact ESP32 board before wiring.

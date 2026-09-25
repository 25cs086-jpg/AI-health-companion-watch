# AI-Powered Personal Health Companion Watch

A hackathon prototype that combines continuous heart-rate monitoring, fall detection, user-response verification, and emergency communication in a wearable companion.

## Features

- ESP32-based wearable controller
- Heart-rate monitoring
- Accelerometer/gyroscope based fall detection
- Emergency button
- Vibration/buzzer feedback
- "Are you okay?" response flow after a detected fall
- Optional GPS location
- Web dashboard deployable with GitHub Pages
- Demo/simulation mode for the live preview

> **Prototype note:** This project is a hackathon prototype, not a medical device. Sensor thresholds and emergency logic must be validated before real-world use.

## Hardware

Typical components:
- ESP32 development board
- MAX30102 heart-rate / pulse-oximeter sensor
- MPU6050 accelerometer + gyroscope
- GPS module (optional, e.g. NEO-6M)
- Push button
- Vibration motor (use a suitable transistor/MOSFET driver for a real motor)
- Buzzer (optional)
- Li-ion/LiPo battery with a suitable power-management circuit

See `hardware/components.md` and `hardware/esp32_code.ino`.

## Web dashboard

The dashboard is a static site and can be hosted on GitHub Pages.

### GitHub Pages setup

1. Create a new GitHub repository, for example `AI-Health-Companion-Watch`.
2. Upload this project.
3. Open **Settings → Pages**.
4. Under **Build and deployment**, select **Deploy from a branch**.
5. Select the `main` branch and the `/web-dashboard` folder if your Pages UI offers folder selection.
6. Save.
7. GitHub will provide a Pages URL similar to:
   `https://YOUR-USERNAME.github.io/AI-Health-Companion-Watch/`

If your Pages configuration only offers `/root`, copy the contents of `web-dashboard/` to the repository root, or use the included GitHub Actions workflow in `.github/workflows/pages.yml`.

## GitHub Actions Pages deployment

The included workflow publishes `web-dashboard/` automatically on pushes to `main`.

Go to **Settings → Pages → Source** and choose **GitHub Actions**.

## Live demo

Open the GitHub Pages URL and use the dashboard's **Demo Mode** controls:

- Simulate normal health
- Simulate fall
- Simulate emergency
- Reset

This lets judges see the complete user journey even when the physical watch is not connected to the browser.

## Real hardware data

The ESP32 sketch currently prints JSON telemetry over Serial so you can verify sensor readings during the hardware demo.

For a production-connected dashboard, add a backend or a supported IoT transport such as MQTT, HTTPS, or WebSocket. Do not put private API keys or guardian phone numbers in a public GitHub repository.

## Safety

Do not rely on this prototype for medical diagnosis or emergency response. Test false-positive/false-negative behavior and use appropriate electrical protection and motor drivers.

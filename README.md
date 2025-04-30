# 🧠 Multithreaded GPIO LED Controller (C)

This project is a multi-threaded C application for embedded Linux systems (e.g., Raspberry Pi, LinkIt Smart) that controls 8 LEDs using GPIO pins and cycles between different LED display patterns based on button input. It uses POSIX threads and accesses GPIO via the Linux sysfs interface.

---

## 📦 Files Overview

| File | Purpose |
|------|---------|
| `main.c` | Core application logic: LED control and button monitoring using multithreading. |
| `main.h` | GPIO helper functions for exporting, setting direction/value, and reading pins. |
| `makefile` | Build instructions (not visible here, but assumed to compile with `-pthread`). |

---

## 🚀 Features

- Uses **8 GPIO LEDs** with multiple display patterns.
- Responds to **button press input** to switch patterns.
- Runs two **POSIX threads**:
  - LED pattern control
  - Button state detection
- GPIO access through **/sys/class/gpio/** (sysfs interface).
- Thread synchronization using **mutex locks**.

---

## 🎮 LED Pattern Modes

| Mode | Description |
|------|-------------|
| `State 0` | One LED walks from left to right. |
| `State 1` | LEDs rotate in a circular pattern. |
| `State 2` | Flash patterns (ALL ON, ALL OFF, 4 ON left, 4 ON right). |
| `State 3` | All LEDs ON → OFF alternation. |

Button presses (on GPIO 14) cycle through states: `0 → 1 → 2 → 3 → 0 ...`.

---

## 🧪 How It Works

### Main Thread (`main`)
- Initializes mutexes and GPIO.
- Starts LED control and button monitoring threads.

### Button Thread (`button_function`)
- Monitors button GPIO input (rising edge detection).
- Changes `buttonState` (0 to 3).
- Updates `LEDstatus` bitmask accordingly.

### LED Thread (`child`)
- Based on current `buttonState`, runs one of four loop functions (`buttonStateXLoop`) that update LEDs based on `LEDstatus`.

---

## 💡 GPIO Interface (from `main.h`)

| Function | Description |
|----------|-------------|
| `gpio_export(gpio)` | Makes a GPIO visible in `/sys/class/gpio/`. |
| `gpio_unexport(gpio)` | Hides the GPIO when done. |
| `gpio_set_dir(gpio, "in"/"out")` | Sets GPIO direction. |
| `gpio_set_value(gpio, val)` | Sets GPIO pin high (1) or low (0). |
| `gpio_get_value(gpio)` | Reads value from input GPIO. |

---

## 🧰 Build & Run

### 🔧 Prerequisites
- A Linux device with GPIO pins (e.g., Raspberry Pi).
- C compiler (e.g., `gcc`).
- GPIO permissions via sysfs.

### ⚙️ Compilation
make

Or manually:

gcc -pthread main.c -o led_app

## ▶️ Run the Program

sudo ./led_app

❗ sudo is required to access GPIO via /sys/class/gpio/.

## 🔐 Suggestions for Enhancement
- Add debouncing logic to button detection.

- Improve bitmask logic readability:

((LEDstatus >> j) & 1) == 1

- Add graceful cleanup (e.g., unexport GPIOs on exit).

- Use #define for constants like LED count and pin numbers.

## 📸 System Diagram

Button GPIO (14)

      ↓

[Button Thread] ----> Updates State

      ↓

[LED Thread] ----> Uses State to control LEDs via GPIOs (2-4, 17, 27, 22, 10, 9)



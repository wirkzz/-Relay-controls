# 🔌 Relay Control via Telegram with ESP8266 📲

This project allows you to control **up to 4 relays** connected to a relay module using **Telegram commands**, through an **ESP8266** (like ESP-01 or NodeMCU).  
It's perfect for home automation, remote control of devices, or simple and effective IoT systems. ⚡️🌐

---

## 🧠 How it works

- The ESP8266 connects to your Wi-Fi network.  
- It gets the current time from NTP servers.  
- It securely communicates with your Telegram bot.  
- When it receives a command like `/rele1`, it toggles the corresponding relay.  
- The bot replies with the updated relay status. 🔁  

---

## 🛠️ Technologies used

- 🧠 **ESP8266** — Wi-Fi microcontroller  
- 📡 **WiFiClientSecure** — HTTPS connection  
- 🤖 **UniversalTelegramBot** — Telegram Bot API integration  
- 🧩 **ArduinoJson** — JSON message parsing  
- ⏱️ **NTP (Network Time Protocol)** — Time synchronization  

---

## 📦 Dependencies

Make sure you have the following libraries installed in the **Arduino IDE**:

- `ESP8266WiFi`
- `WiFiClientSecure`
- `UniversalTelegramBot`
- `ArduinoJson`

You can install them via the **Library Manager**.

---

## ⚙️ Setup

1. Open the `.ino` file in **Arduino IDE**.
2. Replace the placeholders with your actual data:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define BOT_TOKEN "YOUR_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"

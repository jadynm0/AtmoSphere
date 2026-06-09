# AtmoSphere

AtmoSphere is a pair of interconnected ambient light spheres that sense environmental changes in one location and reflect them in another. Instead of sending messages or notifications, AtmoSphere quietly shares a sense of presence by translating local light levels into a synced glow.

This project was built for MakeUofT 2026 as a hardware/software prototype for helping distant spaces feel more connected.

## Inspiration

Long-distance relationships can make it hard to feel physically present with another person. AtmoSphere was inspired by the idea that two people in different places could still feel connected through subtle environmental changes, like the brightness of a room, sunlight, or lamplight.

Rather than directly communicating information, AtmoSphere communicates atmosphere.

## What It Does

AtmoSphere uses a light sensor in one sphere to detect the surrounding light level in real time. That value is sent to a Python Flask server, which acts as a bridge between the two devices.

The second sphere requests the latest light value from the server and adjusts its LED brightness based on the environment of the first sphere. The light is categorized into four states:

- **Off**: very low light
- **Dim**: low light
- **Warm**: medium light
- **Bright**: high light

An LCD display on the receiver also shows the current synced state and light value.

## How It Works

The project is split into three main parts:

### `ball1.ino`

The sender sphere:

- Connects to Wi-Fi
- Reads light levels from an LDR/photoresistor
- Maps the sensor value to a brightness state
- Updates its own LED brightness
- Sends the sensor value to the Flask server

### `bridge.py`

The data bridge:

- Runs a Flask server
- Receives light values from the sender sphere through `/update`
- Stores the most recent light value
- Sends the latest value to the receiver sphere through `/get_light`

### `ball2.ino`

The receiver sphere:

- Connects to Wi-Fi
- Requests the latest light value from the Flask server
- Maps the received value to the same brightness states
- Updates its LED brightness
- Displays the synced state and value on an LCD

## Built With

- Arduino IDE
- C++
- Python
- Flask
- ESP8266 microcontrollers
- Light-dependent resistors (LDRs)
- LEDs
- LCD display

## Hardware Used

- 2 ESP8266 microcontrollers
- 1 light sensor/photoresistor
- LEDs
- LCD display
- Jumper wires
- Breadboard
- Power source
- Sphere/lamp enclosure

## Getting Started

### Prerequisites

Before running the project, make sure you have:

- Arduino IDE installed
- Python installed
- Flask installed
- Two ESP8266 boards
- All required Arduino libraries for Wi-Fi, HTTP requests, and LCD control

Install Flask with:

```bash
pip install flask
```

## Setup

### 1. Clone the repository

```bash
git clone https://github.com/jadynm0/AtmoSphere.git
cd AtmoSphere
```

### 2. Start the Flask bridge server

Run the Python server:

```bash
python bridge.py
```

The server runs on port `5000`.

Make sure your computer and both ESP8266 boards are connected to the same network.

### 3. Update Wi-Fi and server settings

In both `ball1.ino` and `ball2.ino`, replace the Wi-Fi and server information with your own:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverIp = "YOUR_SERVER_IP";
```

The server IP should be the local IP address of the computer running `bridge.py`.

### 4. Upload the Arduino code

Upload `ball1.ino` to the sender ESP8266.

Upload `ball2.ino` to the receiver ESP8266.

Once both boards are running, the sender will begin reading light values and sending them to the Flask server. The receiver will retrieve those values and update its LED and LCD display.

## Challenges

One of the biggest challenges was working with hardware for the first time. The original plan used LED strips, but they were more difficult to control and caused server-related issues, so the project pivoted to regular LEDs.

Another challenge was calibrating the light sensor. The actual environmental light values were much lower than expected, so the brightness mapping had to be adjusted to make the lamp responsive even in low-light conditions.

## What We Learned

Through this project, we learned how to connect hardware and software together using microcontrollers, sensors, LEDs, and a Python server. We also learned how to use the Arduino IDE to build hardware-based interactions and how to debug issues between physical devices and a web server.

## Future Improvements

Future versions of AtmoSphere could include:

- Two-way communication between both spheres
- Temperature and humidity sensing
- Color-changing lights
- Brighter LEDs or LED strips
- A more polished physical enclosure
- Cloud hosting so the spheres can sync from different networks
- A mobile or web interface for setup and customization

## Team

Created by:

- Jadyn Mo
- Jessica Su
- Tamima Wadageri

DEVPOST link: https://devpost.com/software/atmosphere-kdzjtg


# LORM Glove System

A smart communication system that enables deafblind individuals to communicate using the LORM tactile alphabet. The system consists of a specialized glove with sensors and a web application interface.

## Project Overview

The LORM Glove System translates tactile inputs into digital messages and vice versa, enabling seamless communication between deafblind individuals and their communication partners. The project combines hardware (Arduino-based glove) with a web application interface.

### Key Features

- Real-time tactile communication
- Wireless connectivity
- Firebase integration for message storage and real-time updates
- Web-based interface for message visualization
- Support for distance communication
- Long battery life
- Durable and comfortable design

## Project Structure

```
lorm/
├── Arduino/               # Hardware-related code
│   ├── V2/               # Latest version of Arduino code
│   └── ...               # Previous versions and tests
└── lorm-app/             # Web application
    ├── src/              # Source code
    ├── public/           # Public assets
    └── ...               # Configuration files
```

## Hardware Setup

### Components Required

1. NodeMCU ESP8266
2. Arduino board
3. Tactile sensors
4. I2C communication modules
5. Power supply components

### Hardware Installation

1. Connect the tactile sensors to the Arduino board according to the pin configuration in the code
2. Set up I2C communication between Arduino and NodeMCU
3. Upload the appropriate code to each board:
   - For Arduino: Use `Arduino/V2/Arduino-code-11-buttons-I2C/Arduino-code-11-buttons-I2C.ino`
   - For NodeMCU: Use `Arduino/V2/NodeMCU_I2C_Receiver_Code_with_Firebase/NodeMCU_I2C_Receiver_Code_with_Firebase.ino`

## Web Application Setup

### Prerequisites

- PHP 7.4 or higher
- Node.js and npm
- Firebase account

### Installation

1. Clone the repository:
```bash
git clone [repository-url]
cd lorm
```

2. Install web application dependencies:
```bash
cd lorm-app
npm install
composer install
```

3. Configure Firebase:
   - Create a new Firebase project
   - Copy your Firebase credentials to `src/config/firebase.js`
   - Update the Firebase configuration in the NodeMCU code

4. Configure WiFi settings:
   - Update `WIFI_SSID` and `WIFI_PASSWORD` in the NodeMCU code

5. Start the web application:
```bash
php -S localhost:8000 -t public
```

## Usage

1. Power on the LORM glove
2. Connect to WiFi (automatic after configuration)
3. Access the web interface at `http://localhost:8000`
4. Start communicating:
   - Tactile inputs on the glove are converted to text
   - Messages appear in real-time on the web interface
   - Responses can be sent back through the system

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.




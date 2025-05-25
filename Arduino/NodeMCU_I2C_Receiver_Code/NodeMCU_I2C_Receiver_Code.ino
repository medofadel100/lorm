// NodeMCU I2C Receiver Code
// Receives text messages from Arduino via I2C
// ESP8266 NodeMCU

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

// I2C pin definitions for NodeMCU
// D1 = GPIO 5 (SCL)
// D2 = GPIO 4 (SDA)

#define I2C_SLAVE_ADDRESS 8  // Same address as defined in Arduino code

// Buffer to store incoming message
const int BUFFER_SIZE = 100;
char messageBuffer[BUFFER_SIZE];
int bufferIndex = 0;
bool messageReceived = false;

// Function called when I2C data is received
void receiveEvent(int numBytes) {
  // Reset buffer and index if this is a new message
  if (numBytes > 0 && !messageReceived) {
    bufferIndex = 0;
    memset(messageBuffer, 0, BUFFER_SIZE);
  }
  
  // Read incoming data
  while (Wire.available() && bufferIndex < BUFFER_SIZE - 1) {
    char c = Wire.read();
    messageBuffer[bufferIndex++] = c;
  }
  
  // Null terminate the string
  messageBuffer[bufferIndex] = '\0';
  messageReceived = true;
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("\nNodeMCU I2C Receiver");
  Serial.println("Waiting for messages from Arduino...");
  
  // Initialize I2C communications as slave
  Wire.begin(I2C_SLAVE_ADDRESS);
  
  // Register event handler for receiving I2C data
  Wire.onReceive(receiveEvent);
}

void loop() {
  // Process received message
  if (messageReceived) {
    Serial.println("Message received: " + String(messageBuffer));
    
    // Here you can add code to process or forward the message
    // For example, send to a web service, display on OLED screen, etc.
    
    // Reset message received flag
    messageReceived = false;
    
    // Optional: Print a separator for clarity in the serial output
    Serial.println("------------------------------");
  }
  
  // Small delay to prevent watchdog timer issues
  delay(10);
}
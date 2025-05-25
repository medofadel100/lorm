// NodeMCU I2C Receiver Code with Firebase Integration
// Receives text messages from Arduino via I2C and sends them to Firebase
// ESP8266 NodeMCU

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ArduinoJson.h>

// I2C pin definitions for NodeMCU
// D1 = GPIO 5 (SCL)
// D2 = GPIO 4 (SDA)

#define I2C_SLAVE_ADDRESS 8  // Same address as defined in Arduino code

// WiFi credentials - Replace with your network credentials
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Firebase credentials
#define FIREBASE_HOST "lorm-zhite-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "umUmIorWf0ZtCW9Jjf58QqjfmxHokBEOEIe15E2q"

// Define Firebase Data object
FirebaseData firebaseData;
FirebaseJson json;

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

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected to WiFi with IP: ");
  Serial.println(WiFi.localIP());
}

void sendMessageToFirebase(String message) {
  // Get current timestamp
  unsigned long timestamp = millis(); // Ideally this would be actual time from NTP, but millis() works for demo
  
  // Create json for the message
  json.clear();
  json.set("clientId", "2");
  json.set("sender", "Client 2");
  json.set("text", message);
  json.set("timestamp", timestamp);
  json.set("type", "rx");
  
  // Path to store message in Firebase
  String path = "/rx";
  
  // Push data to Firebase (this creates a new child with unique key)
  if (Firebase.pushJSON(firebaseData, path, json)) {
    Serial.println("Message sent to Firebase successfully!");
    Serial.println("Path: " + firebaseData.dataPath());
  } else {
    Serial.println("Failed to send message to Firebase");
    Serial.println("Error: " + firebaseData.errorReason());
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("\nNodeMCU I2C Receiver with Firebase");
  
  // Connect to WiFi
  connectToWiFi();
  
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  // Send initial connection message to Firebase
  sendMessageToFirebase("Client 2 Connected");
  
  // Initialize I2C communications as slave
  Wire.begin(I2C_SLAVE_ADDRESS);
  
  // Register event handler for receiving I2C data
  Wire.onReceive(receiveEvent);
  
  Serial.println("Waiting for messages from Arduino...");
}

void loop() {
  // Process received message
  if (messageReceived) {
    String message = String(messageBuffer);
    Serial.println("Message received: " + message);
    
    // Send message to Firebase
    sendMessageToFirebase(message);
    
    // Reset message received flag
    messageReceived = false;
    
    // Optional: Print a separator for clarity in the serial output
    Serial.println("------------------------------");
  }
  
  // Small delay to prevent watchdog timer issues
  delay(10);
}
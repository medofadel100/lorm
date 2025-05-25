// Arduino code for 11 buttons with I2C communication to NodeMCU
// 10 buttons for letters (each with single/double click for different letters)
// 1 dedicated button (pin 12) for space (single click) and send message (double click)
// Sends formed messages to NodeMCU via I2C, which forwards to Firebase

#include <Wire.h>

#define I2C_SLAVE_ADDRESS 8  // I2C address for NodeMCU

// Button pin definitions (connect buttons between these pins and GND)
const int buttonPins[11] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// Letters assigned to each button (first for single click, second for double click)
// Using all 20 most common letters in English
const char buttonLetters[10][2] = {
  {'e', 't'}, // Button 0: e (single click), t (double click)
  {'a', 'o'}, // Button 1: a (single click), o (double click)
  {'i', 'n'}, // Button 2: i (single click), n (double click)
  {'s', 'r'}, // Button 3: s (single click), r (double click)
  {'h', 'l'}, // Button 4: h (single click), l (double click)
  {'d', 'c'}, // Button 5: d (single click), c (double click)
  {'u', 'm'}, // Button 6: u (single click), m (double click)
  {'f', 'w'}, // Button 7: f (single click), w (double click)
  {'g', 'y'}, // Button 8: g (single click), y (double click)
  {'p', 'b'}  // Button 9: p (single click), b (double click)
};

// Variables to keep track of button states
bool buttonStates[11] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool lastButtonStates[11] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const unsigned long debounceDelay = 50;  // Debounce time in ms

// Variables for detecting double clicks
unsigned long lastClickTime[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const unsigned long doubleClickDelay = 300;  // Max time between clicks to be considered a double click
bool waitingForDoubleClick[11] = {false, false, false, false, false, false, false, false, false, false, false};

// Variable to store the current word
String currentWord = "";

// Button to clear the word (button 1 + button 9 pressed together)
bool clearCombo = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize I2C communications as master
  Wire.begin();
  
  // Set up button pins as inputs with pull-up resistors
  for (int i = 0; i < 11; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  Serial.println("LORM Glove Text Entry System");
  Serial.println("Buttons 0-9: Single click for first letter, Double click for second letter");
  Serial.println("Button 10 (pin 12): Space (single click), Send message (double click)");
  Serial.println("Buttons 1+9: Clear message");
  Serial.println("Current message: ");
}

// Function to send message to NodeMCU via I2C
void sendMessageToNodeMCU(String message) {
  if (message.length() == 0) {
    // Don't send empty messages
    Serial.println("Message is empty, not sending.");
    return;
  }
  
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  
  // Convert String to char array for transmission
  char charArray[message.length() + 1];
  message.toCharArray(charArray, message.length() + 1);
  
  // Send message character by character
  for (int i = 0; i < message.length(); i++) {
    Wire.write(charArray[i]);
  }
  
  // End transmission
  Wire.endTransmission();
  
  Serial.println("Message sent to NodeMCU via I2C: " + message);
}

void loop() {
  // Check if buttons 1 and 9 are pressed together to clear the word
  if (digitalRead(buttonPins[1]) == LOW && digitalRead(buttonPins[9]) == LOW) {
    if (!clearCombo) {
      clearCombo = true;
      currentWord = "";
      Serial.println("\nMessage cleared");
      Serial.print("Current message: ");
      delay(300); // Delay to avoid multiple clears
    }
  } else {
    clearCombo = false;
  }
  
  // Check each button
  for (int i = 0; i < 11; i++) {
    // Read the button state
    bool reading = digitalRead(buttonPins[i]);
    
    // If the button state changed, due to noise or pressing
    if (reading != lastButtonStates[i]) {
      // Reset the debouncing timer
      lastDebounceTime[i] = millis();
    }
    
    // If the button state has been stable for longer than the debounce delay
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      // If the button state has changed
      if (reading != buttonStates[i]) {
        buttonStates[i] = reading;
        
        // Button pressed (LOW because of pull-up resistor)
        if (buttonStates[i] == LOW) {
          // Check if this could be a double click
          if (waitingForDoubleClick[i]) {
            // It's a double click
            waitingForDoubleClick[i] = false;
            
            // Special handling for button 10 (pin 12) double click (send message)
            if (i == 10) {
              Serial.println("\nMessage completed: " + currentWord);
              
              // Send message to NodeMCU via I2C
              sendMessageToNodeMCU(currentWord);
              
              Serial.println("Starting new message");
              Serial.print("Current message: ");
              currentWord = "";
            } else {
              // For buttons 0-9, add the double click letter
              currentWord += buttonLetters[i][1];
              Serial.print(buttonLetters[i][1]);
            }
          } else {
            // Start waiting for a possible double click
            waitingForDoubleClick[i] = true;
            lastClickTime[i] = millis();
          }
        }
      }
    }
    
    // Check if we need to register a single click (timeout on double click wait)
    if (waitingForDoubleClick[i] && (millis() - lastClickTime[i] > doubleClickDelay)) {
      waitingForDoubleClick[i] = false;
      
      // Special handling for button 10 (pin 12) single click (space)
      if (i == 10) {
        currentWord += " ";
        Serial.print(" ");
      } else {
        // For buttons 0-9, add the single click letter
        currentWord += buttonLetters[i][0];
        Serial.print(buttonLetters[i][0]);
      }
    }
    
    // Save the button state for comparison next time
    lastButtonStates[i] = reading;
  }
}
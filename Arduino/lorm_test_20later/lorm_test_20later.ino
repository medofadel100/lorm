// Arduino code for 10 buttons with dual letter functionality
// Each button sends one letter on single click, another on double click
// Displays formed words on Serial monitor

// Button pin definitions (connect buttons between these pins and GND)
const int buttonPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// Letters assigned to each button (first for single click, second for double click)
// Using the 20 most common letters in English
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
bool buttonStates[10] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool lastButtonStates[10] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const unsigned long debounceDelay = 50;  // Debounce time in ms

// Variables for detecting double clicks
unsigned long lastClickTime[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const unsigned long doubleClickDelay = 300;  // Max time between clicks to be considered a double click
bool waitingForDoubleClick[10] = {false, false, false, false, false, false, false, false, false, false};

// Variable to store the current word
String currentWord = "";

// Button to clear the word (button 0 + button 9 pressed together)
bool clearCombo = false;

void setup() {
  Serial.begin(9600);
  
  // Set up button pins as inputs with pull-up resistors
  for (int i = 0; i < 10; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  Serial.println("Arduino Text Entry System");
  Serial.println("Single click: First letter, Double click: Second letter");
  Serial.println("Current word: ");
}

void loop() {
  // Check if buttons 0 and 9 are pressed together to clear the word
  if (digitalRead(buttonPins[0]) == LOW && digitalRead(buttonPins[9]) == LOW) {
    if (!clearCombo) {
      clearCombo = true;
      currentWord = "";
      Serial.println("\nWord cleared");
      Serial.print("Current word: ");
      delay(300); // Delay to avoid multiple clears
    }
  } else {
    clearCombo = false;
  }
  
  // Check each button
  for (int i = 0; i < 10; i++) {
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
            currentWord += buttonLetters[i][1];  // Add the double click letter
            Serial.print(buttonLetters[i][1]);   // Print the letter to Serial
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
      currentWord += buttonLetters[i][0];  // Add the single click letter
      Serial.print(buttonLetters[i][0]);   // Print the letter to Serial
    }
    
    // Save the button state for comparison next time
    lastButtonStates[i] = reading;
  }
  
  // Check for space (button 5 + button 6 pressed together)
  if (digitalRead(buttonPins[5]) == LOW && digitalRead(buttonPins[6]) == LOW) {
    currentWord += " ";
    Serial.print(" ");
    delay(300); // Delay to avoid multiple spaces
  }
  
  // Check for Enter (print the completed word) - button 3 + button 7 pressed together
  if (digitalRead(buttonPins[3]) == LOW && digitalRead(buttonPins[7]) == LOW) {
    Serial.println("\nWord completed: " + currentWord);
    Serial.println("Starting new word");
    Serial.print("Current word: ");
    currentWord = "";
    delay(300); // Delay to avoid multiple enters
  }
}
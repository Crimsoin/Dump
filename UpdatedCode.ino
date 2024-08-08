#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Define LCD settings
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Define Keypad settings
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {A3, A2, A1}; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define Ultrasonic Sensor pins
const int trigPin = 9;
const int echoPin = 8;

// Variables
unsigned long lapStartTime;
int totalLaps = 0;
int lapsCompleted = 0;
bool timerRunning = false;

// Function to measure distance
long readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration / 2) / 29.1;
  return distance;
}

// Function to display time in HH:MM:SS format
String formatTime(unsigned long timeInMillis) {
  unsigned long secs = timeInMillis / 1000;
  unsigned long mins = secs / 60;
  unsigned long hours = mins / 60;
  secs = secs % 60;
  mins = mins % 60;
  char timeString[9];
  sprintf(timeString, "%02lu:%02lu:%02lu", hours, mins, secs);
  return String(timeString);
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter No. of Laps:");
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (!timerRunning) {
    // Read keypad input for number of laps
    char key = keypad.getKey();
    if (key != NO_KEY) {
      if (key >= '0' && key <= '9') {
        totalLaps = totalLaps * 10 + (key - '0');
        lcd.setCursor(0, 1);
        lcd.print(totalLaps);
      } else if (key == '*') {
        timerRunning = true;
        lapStartTime = millis();
        lapsCompleted = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Laps: 0 / ");
        lcd.print(totalLaps);
        lcd.setCursor(0, 1);
        lcd.print("Lap Time: 00:00:00");
      }
    }
  } else {
    // Measure distance
    long distance = readUltrasonic();
    if (distance < 10) {
      unsigned long lapDuration = millis() - lapStartTime;
      lapStartTime = millis();
      lapsCompleted++;
      lcd.setCursor(0, 2);
      lcd.print("Lap ");
      lcd.print(lapsCompleted);
      lcd.print(" Time: ");
      lcd.print(formatTime(lapDuration));
      delay(1000); // Debounce
    }

    // Update display
    lcd.setCursor(6, 0);
    lcd.print(lapsCompleted);
    lcd.setCursor(12, 0);
    lcd.print(totalLaps);

    unsigned long currentLapDuration = millis() - lapStartTime;
    lcd.setCursor(10, 1);
    lcd.print(formatTime(currentLapDuration));

    // Check if laps completed
    if (lapsCompleted >= totalLaps) {
      timerRunning = false;
      lcd.setCursor(0, 3);
      lcd.print("All Laps Completed!");
    }
  }
}

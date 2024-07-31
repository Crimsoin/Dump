#include <NewPing.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 20  // Maximum distance we want to measure (in cm)

// Initialize the NewPing library
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// Variables for timing and lap counting
unsigned long startTime;
unsigned long currentTime;
unsigned long lapTime;
int lapCounter = 0;
int totalLaps = 0;
bool isRunning = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter the number of laps:");
}

void loop() {
  // Check if the user has entered the number of laps
  if (Serial.available() > 0 && !isRunning) {
    totalLaps = Serial.parseInt();
    if (totalLaps > 0) {
      Serial.print("Number of laps set to: ");
      Serial.println(totalLaps);
      isRunning = true;
      lapCounter = 0;
    }
  }

  if (isRunning) {
    // Measure distance
    unsigned int distance = sonar.ping_cm();
    
    // If an object is detected within the sensor's range
    if (distance > 0 && distance <= MAX_DISTANCE) {
      if (lapCounter == 0) {
        startTime = millis();
      } else {
        currentTime = millis();
        lapTime = currentTime - startTime;

        // Convert lap time to hours, minutes, and seconds
        unsigned long seconds = lapTime / 1000;
        unsigned long minutes = seconds / 60;
        unsigned long hours = minutes / 60;
        seconds %= 60;
        minutes %= 60;

        // Print the lap time in Hours:Minutes:Seconds format
        Serial.print("Lap ");
        Serial.print(lapCounter);
        Serial.print(": ");
        Serial.print(hours);
        Serial.print(":");
        if (minutes < 10) Serial.print("0");
        Serial.print(minutes);
        Serial.print(":");
        if (seconds < 10) Serial.print("0");
        Serial.print(seconds);
        Serial.println(" (H:M:S)");
        
        startTime = currentTime;
      }
      
      lapCounter++;
      
      // Check if the required number of laps is completed
      if (lapCounter >= totalLaps) {
        Serial.println("All laps completed.");
        isRunning = false;
      }
      
      // Small delay to avoid multiple detections for the same lap
      delay(1000);
    }
  }
}

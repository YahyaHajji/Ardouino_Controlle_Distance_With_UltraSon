// Include L298N Library
#include <L298N.h>

// Pin definitions for motors
const int ENA = 9; // Motor A speed control
const int IN1 = 8; // Motor A direction control
const int IN2 = 7;
const int ENB = 3; // Motor B speed control
const int IN3 = 5; // Motor B direction control
const int IN4 = 4;

// Pin definitions for HC-SR04
const int TRIG_PIN = 10; // Trigger pin
const int ECHO_PIN = 11; // Echo pin

// Distance thresholds in centimeters
const int MAX_DISTANCE = 100; // Maximum distance for speed scaling
const int STOP_DISTANCE = 2; // Distance to stop the motors

// Timeout for pulseIn (in microseconds)
const unsigned long TIMEOUT = 30000;

// Create motor instances
L298N motorA(ENA, IN1, IN2);
L298N motorB(ENB, IN3, IN4);

void setup() {
  // Initialize motors
  motorA.setSpeed(255);
  motorB.setSpeed(255);

  // Initialize HC-SR04 pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Start Serial Monitor for debugging
  Serial.begin(9600);
}

void loop() {
  // Measure distance
  int distance = getDistance();

  // Debugging: Print the distance
  if (distance >= 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Distance: Out of range or no echo detected!");
  }

  // Control motors based on distance
  if (distance > 0 && distance <= STOP_DISTANCE) {
    // Stop motors
    Serial.println("Object too close! Stopping motors.");
    motorA.stop();
    motorB.stop();
  } else if (distance > STOP_DISTANCE && distance <= MAX_DISTANCE) {
    // Scale motor speed based on distance
   // int speed = map(distance, STOP_DISTANCE, MAX_DISTANCE, 0, 255);
    int speed = ((distance - STOP_DISTANCE) * 255) / (MAX_DISTANCE - STOP_DISTANCE);

    Serial.print("Setting motor speed: ");
    Serial.println(speed);

    motorA.setSpeed(speed);
            motorB.setSpeed(speed);

    // Move motors forward
    motorA.forward();
    motorB.forward();
  } else {
    // Full speed if object is far away
    Serial.println("Object far away, running at full speed.");
    motorA.setSpeed(255);
    motorB.setSpeed(255);

    motorA.forward();
    motorB.forward();
  }

  delay(100); // Small delay to reduce sensor noise
}

// Function to measure distance using HC-SR04
int getDistance() {
  // Send a 10-microsecond pulse to the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the duration of the echo pulse
  long duration = pulseIn(ECHO_PIN, HIGH, TIMEOUT);

  // Check if timeout occurred
  if (duration == 0) {
    return -1; // Return -1 if no echo detected
  }

  // Calculate distance in centimeters
  int distance = duration * 0.034 / 2; // Speed of sound: 343 m/s or 0.034 cm/us

  return distance; // Return the measured distance
}
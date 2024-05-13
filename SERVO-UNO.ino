#include <Servo.h>

#define RELAY_PIN 2        // Pin number for the relay
#define SERVO_PIN A0       // Pin number for the servo motor
#define WATER_PUMP_PIN 7   // Pin number for the water pump control

Servo myservo;            // Servo object to control the servo motor
unsigned long previousMillis = 0; // Variable to store previous time for servo motor
const unsigned long servoInterval = 10; // Interval to run the servo motor (in milliseconds)
const unsigned long servoOffTime = 10;  // Time to turn off the servo motor (in milliseconds)
bool motorRunning = false; // Flag to indicate if motor is currently running

void setup() {
  pinMode(RELAY_PIN, OUTPUT);   // Set relay pin as output
  digitalWrite(RELAY_PIN, LOW); // Turn off the relay pin initially
  pinMode(WATER_PUMP_PIN, INPUT_PULLUP); // Set water pump control pin as input with internal pull-up resistor
  myservo.attach(SERVO_PIN);    // Attach servo motor to the respective pin
  Serial.begin(9600);          // Start serial communication
}

void loop() {
  unsigned long currentMillis = millis();  // Get current time
  int waterPumpStatus = digitalRead(WATER_PUMP_PIN); // Read water pump control pin
   Serial.println(waterPumpStatus);
  // Run the servo motor for 5 seconds if water pump control pin is HIGH
  if (waterPumpStatus == HIGH) {
    Serial.println("servo on");
    if (currentMillis - previousMillis >= servoInterval) {
      previousMillis = currentMillis;
      for (int pos = 0; pos <= 180; pos += 1) {
        myservo.write(pos);
        delay(15);
      }
      // Move servo motor to 90 degrees
      delay(servoOffTime); // Keep the servo motor on for 2 seconds
      for (int pos = 180; pos >= 0; pos -= 1) {
        myservo.write(pos);
        delay(15);
      }
      // Move servo motor to 0 degrees
    }
if (!motorRunning && waterPumpStatus != 0 && (currentMillis - previousMillis >= 10000)) {
    previousMillis = currentMillis;
    digitalWrite(RELAY_PIN, HIGH); // Turn on the relay pin to run the motor
    Serial.println("motor on");
    motorRunning = true; // Set motorRunning flag to true
  }
  }

  // Turn on the relay pin if water pump control pin is HIGH and motor is not running
  

  // Turn off the relay pin if water pump control pin is LOW
  if (waterPumpStatus == LOW) {
    digitalWrite(RELAY_PIN, LOW); // Turn off the relay pin to stop the motor
    myservo.write(0); // Set servo motor to 0 degrees
    Serial.println("motor off");
    motorRunning = false; // Set motorRunning flag to false
  }
}
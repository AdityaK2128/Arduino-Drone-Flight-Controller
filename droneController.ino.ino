#include <Servo.h>
#include <PinChangeInterrupt.h>

// Motor ESC connections
Servo escFrontLeft;
Servo escFrontRight;
Servo escRearLeft;
Servo escRearRight;

// Define the pins for the receiver channels
const int yawPin = 8;      // Yaw channel (Channel 1)
const int pitchPin = 9;    // Pitch channel (Channel 2)
const int throttlePin = 10; // Throttle channel (Channel 3)
const int rollPin = 11;    // Roll channel (Channel 4)

// Motor pins
const int motorFRPin = 4; // Front Right Motor
const int motorFLPin = 7; // Front Left Motor
const int motorRRPin = 5; // Rear Right Motor
const int motorRLPin = 6; // Rear Left Motor

// Variables to store channel input values
volatile int throttleValue = 1500;
volatile int rollValue = 1500;
volatile int pitchValue = 1500;
volatile int yawValue = 1500;

// Variables to store motor speed values
int motorFLValue = 1000;
int motorFRValue = 1000;
int motorRLValue = 1000;
int motorRRValue = 1000;

// Minimum throttle value to keep motors spinning
const int minimumThrottle = 1100;  // Adjust this value as necessary to keep motors spinning

// Interrupt service routines for each channel
void readThrottle() {
  static unsigned long lastRise = 0;
  unsigned long time = micros();
  if (digitalRead(throttlePin) == HIGH) {
    lastRise = time;
  } else {
    throttleValue = time - lastRise;
  }
}

void readRoll() {
  static unsigned long lastRise = 0;
  unsigned long time = micros();
  if (digitalRead(rollPin) == HIGH) {
    lastRise = time;
  } else {
    rollValue = time - lastRise;
  }
}

void readPitch() {
  static unsigned long lastRise = 0;
  unsigned long time = micros();
  if (digitalRead(pitchPin) == HIGH) {
    lastRise = time;
  } else {
    pitchValue = time - lastRise;
  }
}

void readYaw() {
  static unsigned long lastRise = 0;
  unsigned long time = micros();
  if (digitalRead(yawPin) == HIGH) {
    lastRise = time;
  } else {
    yawValue = time - lastRise;
  }
}

void setup() {
  // Initialize serial communication at 9600 bps for debugging
  Serial.begin(9600);

  // Initialize receiver channel pins as inputs
  pinMode(throttlePin, INPUT);
  pinMode(rollPin, INPUT);
  pinMode(pitchPin, INPUT);
  pinMode(yawPin, INPUT);

  // Attach pin change interrupts to the receiver channels
  attachPCINT(digitalPinToPCINT(throttlePin), readThrottle, CHANGE);
  attachPCINT(digitalPinToPCINT(rollPin), readRoll, CHANGE);
  attachPCINT(digitalPinToPCINT(pitchPin), readPitch, CHANGE);
  attachPCINT(digitalPinToPCINT(yawPin), readYaw, CHANGE);

  // Attach ESCs to motor pins
  escFrontLeft.attach(motorFLPin);
  escFrontRight.attach(motorFRPin);
  escRearLeft.attach(motorRLPin);
  escRearRight.attach(motorRRPin);

  // Initialize motors to minimum speed
  escFrontLeft.writeMicroseconds(1000);
  escFrontRight.writeMicroseconds(1000);
  escRearLeft.writeMicroseconds(1000);
  escRearRight.writeMicroseconds(1000);
}

void loop() {
  // Read and normalize the input values
  int normalizedThrottle = map(throttleValue, 1000, 2000, 0, 1000); // Throttle range 0 to 1000 (instead of 0 to 2000)
  int normalizedRoll = map(rollValue, 1000, 2000, 500, -500);  // Inverted Roll
  int normalizedPitch = map(pitchValue, 1000, 2000, -500, 500); // Normal Pitch
  int normalizedYaw = map(yawValue, 1000, 2000, 500, -500);  // Inverted Yaw

  // Check if throttle is above the minimum threshold
  if (throttleValue > 1000) {
    // Calculate motor speeds based on normalized values with flipped Roll and Yaw
    motorFLValue = constrain(minimumThrottle + normalizedThrottle - normalizedPitch - normalizedRoll - normalizedYaw, minimumThrottle, 2000);
    motorFRValue = constrain(minimumThrottle + normalizedThrottle - normalizedPitch + normalizedRoll + normalizedYaw, minimumThrottle, 2000);
    motorRLValue = constrain(minimumThrottle + normalizedThrottle + normalizedPitch - normalizedRoll + normalizedYaw, minimumThrottle, 2000);
    motorRRValue = constrain(minimumThrottle + normalizedThrottle + normalizedPitch + normalizedRoll - normalizedYaw, minimumThrottle, 2000);
  } else {
    // Set motors to idle speed if throttle is below or equal to 1000
    motorFLValue = 1000;
    motorFRValue = 1000;
    motorRLValue = 1000;
    motorRRValue = 1000;
  }

  // Send calculated speeds to ESCs
  escFrontLeft.writeMicroseconds(motorFLValue);
  escFrontRight.writeMicroseconds(motorFRValue);
  escRearLeft.writeMicroseconds(motorRLValue);
  escRearRight.writeMicroseconds(motorRRValue);

  // Print the values to the serial monitor for debugging
  Serial.print("Throttle: ");
  Serial.print(throttleValue);
  Serial.print("\tRoll: ");
  Serial.print(rollValue);
  Serial.print("\tPitch: ");
  Serial.print(pitchValue);
  Serial.print("\tYaw: ");
  Serial.println(yawValue);

  // Delay for 10 milliseconds to stabilize the loop
  delay(10);
}

// ---------------------------------------------------------- //
// Libraries
// ---------------------------------------------------------- //

#include <Servo.h> // Servo control
#include <NewPing.h> // Sonor reading

// ---------------------------------------------------------- //
// Pin mapping
// ---------------------------------------------------------- //

// Sonar
#define SONAR_VCC 11
#define SONAR_TRIG 13
#define SONAR_ECHO 12
#define SONAR_GND Ground // For documentation

// Servo
#define SERVO_PWM 10
#define SERVO_5V 5V // For documentation
#define SERVO_GND Ground // For documentation

// ---------------------------------------------------------- //
// Important definitions
// ---------------------------------------------------------- //

// General
#define ONE_SECOND 1000

// Sonar
#define MAX_DISTANCE 200 // Maximum distance set to 200 cm
#define DISTANCES_SIZE 10 // Number of distances to measure to
                          // prevent outliers
#define UPPER_HEIGHT 30 // Initialization height
#define GRAB_DELAY 4000 // Time to wait after grabbing
                        // an object before attempting
                        // to release the object
#define RELEASE_DELAY 3000 // Time to wait after releasing
                           //an object before attempting
                           // to grab another object
#define GRAB_THRESHOLD 15 // Distance to close the claw
#define RELEASE_THRESHOLD 17 // Distance to open the claw
#define RETRY_THRESHOLD 20 // Distance to retry grabbing
// Initialize NewPing
NewPing sonar(SONAR_TRIG, SONAR_ECHO, MAX_DISTANCE);

// Servo
Servo myservo; // Create servo object to control a servo
#define SERVO_OPEN 180
#define SERVO_CLOSE 0

// States
#define CALIBRATE 0
#define GROUNDED 1
#define SENSE 2
#define GRABBED 3
#define RELEASE 4

// Variables
int state = CALIBRATE;
unsigned long target_time = 0;
int distances[DISTANCES_SIZE] = {0};
int calibrateCount = 0;

// ---------------------------------------------------------- //
// Function prototypes
// ---------------------------------------------------------- //

void printArray (int* data, int arraySize);
int push (int value);

// ---------------------------------------------------------- //
// Initialization of the program
// ---------------------------------------------------------- //

void setup()
{
    // Set data transmission rate to communicate with computer
    Serial.begin(9600);
    Serial.println("Initializing device...");

    // Sonar pins
    pinMode(SONAR_TRIG, OUTPUT);
    pinMode(SONAR_ECHO, INPUT);

    // Servo
    myservo.attach(SERVO_PWM); // Attaches servo to servo object
    myservo.write(SERVO_OPEN); // Default position
    pinMode(SONAR_VCC, OUTPUT);     // Tell pin to be output
    digitalWrite(SONAR_VCC, HIGH);  // Set pin to 5V

    Serial.println("Claw Retrival System setup complete");
}

// ---------------------------------------------------------- //
// Superloop
// ---------------------------------------------------------- //

void loop()
{
    int distance = push(sonar.ping_cm()); // Read from sonar

    switch (state) {
        case CALIBRATE:
            calibrateCount++;
            if (calibrateCount >= DISTANCES_SIZE) {
                Serial.println("Calibration complete");
                if (distance >= UPPER_HEIGHT) {
                    state = SENSE; 
                } else {
                    state = GROUNDED;
                }
            }
            break;
        case GROUNDED:
            if (distance < 30) {
                myservo.write(SERVO_CLOSE);
            }
            else {
                myservo.write(SERVO_OPEN);
            }
            break;
    }
}

// ---------------------------------------------------------- //
// Push into distances array
// ---------------------------------------------------------- //

int push (int value) {
    int average = value;
    for (int index = 1; index < DISTANCES_SIZE; index ++) {
        average += distances[index];
        distances[index - 1] = distances[index];
    }
    distances[DISTANCES_SIZE - 1] = value;
    return average / DISTANCES_SIZE;
}

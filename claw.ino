/*
* Author: Elio Di Nino
* Course: APSC 101
* Group: N-9
* Some code was used from APSC 101 as well as Haxrox on GitHub
*/

/*
* Purpose:  To controls a physcial claw via servo motor based on
*           sonar input. This allows for autonomous opening and
*           closing of the device when picking objects up.
*/

/*
* Specific Implementation details:
* - Starts with a short calibration phase so that the distance
*   averaging array is filled
* - Our program then waits until the device is at a set height
* - Following this, we wait until the height is under the
*   threshold for picking up itemss
* - We run a check to see if the claw has remained in position
*   after grabbing indicating that we want the claw to try
*   grabbing the item again
* - The object is released based on another height threshold
*   only after it goes higher than the upper height requirement
* - This whole process repeats automatically
*/

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
#define SONAR_TRIG 12
#define SONAR_ECHO 13
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
#define DISTANCES_SIZE 5 // Number of distances to measure to
                         // prevent outliers
#define UPPER_HEIGHT 30 // Initialization height
#define GRAB_DELAY 2000 // Time to wait after grabbing
                        // an object before attempting
                        // to release the object
#define RELEASE_DELAY 3000 // Time to wait after releasing
                           //an object before attempting
                           // to grab another object
#define GRAB_THRESHOLD 14 // Distance to close the claw - 14 for Sr. | 16 for Jr.
#define RELEASE_THRESHOLD 16 // Distance to open the claw - 16 for Sr. | 18 for Jr.
#define RETRY_THRESHOLD 18 // Distance to retry grabbing - 18 for Sr. | 20 for Jr.
// Initialize NewPing
NewPing sonar(SONAR_TRIG, SONAR_ECHO, MAX_DISTANCE);

// Servo
Servo myservo; // Create servo object to control a servo
#define SERVO_OPEN 10 // 10 for Sr. | 0 for Jr.
#define SERVO_CLOSE 180

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
    unsigned long current_time = millis(); // Start time

    /*Serial.print("Ping: ");
    Serial.print(distance);
    Serial.println("cm");*/

    if (current_time >= target_time) {
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
                if (distance >= UPPER_HEIGHT) {
                    Serial.println("Sensing ...");
                    state = SENSE;
                }
                break;
            case SENSE:
                if (distance <= GRAB_THRESHOLD) {
                    Serial.print("Grab delay active ... ");
                    state = GRABBED;
                    delay(2 * ONE_SECOND);
                    Serial.println("Grabbing object");
                    myservo.write(SERVO_CLOSE);
                    target_time = current_time + 2 * ONE_SECOND + GRAB_DELAY;
                }
                break;
            case GRABBED: 
                if (distance >= UPPER_HEIGHT) {
                    Serial.println("Object lifted");
                    state = RELEASE;
                }
                else if (distance <= RETRY_THRESHOLD) {
                    Serial.println("Retrying");
                    state = SENSE;
                    myservo.write(SERVO_OPEN);
                }
                break;
            case RELEASE: 
                if (distance <= RELEASE_THRESHOLD) {
                    Serial.println("Releasing object");
                    state = GROUNDED;
                    myservo.write(SERVO_OPEN);
                    target_time = current_time + RELEASE_DELAY;
                }
                break;
        }
    }
}

// ---------------------------------------------------------- //
// Print the distance array (for averaging)
// ---------------------------------------------------------- //

void printArray (int* data, int arraySize) {
    Serial.print("[");
    for (int index = 0; index < arraySize - 1; index ++) {
        Serial.print(data[index]);
        Serial.print(", ");
    }
    Serial.print(data[arraySize-1]);
    Serial.println("]");
}

// ---------------------------------------------------------- //
// Push into distances array
// ---------------------------------------------------------- //

int push (int value) {
    if (value == 0) {
        value = 200;
    }
    int average = value;
    
    for (int index = 1; index < DISTANCES_SIZE; index ++) {
        average += distances[index];
        distances[index - 1] = distances[index];
    }
    distances[DISTANCES_SIZE - 1] = value;
    //printArray(distances, DISTANCES_SIZE);
    return average / DISTANCES_SIZE;
}

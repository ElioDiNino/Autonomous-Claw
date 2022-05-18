#include <Servo.h> // Servo control
#include <NewPing.h> // Sonor reading

// Sonar
#define SONAR_VCC 11
#define SONAR_TRIG 13
#define SONAR_ECHO 12
#define SONAR_GND Ground // For documentation

// Servo
#define SERVO_PWM 10
#define SERVO_5V 5V // For documentation
#define SERVO_GND Ground // For documentation

// Initialize NewPing
NewPing sonar(SONAR_TRIG, SONAR_ECHO, MAX_DISTANCE);

// Servo
Servo myservo; // Create servo object to control a servo
#define SERVO_OPEN 180
#define SERVO_CLOSE 0

void setup()
{
    // Set data transmission rate to communicate with computer
    Serial.begin(9600);
    // Sonar pins
    pinMode(SONAR_TRIG, OUTPUT);
    pinMode(SONAR_ECHO, INPUT);

    // Servo
    myservo.attach(SERVO_PWM); // Attaches servo to servo object
    myservo.write(SERVO_OPEN); // Default position
    pinMode(SONAR_VCC, OUTPUT);     // Tell pin to be output
    digitalWrite(SONAR_VCC, HIGH);  // Set pin to 5V
}

void loop()
{
    int distance = sonar.ping_cm(); // Read from sonar

    if (distance < 30) {
        myservo.write(SERVO_CLOSE);
    }
    else {
        myservo.write(SERVO_OPEN);
    }
}

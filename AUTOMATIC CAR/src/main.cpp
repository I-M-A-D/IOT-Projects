#include <Arduino.h>
#include <Servo.h>



constexpr uint8_t TRIG_PIN = 2;
constexpr uint8_t ECHO_PIN = 3;
constexpr uint8_t SERVO_PIN = 10;

constexpr uint8_t ENA = 5;
constexpr uint8_t IN1 = 7;
constexpr uint8_t IN2 = 8;

constexpr uint8_t ENB = 6;
constexpr uint8_t IN3 = 12;
constexpr uint8_t IN4 = 13;

constexpr int SAFE_DISTANCE_CM = 25;
constexpr int FORWARD_SPEED = 140;
constexpr int TURN_SPEED = 145;

constexpr int CENTER_ANGLE = 90;
constexpr int LEFT_ANGLE = 35;
constexpr int RIGHT_ANGLE = 145;

constexpr unsigned long SERVO_DELAY_MS = 300;
constexpr unsigned long TURN_TIME_MS = 350;
constexpr unsigned long ECHO_TIMEOUT_US = 25000UL;

Servo scanner;

void setupPins();

void stopMotors();

void moveForward();

void turnLeft();

void turnRight();

void driveMotors(int leftSpeed, int rightSpeed);

void controlMotor(int speed, uint8_t enablePin, uint8_t input1, uint8_t input2);

int measureDistance();

int getFilteredDistance();

int scanDirection(int angle);



void setup()
{
    Serial.begin(9600);
    setupPins();
    scanner.attach(SERVO_PIN);
    scanner.write(CENTER_ANGLE);
    stopMotors();
    delay(1000);
    Serial.println();
    Serial.println(" AUTONOMOUS ROBOT STARTED");
    Serial.println("System Initialization Complete.");
    delay(500);
}



void loop()
{
    scanner.write(CENTER_ANGLE);
    delay(120);
    int frontDistance = getFilteredDistance();
    Serial.print("Front Distance: ");
    Serial.print(frontDistance);
    Serial.println(" cm");
    
    if (frontDistance > SAFE_DISTANCE_CM)
    {
        Serial.println("Path Clear - Moving Forward");
        moveForward();
        delay(60);
        return;
    }

    Serial.println("Obstacle Detected!");
    stopMotors();
    delay(150);

    int leftDistance = scanDirection(LEFT_ANGLE);
    int rightDistance = scanDirection(RIGHT_ANGLE);

    Serial.print("Left Distance: ");
    Serial.print(leftDistance);
    Serial.println(" cm");

    Serial.print("Right Distance: ");
    Serial.print(rightDistance);
    Serial.println(" cm");

    scanner.write(CENTER_ANGLE);

    if (leftDistance > SAFE_DISTANCE_CM || rightDistance > SAFE_DISTANCE_CM)
    {
        if (leftDistance > rightDistance)
        {
            Serial.println("Decision: Turning LEFT");
            turnLeft();
        }
        else
        {
            Serial.println("Decision: Turning RIGHT");
            turnRight();
        }
    }

    else
    {
        Serial.println("No Safe Direction Available.");
        Serial.println("Robot Stopped.");
        stopMotors();
        delay(300);
    }
}



void setupPins()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    digitalWrite(TRIG_PIN, LOW);

    stopMotors();
}



int measureDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(3);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

    if (duration == 0)
    {
        return -1;
    }

    int distance = duration / 58UL;

    return distance;
}



int getFilteredDistance()
{
    int readings[3];

    for (int i = 0; i < 3; i++)
    {
        readings[i] = measureDistance();
        delay(20);
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = i + 1; j < 3; j++)
        {
            if (readings[j] < readings[i])
            {
                int temp = readings[i];
                readings[i] = readings[j];
                readings[j] = temp;
            }
        }
    }

    return readings[1];
}



int scanDirection(int angle)
{
    scanner.write(angle);
    delay(SERVO_DELAY_MS);
    int distance = getFilteredDistance();
    return distance;
}



void controlMotor(int speed, uint8_t enablePin, uint8_t input1, uint8_t input2)
{
    int pwm = speed >= 0 ? speed : -speed;
    pwm = constrain(pwm, 0, 255);

    if (speed > 0)
    {
        digitalWrite(input1, HIGH);
        digitalWrite(input2, LOW);
    }

    else if (speed < 0)
    {
        digitalWrite(input1, LOW);
        digitalWrite(input2, HIGH);
    }

    else
    {
        digitalWrite(input1, LOW);
        digitalWrite(input2, LOW);
    }

    analogWrite(enablePin, pwm);
}



void driveMotors(int leftSpeed, int rightSpeed)
{
    controlMotor(leftSpeed, ENA, IN1, IN2);
    controlMotor(rightSpeed, ENB, IN3, IN4);
}



void moveForward()
{
    driveMotors(FORWARD_SPEED, FORWARD_SPEED);
}



void turnLeft()
{
    driveMotors(-TURN_SPEED, TURN_SPEED);
    delay(TURN_TIME_MS);
    stopMotors();
    delay(120);
}



void turnRight()
{
    driveMotors(TURN_SPEED, -TURN_SPEED);
    delay(TURN_TIME_MS);
    stopMotors();
    delay(120);
}



void stopMotors()
{
    driveMotors(0, 0);
}
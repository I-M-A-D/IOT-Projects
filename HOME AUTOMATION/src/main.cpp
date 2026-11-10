#include <Arduino.h>

#include <WiFi.h>

#include <WebServer.h>

                         // ==============
#include "dashboard.h"   // WEB DASHBOARD
                         // ==============

#include <ESP32Servo.h>

#include <DHT.h>

#include <OneWire.h>

#include <DallasTemperature.h>

#include <math.h>



const char* AP_SSID = "SmartHome-Demo";
const char* AP_PASSWORD = "ChangeMe123";

WebServer server(80);

constexpr uint8_t PIN_DHT = 16;
constexpr uint8_t PIN_SOIL = 34;
constexpr uint8_t PIN_LDR = 35;
constexpr uint8_t PIN_PIR = 27;
constexpr uint8_t PIN_RAIN = 33;
constexpr uint8_t PIN_DOOR_REED = 25;
constexpr uint8_t PIN_WINDOW_REED = 26;
constexpr uint8_t PIN_FRIDGE = 4;
constexpr uint8_t PIN_GATE_SERVO = 18;
constexpr uint8_t PIN_DOOR_SERVO = 19;
constexpr uint8_t PIN_WINDOW_SERVO = 32;
constexpr uint8_t PIN_LIGHT_LED = 21;
constexpr uint8_t PIN_FAN_LED = 22;
constexpr uint8_t PIN_AC_LED = 23;
constexpr uint8_t PIN_IRRIGATION_LED = 14;
constexpr uint8_t PIN_FRIDGE_LED = 17;
constexpr uint8_t PIN_BUZZER = 13;

#define DHT_TYPE DHT22
DHT roomSensor(PIN_DHT, DHT_TYPE);
OneWire oneWire(PIN_FRIDGE);
DallasTemperature fridgeSensor(&oneWire);

Servo gateServo;
Servo doorServo;
Servo windowServo;

constexpr int SOIL_DRY_RAW = 3000;
constexpr int SOIL_WET_RAW = 1400;
constexpr int SOIL_THRESHOLD = 35;
constexpr int DARK_THRESHOLD = 35;
constexpr float FAN_ON_TEMP = 27.0;
constexpr float FAN_OFF_TEMP = 25.0;

constexpr float AC_ON_TEMP = 30.0;
constexpr float AC_OFF_TEMP = 28.0;

constexpr float FRIDGE_ALERT_TEMP = 6.0;

constexpr float WINDOW_OPEN_TEMP = 28.0;
constexpr float WINDOW_CLOSE_TEMP = 26.0;

constexpr int LDR_DARK_RAW = 800;
constexpr int LDR_BRIGHT_RAW = 3000;

constexpr unsigned long FAST_SENSOR_INTERVAL = 500;
constexpr unsigned long SLOW_SENSOR_INTERVAL = 2500;
constexpr unsigned long PIR_WARMUP_TIME = 30000;

constexpr int GATE_OPEN_ANGLE = 100;
constexpr int GATE_CLOSED_ANGLE = 10;

constexpr int DOOR_OPEN_ANGLE = 90;
constexpr int DOOR_CLOSED_ANGLE = 10;

constexpr int WINDOW_OPEN_ANGLE = 80;
constexpr int WINDOW_CLOSED_ANGLE = 15;

constexpr unsigned long GATE_AUTO_CLOSE_MS = 8000;
constexpr unsigned long DOOR_AUTO_CLOSE_MS = 5000;

constexpr bool RAIN_ACTIVE_LOW = true;

float roomTemperature = NAN;
float roomHumidity = NAN;
float fridgeTemperature = NAN;

bool fridgeSensorValid = false;

int soilMoisture = 100;
int lightIntensity = 0;
int soilRaw = 0;
int lightRaw = 0;

bool motionDetected = false;
bool doorOpen = false;
bool windowOpen = false;
bool raining = false;

bool irrigationRequested = false;
bool lightsOn = false;
bool fanDemand = false;
bool acDemand = false;
bool fridgeAlert = false;

bool securityArmed = false;
bool alarmActive = false;

bool gateOpen = false;
bool modelDoorOpen = false;
bool modelWindowOpen = false;

unsigned long bootTime = 0;
unsigned long lastFastSensorRead = 0;
unsigned long lastSlowSensorRead = 0;
unsigned long gateOpenedAt = 0;
unsigned long doorOpenedAt = 0;



void initializePins();

void initializeSensors();

void initializeServos();

void initializeWiFi();

void initializeWebServer();

int readAnalogAverage(uint8_t pin);

void readFastSensors();

void readSlowSensors();

void updateAutomation();

void updateWindowAutomation();

void updateSecurity();

void updateIndicators();

void updateModelActuators();

void handleRoot();

void handleApi();

void closeModelGate();

void closeModelDoor();

void updateIrrigation();

void updateLighting();

void updateFan();

void updateAC();

void updateFridge();



void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("==============================");
    Serial.println(" SMART HOME AUTOMATION SYSTEM");
    Serial.println("==============================");

    bootTime = millis();

    initializePins();
    initializeSensors();
    initializeServos();
    initializeWiFi();
    initializeWebServer();

    Serial.println("System Initialization Complete.");
}



void loop()
{
    server.handleClient();

    readFastSensors();

    readSlowSensors();

    updateAutomation();

    updateSecurity();

    updateWindowAutomation();

    updateModelActuators();

    updateIndicators();

    delay(2);
}



void initializePins()
{
    pinMode(PIN_SOIL, INPUT);
    pinMode(PIN_LDR, INPUT);

    pinMode(PIN_PIR, INPUT);
    pinMode(PIN_RAIN, INPUT_PULLUP);
    pinMode(PIN_DOOR_REED, INPUT_PULLUP);
    pinMode(PIN_WINDOW_REED, INPUT_PULLUP);

    pinMode(PIN_LIGHT_LED, OUTPUT);
    pinMode(PIN_FAN_LED, OUTPUT);
    pinMode(PIN_AC_LED, OUTPUT);
    pinMode(PIN_IRRIGATION_LED, OUTPUT);
    pinMode(PIN_FRIDGE_LED, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);

    digitalWrite(PIN_LIGHT_LED, LOW);
    digitalWrite(PIN_FAN_LED, LOW);
    digitalWrite(PIN_AC_LED, LOW);
    digitalWrite(PIN_IRRIGATION_LED, LOW);
    digitalWrite(PIN_FRIDGE_LED, LOW);
    digitalWrite(PIN_BUZZER, LOW);

    analogReadResolution(12);

    Serial.println("GPIO Initialization Complete.");
}



void initializeSensors()
{
    roomSensor.begin();

    fridgeSensor.begin();
    fridgeSensor.setResolution(9);

    lastSlowSensorRead = millis() - SLOW_SENSOR_INTERVAL;

    Serial.println("Sensor Initialization Complete.");
}



void initializeServos()
{
    gateServo.setPeriodHertz(50);
    doorServo.setPeriodHertz(50);
    windowServo.setPeriodHertz(50);

    gateServo.attach(PIN_GATE_SERVO, 500, 2400);
    doorServo.attach(PIN_DOOR_SERVO, 500, 2400);
    windowServo.attach(PIN_WINDOW_SERVO, 500, 2400);

    gateServo.write(GATE_CLOSED_ANGLE);
    doorServo.write(DOOR_CLOSED_ANGLE);
    windowServo.write(WINDOW_CLOSED_ANGLE);

    Serial.println("Servo Initialization Complete.");
}



void initializeWiFi()
{
    WiFi.mode(WIFI_AP);

    bool result = WiFi.softAP(AP_SSID, AP_PASSWORD);

    if (result)
    {
        Serial.println("WiFi Access Point Started.");
        
        Serial.print("Network Name: ");
        Serial.println(AP_SSID);

        Serial.print("Dashboard IP: ");
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println("WiFi Initialization Failed.");
    }
}



void initializeWebServer()
{
    server.on("/", HTTP_GET, handleRoot);
    server.on("/api", HTTP_GET, handleApi);

    server.on
    (
        "/gate/open", HTTP_POST, []()
        {
            gateOpen = true;
            gateOpenedAt = millis();
            gateServo.write(GATE_OPEN_ANGLE);
            server.send(200, "text/plain", "Model gate opened.");
        }
    );

    server.on
    (
        "/gate/close", HTTP_POST, []()
        {
            closeModelGate();
            server.send(200, "text/plain", "Model gate closed.");
        }
    );

    server.on
    (
        "/door/open", HTTP_POST, []()
        {
            modelDoorOpen = true;
            doorOpenedAt = millis();
            doorServo.write(DOOR_OPEN_ANGLE);
            server.send(200, "text/plain", "Model door opened.");
        }
    );

    server.on
    (
        "/door/close", HTTP_POST, []()
        {
            closeModelDoor();
            server.send(200, "text/plain", "Model door closed.");
        }
    );

    server.on
    (
        "/security/arm", HTTP_POST, []()
        {
            securityArmed = true;
            server.send(200, "text/plain", "Security armed.");
        }
    );

    server.on
    (
        "/security/disarm", HTTP_POST, []()
        {
            securityArmed = false;
            alarmActive = false;
            server.send(200, "text/plain", "Security disarmed.");
        }
    );

    server.onNotFound
    (
        []()
        {
            server.send(404, "text/plain", "Page not found.");
        }
    );

    server.begin();

    Serial.println("Web Server Started.");
}



void handleRoot()
{
    server.send_P(200, "text/html", DASHBOARD_HTML);
}



int readAnalogAverage(uint8_t pin)
{
    const int sampleCount = 8;
    long total = 0;
    for (int i = 0; i < sampleCount; i++)
    {
        total += analogRead(pin);
        delayMicroseconds(500);
    }
    return total / sampleCount;
}



void readFastSensors()
{
    unsigned long currentTime = millis();
    if (currentTime - lastFastSensorRead < FAST_SENSOR_INTERVAL)
    {
        return;
    }
    lastFastSensorRead = currentTime;

    soilRaw = readAnalogAverage(PIN_SOIL);
    soilMoisture = constrain(map(soilRaw, SOIL_DRY_RAW, SOIL_WET_RAW, 0, 100), 0, 100);

    lightRaw = readAnalogAverage(PIN_LDR);
    lightIntensity = constrain(map(lightRaw, LDR_DARK_RAW, LDR_BRIGHT_RAW, 0, 100), 0, 100);

    motionDetected = (currentTime - bootTime >= PIR_WARMUP_TIME) && (digitalRead(PIN_PIR) == HIGH);

    int rainState = digitalRead(PIN_RAIN);
    raining = RAIN_ACTIVE_LOW ? (rainState == LOW) : (rainState == HIGH);

    doorOpen = digitalRead(PIN_DOOR_REED) == HIGH;
    windowOpen = digitalRead(PIN_WINDOW_REED) == HIGH;
}



void readSlowSensors()
{
    unsigned long currentTime = millis();
    if (currentTime - lastSlowSensorRead < SLOW_SENSOR_INTERVAL)
    {
        return;
    }
    lastSlowSensorRead = currentTime;

    float temperature = roomSensor.readTemperature();
    float humidity = roomSensor.readHumidity();
    if (isnan(temperature) || isnan(humidity))
    {
        roomTemperature = NAN;
        roomHumidity = NAN;
        Serial.println("DHT22 Sensor Error.");
    }
    else
    {
        roomTemperature = temperature;
        roomHumidity = humidity;
    }

    fridgeSensor.requestTemperatures();
    float fridgeValue = fridgeSensor.getTempCByIndex(0);
    if (fridgeValue != DEVICE_DISCONNECTED_C && fridgeValue >= -40.0 && fridgeValue <= 85.0)
    {
        fridgeTemperature = fridgeValue;
        fridgeSensorValid = true;
    }
    else
    {
        fridgeTemperature = NAN;
        fridgeSensorValid = false;
        Serial.println("Fridge Sensor Error.");
    }

    Serial.print("Room Temperature: ");
    Serial.println(roomTemperature);

    Serial.print("Room Humidity: ");
    Serial.println(roomHumidity);

    Serial.print("Fridge Temperature: ");
    Serial.println(fridgeTemperature);
}



void updateIrrigation()
{
    irrigationRequested = (soilMoisture < SOIL_THRESHOLD) && (!raining);
    if (irrigationRequested)
    {
        Serial.println("IRRIGATION REQUESTED.");
    }
}



void updateLighting()
{
    lightsOn = (lightIntensity < DARK_THRESHOLD) && (motionDetected);
}



void updateFan()
{
    if (isnan(roomTemperature))
    {
        fanDemand = false;
        return;
    }
    if (!fanDemand && roomTemperature >= FAN_ON_TEMP)
    {
        fanDemand = true;
    }
    else if (fanDemand && roomTemperature <= FAN_OFF_TEMP)
    {
        fanDemand = false;
    }
}



void updateAC()
{
    if (isnan(roomTemperature))
    {
        acDemand = false;
        return;
    }
    if (!acDemand && roomTemperature >= AC_ON_TEMP)
    {
        acDemand = true;
    }
    else if (acDemand && roomTemperature <= AC_OFF_TEMP)
    {
        acDemand = false;
    }
}



void updateFridge()
{
    if (!fridgeSensorValid)
    {
        fridgeAlert = false;
        return;
    }
    fridgeAlert = fridgeTemperature > FRIDGE_ALERT_TEMP;
    if (fridgeAlert)
    {
        Serial.println("WARNING: REFRIGERATOR TEMPERATURE HIGH.");
    }
}



void updateAutomation()
{
    updateIrrigation();
    updateLighting();
    updateFan();
    updateAC();
    updateFridge();
}



void updateSecurity()
{
    alarmActive = securityArmed && (motionDetected || doorOpen || windowOpen);
}



void updateWindowAutomation()
{
    bool desiredWindowState = modelWindowOpen;
    if (raining)
    {
        desiredWindowState = false;
    }
    else if (!isnan(roomTemperature))
    {
        if ( roomTemperature >= WINDOW_OPEN_TEMP)
        {
            desiredWindowState = true;
        }
        else if (roomTemperature <= WINDOW_CLOSE_TEMP)
        {
            desiredWindowState = false;
        }
    }
    if (desiredWindowState != modelWindowOpen)
    {
        modelWindowOpen = desiredWindowState;
        if (modelWindowOpen)
        {
            windowServo.write(WINDOW_OPEN_ANGLE);
            Serial.println("MODEL WINDOW OPENED.");
        }
        else
        {
            windowServo.write(WINDOW_CLOSED_ANGLE);
            Serial.println("MODEL WINDOW CLOSED.");
        }
    }
}



void updateModelActuators()
{
    unsigned long currentTime = millis();
    if (gateOpen && (currentTime - gateOpenedAt >= GATE_AUTO_CLOSE_MS))
    {
        closeModelGate();
    }
    if (modelDoorOpen && (currentTime - doorOpenedAt >= DOOR_AUTO_CLOSE_MS))
    {
        closeModelDoor();
    }
}



void closeModelGate()
{
    gateOpen = false;
    gateServo.write(GATE_CLOSED_ANGLE);
    Serial.println("MODEL GATE CLOSED.");
}



void closeModelDoor()
{
    modelDoorOpen = false;
    doorServo.write(DOOR_CLOSED_ANGLE);
    Serial.println("MODEL DOOR CLOSED.");
}



void updateIndicators()
{
    digitalWrite(PIN_LIGHT_LED, lightsOn ? HIGH : LOW);

    digitalWrite(PIN_FAN_LED, fanDemand ? HIGH : LOW);

    digitalWrite(PIN_AC_LED, acDemand ? HIGH : LOW);

    digitalWrite(PIN_IRRIGATION_LED, irrigationRequested ? HIGH : LOW);

    digitalWrite(PIN_FRIDGE_LED, fridgeAlert ? HIGH : LOW);

    digitalWrite(PIN_BUZZER, alarmActive ? HIGH : LOW);
}



void handleApi()
{
    String json = "{";

    json += "\"roomTemperature\":";
    if (isnan(roomTemperature))
    {
        json += "null";
    }
    else
    {
        json += String(roomTemperature, 1);
    }

    json += ",\"humidity\":";
    if (isnan(roomHumidity))
    {
        json += "null";
    }
    else
    {
        json += String(roomHumidity, 1);
    }

    json += ",\"soilMoisture\":";
    json += String(soilMoisture);

    json += ",\"lightIntensity\":";
    json += String(lightIntensity);

    json += ",\"fridgeTemperature\":";
    if (!fridgeSensorValid)
    {
        json += "null";
    }
    else
    {
        json += String(fridgeTemperature, 1);
    }

    json += ",\"raining\":";
    json += (raining ? "true" : "false");

    json += ",\"motionDetected\":";
    json += (motionDetected ? "true" : "false");

    json += ",\"doorOpen\":";
    json += (doorOpen ? "true" : "false");

    json += ",\"windowOpen\":";
    json += (windowOpen ? "true" : "false");

    json += ",\"irrigationRequested\":";
    json += (irrigationRequested ? "true" : "false");

    json += ",\"lightsOn\":";
    json += (lightsOn ? "true" : "false");

    json += ",\"fanDemand\":";
    json += (fanDemand ? "true" : "false");

    json += ",\"acDemand\":";
    json += (acDemand ? "true" : "false");

    json += ",\"fridgeAlert\":";
    json += (fridgeAlert ? "true" : "false");

    json += ",\"securityArmed\":";
    json += (securityArmed ? "true" : "false");

    json += ",\"alarmActive\":";
    json += (alarmActive ? "true" : "false");

    json += ",\"gateOpen\":";
    json += (gateOpen ? "true" : "false");

    json += ",\"modelDoorOpen\":";
    json += (modelDoorOpen ? "true" : "false");

    json += ",\"modelWindowOpen\":";
    json += (modelWindowOpen ? "true" : "false");

    json += "}";

    server.send(200, "application/json", json);
}

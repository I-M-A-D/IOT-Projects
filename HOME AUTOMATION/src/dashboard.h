#pragma once
#include <pgmspace.h>



const char DASHBOARD_HTML[] PROGMEM = R"HTML(

<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Smart Home Automation</title>
<style>
body {
    font-family: Arial, sans-serif;
    background: #101827;
    color: white;
    margin: 0;
    padding: 20px;
}
h1 {
    text-align: center;
}
.container {
    max-width: 850px;
    margin: auto;
}
.grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(190px, 1fr));
    gap: 12px;
}
.card {
    background: #1e293b;
    padding: 16px;
    border-radius: 12px;
}
.value {
    color: #38bdf8;
    font-size: 23px;
    font-weight: bold;
}
button {
    background: #2563eb;
    color: white;
    border: none;
    border-radius: 7px;
    padding: 11px 15px;
    margin: 4px;
    cursor: pointer;
}
button:hover {
    background: #1d4ed8;
}
.status {
    color: #34d399;
}
.warning {
    color: #fb923c;
}
.section {
    margin-top: 25px;
}
</style>
</head>
<body>
<div class="container">
<h1>SMART HOME AUTOMATION</h1>
<p style="text-align:center">
ESP32 IoT Monitoring Dashboard
</p>

<div class="grid">
    <div class="card">
        <h3>Room Temperature</h3>
        <div class="value" id="temp">--</div>
    </div>
    <div class="card">
        <h3>Humidity</h3>
        <div class="value" id="humidity">--</div>
    </div>
    <div class="card">
        <h3>Soil Moisture</h3>
        <div class="value" id="soil">--</div>
    </div>
    <div class="card">
        <h3>Light Intensity</h3>
        <div class="value" id="light">--</div>
    </div>
    <div class="card">
        <h3>Fridge Temperature</h3>
        <div class="value" id="fridge">--</div>
    </div>
    <div class="card">
        <h3>Rain Detection</h3>
        <div class="value" id="rain">--</div>
    </div>
</div>

<div class="section">
<h2>Automatic Systems</h2>
<div class="grid">
    <div class="card">
        <h3>Lighting</h3>
        <p id="lights">--</p>
    </div>
    <div class="card">
        <h3>Fan Demand</h3>
        <p id="fan">--</p>
    </div>
    <div class="card">
        <h3>AC Demand</h3>
        <p id="ac">--</p>
    </div>
    <div class="card">
        <h3>Irrigation</h3>
        <p id="irrigation">--</p>
    </div>
    <div class="card">
        <h3>Fridge Alert</h3>
        <p id="fridgeAlert">--</p>
    </div>
</div>
</div>

<div class="section">
<h2>Security Monitoring</h2>
<div class="grid">
    <div class="card">
        <h3>Security Mode</h3>
        <p id="security">Disarmed</p>
        <button onclick="command('/security/arm')">ARM</button>
        <button onclick="command('/security/disarm')">DISARM</button>
    </div>
    <div class="card">
        <h3>Security Alarm</h3>
        <p id="alarm">--</p>
    </div>
    <div class="card">
        <h3>Door Sensor</h3>
        <p id="doorSensor">--</p>
    </div>
    <div class="card">
        <h3>Window Sensor</h3>
        <p id="windowSensor">--</p>
    </div>
    <div class="card">
        <h3>Motion Detection</h3>
        <p id="motion">--</p>
    </div>
</div>
</div>

<div class="section">
<h2>Model Gate Control</h2>
<div class="card">
    <p id="gate">--</p>
    <button onclick="command('/gate/open')">Open Gate</button>
    <button onclick="command('/gate/close')">Close Gate</button>
    <p>Model gate automatically closes after 8 seconds.</p>
</div>
</div>

<div class="section">
<h2>Model Door Control</h2>
<div class="card">
    <p id="modelDoor">--</p>
    <button onclick="command('/door/open')">Open Door</button>
    <button onclick="command('/door/close')">Close Door</button>
    <p>Model door automatically closes after 5 seconds.</p>
</div>
</div>

<div class="section">
<h2>Automatic Window Control</h2>
<div class="card">
    <p id="modelWindow">--</p>
    <p>
    The model window opens when the room is hot
    and closes when rain is detected.
    </p>
</div>
</div>
</div>

<script>
async function command(path) {
    try {
        await fetch(path, {
            method: "POST"
        });
        setTimeout(refresh, 200);
    }
    catch(error) {
        console.log(error);
    }
}

async function refresh() {
    try {
        const response = await fetch("/api");
        const data = await response.json();

        document.getElementById("temp").textContent =
            data.roomTemperature === null
            ? "Sensor Error"
            : data.roomTemperature + " °C";

        document.getElementById("humidity").textContent =
            data.humidity === null
            ? "Sensor Error"
            : data.humidity + " %";

        document.getElementById("soil").textContent =
            data.soilMoisture + " %";

        document.getElementById("light").textContent =
            data.lightIntensity + " %";

        document.getElementById("fridge").textContent =
            data.fridgeTemperature === null
            ? "Sensor Error"
            : data.fridgeTemperature + " °C";

        document.getElementById("rain").textContent =
            data.raining ? "RAIN DETECTED" : "NO RAIN";

        document.getElementById("lights").textContent =
            data.lightsOn ? "ON" : "OFF";

        document.getElementById("fan").textContent =
            data.fanDemand ? "ON REQUEST" : "OFF";

        document.getElementById("ac").textContent =
            data.acDemand ? "COOLING REQUEST" : "OFF";

        document.getElementById("irrigation").textContent =
            data.irrigationRequested
            ? "WATERING REQUEST"
            : "NO REQUEST";

        document.getElementById("fridgeAlert").textContent =
            data.fridgeAlert
            ? "TEMPERATURE ALERT"
            : "NORMAL / NO ALERT";

        document.getElementById("security").textContent =
            data.securityArmed ? "ARMED" : "DISARMED";

        document.getElementById("alarm").textContent =
            data.alarmActive ? "ALARM ACTIVE" : "NORMAL";

        document.getElementById("doorSensor").textContent =
            data.doorOpen ? "OPEN" : "CLOSED";

        document.getElementById("windowSensor").textContent =
            data.windowOpen ? "OPEN" : "CLOSED";

        document.getElementById("motion").textContent =
            data.motionDetected ? "MOTION DETECTED" : "NO MOTION";

        document.getElementById("gate").textContent =
            data.gateOpen ? "OPEN" : "CLOSED";

        document.getElementById("modelDoor").textContent =
            data.modelDoorOpen ? "OPEN" : "CLOSED";

        document.getElementById("modelWindow").textContent =
            data.modelWindowOpen ? "OPEN" : "CLOSED";
    }
    catch(error) {
        console.log("Dashboard connection error:", error);
    }
}

// Update dashboard every 2 seconds
setInterval(refresh, 2000);
refresh();
</script>
</body>
</html>

)HTML";
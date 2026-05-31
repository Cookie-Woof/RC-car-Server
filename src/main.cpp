/*
main.cpp - ESP32 TCP server for controlling a servo motor
- Connects to WiFi and starts a TCP server on port 9000
acts as the server for the program running on the esp32.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <iostream>

#define WIFI_NAME   "Melar2.4"
#define WIFI_PASS   "ariel778"

// #define WIFI_NAME "Adassim"
// #define WIFI_PASS "20406080"

#define SERVO_PIN   18

#define PORT        9000
#define ANGLE_MIN   80
#define ANGLE_MAX   140
#define ANGLE_CENTER 110

Servo steeringServo;
WiFiServer server(PORT);

void setup() {
    Serial.begin(115200);

    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_NAME, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Serial.println("WiFi connected!");
    std::cout << "WiFi connected!" << "/n";
    // Serial.println("ESP32 IP address: ");
    std::cout << "ESP32 IP address: " << WiFi.localIP() << "\n";
    // Serial.print(WiFi.localIP());
    server.begin();
    // Serial.println("TCP server started on port " + String(PORT));
    std::cout << "TCP server started on port " << PORT << "\n";

    steeringServo.attach(SERVO_PIN);
    steeringServo.write(ANGLE_CENTER);
    // Serial.println("Servo ready at center (110)");
    std::cout << "Servo ready at center (" << ANGLE_CENTER << ")\n";
}

void loop() {
    WiFiClient client = server.available();

    if (client) {
        Serial.println("Client connected");

        while (client.connected()) {
            if (client.available()) {
                String incoming = client.readStringUntil('\n');
                incoming.trim();

                int angle = incoming.toInt();

                if (angle >= ANGLE_MIN && angle <= ANGLE_MAX) {
                    steeringServo.write(angle);
                    // Serial.println("Angle: " + String(angle));
                    std::cout << "Angle: " << angle << "\n";
                } else {
                    // Serial.println("Ignored out-of-range value: " + incoming);
                    std::cout << "Ignored out-of-range value: " << incoming << "\n";
                }
            }
        }

        client.stop();
        Serial.println("Client disconnected");
    }
}

#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <WiFiUdp.h>
#include <vector>
#include <iostream>


#define WIFI_NAME   "Melar2.4"
#define WIFI_PASS   "ariel778"

// // #define WIFI_NAME "Adassim"
// // #define WIFI_PASS "20406080"

#define SERVO_PIN    18
#define PORT         5005
#define ANGLE_MIN    87
#define ANGLE_MAX    140
#define ANGLE_CENTER 117


struct ControlPacket {
    int angle;
    float throttle;
    float speed;
};

Servo steeringServo;
WiFiUDP udpServer;

std::vector<IPAddress> playerQueue;

//==============================================================
void setup() {
    Serial.begin(115200);

    std::cout << "Connecting to WiFi...";
    WiFi.begin(WIFI_NAME, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        std::cout << ".";
    }
    
    std::cout << "\nWiFi connected!\n";
    std::cout << "ESP32 IP address: " << WiFi.localIP().toString().c_str() << "\n";

    udpServer.begin(PORT);
    std::cout << "UDP Server active on port " << PORT << "\n";

    steeringServo.attach(SERVO_PIN);
    steeringServo.write(ANGLE_CENTER);
    std::cout << "Servo ready at center (" << ANGLE_CENTER << ")\n";
}

//==============================================================

void loop() {
    int packetSize = udpServer.parsePacket();

    if (packetSize >= sizeof(ControlPacket)) {
        IPAddress senderIP = udpServer.remoteIP();
        ControlPacket packet;
        
        udpServer.read((uint8_t*)&packet, sizeof(ControlPacket));

        int myPosition = -1;
        for (size_t i = 0; i < playerQueue.size(); i++) {
            if (playerQueue[i] == senderIP) {
                myPosition = i;
                break;
            }
        }

        if (myPosition == -1) {
            playerQueue.push_back(senderIP);
            myPosition = playerQueue.size() - 1;
            std::cout << "NEW PILOT LOGGED IN. Queue Size: " << playerQueue.size() << "\n";
        }

        if (myPosition == 0) {

            if (packet.angle >= ANGLE_MIN && packet.angle <= ANGLE_MAX) {
                steeringServo.write(packet.angle);
                std::cout << "Angle: " << packet.angle << "\n";
            }

            int throttlePercent = (int)(packet.throttle * 100);
            if (throttlePercent >= 0 && throttlePercent <= 100) {
                std::cout << "Throttle: " << throttlePercent << "%\n";

            }
        }

        udpServer.beginPacket(senderIP, udpServer.remotePort());
        udpServer.write((uint8_t*)&myPosition, sizeof(myPosition));
        udpServer.endPacket();
    }
}
// #include <Arduino.h>
// #include <ESP32Servo.h>
// #include <WiFi.h>
// #include <WiFiUdp.h>

// // 1. Radio Settings (Access Point)
// const char* netWorkName = "RaceCar";
// const char* password = "JdmLegend180sxorMx5";

// WiFiUDP udp;
// unsigned int port = 6000;
// char packetBuffer[255];


// // put function / declarations here:
// Servo myServo;
// const int servoPin = 12;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   myServo.attach(servoPin);

//   WiFi.softAP(netWorkName, password);
//   Serial.println("Robot Radio is LIVE.");
//   Serial.print("Connect to WiFi: "); Serial.println(netWorkName);

//   udp.begin(port);
  

//   // ESP32PWM::allocateTimer(0);
//   // ESP32PWM::allocateTimer(1);
//   // ESP32PWM::allocateTimer(2);
//   // ESP32PWM::allocateTimer(3);

//   // myServo.setPeriodHertz(50);
//   // myServo.attach(servoPin, 500, 2400);
// }

// void loop() {
//   // put your main code here, to run repeatedly:

//   int packetSize = udp.parsePacket();
//   if(packetSize) { // if we got a new packet ready for us to check out
//     int packetLen = udp.read(packetBuffer, 255); // limit the esp reading to a max of 255
//     if(packetLen > 0) {
//       packetBuffer[packetLen] = 0;
//     }

//     int angle = atoi(packetBuffer);

//     //move
//     if (angle >= 0 && angle <= 180) {
//       myServo.write(angle);
//       Serial.println("Steering to: "); Serial.println(angle);
//     }
//   }
//   // for(int pos = 0; pos <= 180; pos++) {
//   //   myServo.write(pos);
//   //   delay(15);
//   // }
//   // for (int pos = 180; pos >= 0; pos--) {
//   //   myServo.write(pos);
//   //   delay(15);
//   // }
 
// }
#include <ESP32Servo.h>

Servo myServo;
int servoPin = 13; 

void setup() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  myServo.setPeriodHertz(50);    // Standard 50hz servo
  myServo.attach(servoPin, 500, 2400); // Attach with min/max pulse widths
}

void loop() {
  myServo.write(80);  // Move to 90 degrees
  delay(1000);
   myServo.write(110);   // Move to 0 degrees
  delay(1000);
  myServo.write(130); // Move to 180 degrees
  delay(1000);
}
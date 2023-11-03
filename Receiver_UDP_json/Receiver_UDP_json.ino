  // Include the ArduinoJSON library
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "stepper";       // Your network SSID
const char* password = "stepper123"; // Your network password
const int localPort = 12345;
WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());


  // Set up the UDP server
  udp.begin(localPort);
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }
  char packetBuffer[255];
  int packetSize = udp.parsePacket();
  
  if (packetSize) {
    udp.read(packetBuffer, packetSize);
    packetBuffer[packetSize] = 0;
    Serial.println(packetBuffer);
//    Serial.print("Received: ");
    

    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.print(0);
    udp.endPacket();
//    Serial.println("Sent confirmation flag.");
    } else {
      


    // Send a confirmation flag back
//    udp.beginPacket(udp.remoteIP(), udp.remotePort());
//    udp.print(0);
//    udp.endPacket();
//    Serial.println("Sent confirmation flag.");
    }
    

  }

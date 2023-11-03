#include <WiFi.h>
#include <WiFiUdp.h>
const char* ssid = "stepper";       // Your network SSID
const char* password = "stepper123"; // Your network password
const char* host = "192.168.4.2"; // IP address of the receiving ESP32
WiFiUDP udp;
String incomingData = "";
bool newData = false;
void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Set up the UDP client
  udp.begin(12345);
}

void loop() {
//  if (Serial.available()) {
//    String data = Serial.readString();
//    sendUDPData(data);
//  }
if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n') {
      // End of the line, send the data through ESP-NOW
      if (newData) {
        // Send the data to the receiver
          sendUDPData(incomingData);
//        esp_now_send(receiverMac, (uint8_t *)incomingData.c_str(), incomingData.length());
//        Serial.println("Sending data: " + incomingData);
        incomingData = ""; // Clear the data buffer
        newData = false;
      }
    } else {
      // Add the character to the incoming data buffer
      incomingData += c;
      newData = true;
    }
}
}
void sendUDPData(String data) {
  // Send data to the receiving ESP32
  udp.beginPacket(host, 12345);
  udp.print(data);
  udp.endPacket();
  //Serial.println("Sent: " + data);

  // Wait for a response
  char reply[10];
  int packetSize = udp.parsePacket();
  if (packetSize) {
    udp.read(reply, packetSize);
//      Serial.println(reply);
//    Serial.print("Received flag: ");
//    Serial.println();
    Serial.write(reply,packetSize);
    reply[packetSize] = 0;
    
  }
}

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "tamimas iphone";
const char* password = "12345678";
const char* serverIp = "172.20.10.9"; 

const int ledPin = 2;    // Pin D4 (GPIO 2)
const int sensorPin = A0; // Defined sensor variable

void setup() {
  Serial.begin(115200); // Start serial for debugging
  pinMode(ledPin, OUTPUT);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  int sensorValue = analogRead(sensorPin); 
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue); 

  int brightness = 0;

  // Define clear states based on your 20-80 range
  if (sensorValue < 30) {
    brightness = 0;    // State 1: Dark / Off
    Serial.println("State: OFF");
  } 
  else if (sensorValue < 50) {
    brightness = 64;   // State 2: Dim Glow (25% power)
    Serial.println("State: DIM");
  } 
  else if (sensorValue < 70) {
    brightness = 150;  // State 3: Warm Glow (60% power)
    Serial.println("State: WARM");
  } 
  else {
    brightness = 255;  // State 4: Full Brightness (100% power)
    Serial.println("State: BRIGHT");
  }

  // Apply the stepped brightness to the Red LED
  analogWrite(ledPin, brightness); 

  // Send the raw data to the Mac so Ball 2 stays in sync
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    String url = "http://" + String(serverIp) + ":5000/update?l=" + String(sensorValue);
    http.begin(client, url);
    http.GET();
    http.end();
  }
  delay(500);
}

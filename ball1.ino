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
  // Use the sensor variable to read the pin
  int sensorValue = analogRead(sensorPin); 
  
  // Print the raw value so you can see if it changes
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue); 
  
  int brightness = map(sensorValue, 25, 75, 0, 255); 
  brightness = constrain(brightness, 0, 255);
  
  // Apply the brightness to the Red LED
  analogWrite(ledPin, brightness); 

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

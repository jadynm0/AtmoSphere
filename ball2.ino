#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal.h>

const char* ssid = "tamimas iphone"; 
const char* password = "12345678";
const char* serverIp = "172.20.10.9"; 

// --- LED SETTINGS ---
const int ledPin = 2; // Pin D4 (GPIO 2)

// --- LCD SETTINGS ---
// RS, E, D4, D5, D6, D7 
LiquidCrystal lcd(16, 14, 12, 13, 4, 5); 

void setup() {
  Serial.begin(115200);
  
  // Initialize Red LED
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0); // Start off

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("AtmoSphere Sync");
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("Sync Active!");
  delay(1000);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    
    String url = "http://" + String(serverIp) + ":5000/get_light";
    http.begin(client, url);
    
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      int start = payload.indexOf(":") + 1;
      int end = payload.indexOf("}");
      int remoteLightValue = payload.substring(start, end).toInt();

      int brightness = 0;
      String stateName = "";

      // --- MATCHING THE BALL 1 STATES ---
      if (remoteLightValue < 30) {
        brightness = 0;
        stateName = "OFF  "; // Spaces clear old text
      } 
      else if (remoteLightValue < 50) {
        brightness = 64;  // 25% Power
        stateName = "DIM  ";
      } 
      else if (remoteLightValue < 70) {
        brightness = 150; // 60% Power
        stateName = "WARM ";
      } 
      else {
        brightness = 255; // 100% Power
        stateName = "BRIGHT";
      }

      // --- UPDATE LCD ---
      lcd.setCursor(0, 0);
      lcd.print("Toronto: ");
      lcd.print(stateName); 
      
      lcd.setCursor(0, 1);
      lcd.print("Value: ");
      lcd.print(remoteLightValue);
      lcd.print("     "); // Extra spaces to prevent "ghost" digits

      // --- APPLY TO RED LED ---
      analogWrite(ledPin, brightness);

    } else {
      lcd.setCursor(0, 1);
      lcd.print("Server Err: ");
      lcd.print(httpCode);
    }
    http.end();
  } else {
    WiFi.begin(ssid, password);
  }
  
  delay(1000); 
}

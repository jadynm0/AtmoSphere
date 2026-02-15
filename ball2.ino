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
    
    // Request data from the Python server
    String url = "http://" + String(serverIp) + ":5000/get_light";
    http.begin(client, url);
    
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      
      // Parse JSON: {"light": 45}
      int start = payload.indexOf(":") + 1;
      int end = payload.indexOf("}");
      String val = payload.substring(start, end);
      val.trim();

      int remoteLightValue = val.toInt();

      // --- UPDATE LCD ---
      lcd.setCursor(0, 0);
      lcd.print("Toronto Light: ");
      lcd.setCursor(0, 1);
      lcd.print(remoteLightValue);
      lcd.print(" units    "); 

      // --- BRIGHTNESS MATH ---
      // Matching Ball 1's range (25 to 75)
      int brightness = map(remoteLightValue, 25, 75, 0, 255);
      brightness = constrain(brightness, 0, 255);

      // Apply to Red LED
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
  
  delay(1000); // Sync every second
}

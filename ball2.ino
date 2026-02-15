#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "jadyn"; 
const char* password = "bigfatgio";
const char* serverIp = "172.20.10.4"; // Use the IP from Option+Click WiFi

// --- LED SETTINGS ---
#define LED_PIN 2    // This is Pin D4 on most ESP8266 boards
#define NUM_LEDS 10  // Adjust this to the number of LEDs you have
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// --- LCD SETTINGS ---
// RS, E, D4, D5, D6, D7 
// Mapped to: D0(16), D5(14), D6(12), D7(13), D2(4), D1(5)
LiquidCrystal lcd(16, 14, 12, 13, 4, 5); 

void setup() {
  Serial.begin(115200);
  
  // Initialize LEDs
  strip.begin();
  strip.setBrightness(50); // Keep it at 50 to save power for the LCD
  strip.show(); 

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
      
      // Parse JSON: {"light": 450}
      int start = payload.indexOf(":") + 1;
      int end = payload.indexOf("}");
      String val = payload.substring(start, end);
      val.trim();

      int remoteLightValue = val.toInt();

      // --- UPDATE LCD ---
      lcd.setCursor(0, 0);
      lcd.print("Toronto Status: ");
      lcd.setCursor(0, 1);
      
      if (remoteLightValue < 500) {
        lcd.print("DARK (");
        lcd.print(remoteLightValue);
        lcd.print(")    ");
        
        // --- UPDATE LEDs (Orange/Sunset) ---
        for(int i=0; i<NUM_LEDS; i++) {
          strip.setPixelColor(i, strip.Color(255, 100, 0)); 
        }
      } else {
        lcd.print("BRIGHT (");
        lcd.print(remoteLightValue);
        lcd.print(")  ");
        
        // --- UPDATE LEDs (Blue/Daylight) ---
        for(int i=0; i<NUM_LEDS; i++) {
          strip.setPixelColor(i, strip.Color(0, 150, 255));
        }
      }
      strip.show();

    } else {
      lcd.setCursor(0, 1);
      lcd.print("Server Err: ");
      lcd.print(httpCode);
    }
    http.end();
  } else {
    lcd.clear();
    lcd.print("WiFi Lost...");
    WiFi.begin(ssid, password);
  }
  
  delay(1500); // Check for updates every 1.5 seconds
}
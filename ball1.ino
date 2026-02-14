#include <Adafruit_NeoPixel.h>

#define LED_PIN    2   // Pin D4
#define SENSOR_PIN A0  // Analog Light Sensor
#define NUM_LEDS   10 

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200); // Start communication with your Mac
  strip.begin();
  strip.show();
}

void loop() {
  int lightValue = analogRead(SENSOR_PIN); // Read the room brightness
  Serial.print("Light Level: ");
  Serial.println(lightValue);

  // If it's dark (tweak this number based on your room)
  if (lightValue < 500) {
    for(int i=0; i<NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(255, 100, 0)); // Sunset Orange
    }
  } else {
    strip.clear(); // Turn off if it's bright
  }

  strip.show();
  delay(100);
}
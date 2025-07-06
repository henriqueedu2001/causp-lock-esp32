#include <Arduino.h>

#define BAUD_RATE 115200
#define LED_BUILTIN 4

void ledBlink(int n);

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  
}

/**
 * @brief Blinks the ESP32-CAM Led n times, for a given duration
 * @param n The number of blinks (int)
 * @param duration The duration of each blink 
 */
void ledBlink(int n, int duration) {
  for(int i = 0; i < n; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(duration);
    digitalWrite(LED_BUILTIN, LOW);
    delay(duration);
  }
}

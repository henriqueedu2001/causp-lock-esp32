#include <Arduino.h>
#include <qrcode.h>
#include <decoder.h>
#include <stdint.h>

#include "esp_heap_caps.h"

#define BAUD_RATE 115200
#define LED_BUILTIN 4

void ledBlink(int n);
void printHeapFreeSize();

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
  setupQRCodeReader();
  resumeQRCodeReading();
}

void loop() {
  QRCodePayload qrcodePayload = readQRCode();

  if(qrcodePayload.successfulRead) {
    DecodedQRCodeData decodedQRCodeData = getQRCodeData(qrcodePayload.rawPayload, qrcodePayload.payloadLength);
    printDecodedQRCodeData(decodedQRCodeData);
    freeMallocData(&decodedQRCodeData);
    ledBlink(1, 50);
  }
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

/**
 * @brief Prints the ESP32-CAM heap free size in bytes
 */
void printHeapFreeSize() {
  printf("Heap free size: %u bytes\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
}
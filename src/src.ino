#include <Arduino.h>
#include <qrcode.h>
#include <decoder.h>
#include <stdint.h>
#include <auth.h>

#include "esp_heap_caps.h"

#define BAUD_RATE 115200
#define ELETRIC_LOCK_PINK 2
#define LED_BUILTIN 4

void ledBlink(int n);
void printHeapFreeSize();
void unlock();

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ELETRIC_LOCK_PINK, OUTPUT);
  setupQRCodeReader();
  resumeQRCodeReading();
}

void loop() {
  QRCodePayload qrcodePayload = readQRCode();
  if(qrcodePayload.successfulRead) {
    DecodedQRCodeData decodedQRCodeData = getQRCodeData(qrcodePayload.rawPayload, qrcodePayload.payloadLength);
    bool validity = validateMessage(
      decodedQRCodeData.payloadMessage,
      decodedQRCodeData.payloadHash,
      decodedQRCodeData.messageLength,
      decodedQRCodeData.messageType
    );
    printDecodedQRCodeData(decodedQRCodeData);
    Serial.print("validity: ");
    Serial.println(validity);
    ledBlink(1, 50);
    if(validity) {
      unlock();
    }
    freeMallocData(&decodedQRCodeData);
  }
}

void unlock() {
  digitalWrite(ELETRIC_LOCK_PINK, HIGH);
  delay(1000);
  digitalWrite(ELETRIC_LOCK_PINK, LOW);
  delay(4000);
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
#include <Arduino.h>
#include <qrcode.h>
#include <decoder.h>
#include <stdint.h>
#include <auth.h>

#include "esp_heap_caps.h"

#define BAUD_RATE 115200
#define LED_BUILTIN 4

uint8_t DEFAULT_ACCESS_KEY[20] = {
  0x85, 0xf1, 0xe2, 0x04, 0xba, 0x63, 0xfe, 0x41, 0xa0, 0xf0,
  0xda, 0x37, 0x74, 0x3e, 0x8d, 0x1c, 0x6a, 0xf5, 0x33, 0xfc
};

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
    uint8_t outputHash[20];
    getHMAC_SHA1(DEFAULT_ACCESS_KEY, KEY_LENGTH, decodedQRCodeData.payloadMessage, decodedQRCodeData.messageLength, outputHash);
    bool signatureValidity = validateSignature(decodedQRCodeData.payloadHash, outputHash);
    printHMACSHA1(outputHash);
    Serial.print("Signature validity: ");
    Serial.println(signatureValidity);
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
#include <ESP32QRCodeReader.h>
#include <stdint.h>

typedef struct {
  uint8_t *rawPayload;
  int payloadLength;
  bool successfulRead;
} QRCodePayload;

void setupQRCodeReader();
void resumeQRCodeReading();
void suspendQRCodeReading();
void setReadingDelay(int newDelay);

QRCodePayload readQRCode();
void printQRCodePayload(QRCodePayload qrcode);
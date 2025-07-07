#include <ESP32QRCodeReader.h>

void setupQRCodeReader();
void resumeQRCodeReading();
void suspendQRCodeReading();
void setReadingDelay(int newDelay);
char *readQRCode();
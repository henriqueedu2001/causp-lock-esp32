#include <qrcode.h>
#include <stdint.h>

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);

void onQrCodeTask(void *pvParameters);

int readingDelay = 50;
bool readingQRCode = false;

QRCodePayload qrCodePayload = {
  NULL,
  -1,
  false
};

/**
 * @brief Setups the QR Code reader
 */
void setupQRCodeReader() {
  reader.setup();
  reader.beginOnCore(1);
  xTaskCreate(onQrCodeTask, "onQrCode", 4 * 1024, NULL, 4, NULL);
  resumeQRCodeReading();
}

/**
 * @brief Resumes the QR Code reading, if suspended
 */
void resumeQRCodeReading() {
  readingQRCode = true;
}

/**
 * @brief Suspend the QR Code reading
 */
void suspendQRCodeReading() {
  readingQRCode = false;
}

/**
 * @brief Sets the QR Code reading task delay
 * @param newDelay The new delay
 */
void setReadingDelay(int newDelay) {
  readingDelay = newDelay;
}

/**
 * @brief The QR Code reading task from the RTOS
 */
void onQrCodeTask(void *pvParameters) {
  struct QRCodeData qrCodeData;
  while (true) {
    if (readingQRCode) {
      if (reader.receiveQrCode(&qrCodeData, 100)) {
        if (qrCodeData.valid) {
          qrCodePayload.rawPayload = qrCodeData.payload;
          qrCodePayload.payloadLength = qrCodeData.payloadLen;
          qrCodePayload.successfulRead = true;
        } else {
          qrCodePayload.successfulRead = false;
        }
      } else {
        qrCodePayload.successfulRead = false;
      }
    }
    vTaskDelay(readingDelay / portTICK_PERIOD_MS);
  }
}

/**
 * @brief Reads the QR Code from the ESP32-CAM
 * @return The read QR Code payload
 */
QRCodePayload readQRCode() {
  return qrCodePayload;
}

/**
 * @brief Prints the QR Code payload in the HEX format
 * @param qrcode The QR Code payload
 * @param format The print format (DEC, HEX, OCT, etc.)
 */
void printQRCodePayload(QRCodePayload qrcode, int format) {
  if(qrcode.successfulRead) {
    for(int i = 0; i < qrcode.payloadLength; i++) {
      uint8_t payloadByte = qrcode.rawPayload[i];
      if(payloadByte <= 15 && format == HEX) Serial.print("0"); /* 0 left padding for 0 to F */
      Serial.print(payloadByte, format);
      Serial.print(" ");
    }
    Serial.println();
  }
}
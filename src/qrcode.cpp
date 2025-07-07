#include <qrcode.h>

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);

void onQrCodeTask(void *pvParameters);

int readingDelay = 50;
bool readingQRCode = false;
bool successfulRead = false;
char *rawPayload = NULL;

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
          rawPayload = (char *)qrCodeData.payload;
          successfulRead = true;
        } else {
          successfulRead = false;
        }
      } else {
        successfulRead = false;
      }
    }
    vTaskDelay(readingDelay / portTICK_PERIOD_MS);
  }
}

/**
 * @brief Reads the QR Code from the ESP32-CAM
 * @return The read QR Code payload or NULL if the read failed
 */
char *readQRCode() {
  if (successfulRead) {
    return rawPayload;
  }
  return NULL;
}
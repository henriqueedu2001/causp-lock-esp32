#include <stdint.h>
#include <iostream>
#include <Arduino.h>

#define HEADER_LENGTH 1
#define HASH_LENGTH 20
#define USER_ID_LENGTH 4
#define DATETIME_LENGTH 4
#define INT_LENGTH 4

typedef struct {
  uint8_t payloadHeader;
  uint8_t *payloadBody;
  uint8_t *payloadHash;
  uint8_t *payloadMessage;
  uint8_t messageType;
  uint8_t operationType;
  int userId;
  int generatedAt;
  int syncTime;
  int debugBlink;
  int debugSyncTime;
  uint8_t *newKey;
  bool successfulDecoding;
  int bodyLength;
  int messageLength;
} DecodedQRCodeData;

DecodedQRCodeData getQRCodeData(uint8_t *payload, int payloadLength);
void freeMallocData(DecodedQRCodeData *decodedQRCodeData);

uint8_t getPayloadHeader(uint8_t *payload);
uint8_t *getPayloadBody(uint8_t *payload, int payloadLength);
uint8_t *getPayloadHash(uint8_t *payload, int payloadLength);
uint8_t *getPayloadMessage(uint8_t *payload, int payloadLength);

uint8_t getMessageType(uint8_t header);
uint8_t getOperationType(uint8_t header);
int getUserId(uint8_t *body);
int getGeneratedAt(uint8_t *body);
int getSyncTime(uint8_t *body);
int getDebugBlink(uint8_t *body);
int getDebugSyncTime(uint8_t *body);
uint8_t *getNewKey(uint8_t *body);

void printDecodedQRCodeData(DecodedQRCodeData decodedQRCodeData);
#include <stdint.h>
#include <iostream>
#include <Arduino.h>

#define HEADER_LENGTH 1
#define HASH_LENGTH 20
#define USER_ID_LENGTH 4
#define DATETIME_LENGTH 4
#define INT_LENGTH 4
#define NEW_KEY_LENGTH 20

/* message types */
#define MESSAGE_TYPE_ACCESS 0
#define MESSAGE_TYPE_SYNC 1
#define MESSAGE_TYPE_CONFIG 2
#define MESSAGE_TYPE_DEBUG 3

/* operation types */
#define OPERATION_TYPE_CHECK_IN 0
#define OPERATION_TYPE_CHECK_OUT 1
#define OPERATION_TYPE_BI_ACCESS 2
#define OPERATION_TYPE_CHECK_IN 0
#define OPERATION_SET_TIME = 0
#define OPERATION_SET_MASTER_KEY = 0
#define OPERATION_SET_CONFIG_KEY = 1
#define OPERATION_SET_SYNC_KEY = 2
#define OPERATION_SET_ACCESS_KEY = 3
#define OPERATION_BLINK_N_TIMES = 0
#define OPERATION_BLINK_IF_SYNC = 1

typedef struct {
  uint8_t payloadHeader;
  uint8_t *payloadBody;
  uint8_t *payloadHash;
  uint8_t *payloadMessage;
  uint8_t messageType;
  uint8_t operationType;
  unsigned int userId;
  unsigned int generatedAt;
  unsigned int syncTime;
  unsigned int debugBlink;
  unsigned int debugSyncTime;
  uint8_t *newKey;
  bool successfulDecoding;
  int bodyLength;
  int messageLength;
  bool needToAuthenticate;
} DecodedQRCodeData;

DecodedQRCodeData getQRCodeData(uint8_t *payload, int payloadLength);
void freeMallocData(DecodedQRCodeData *decodedQRCodeData);

uint8_t getPayloadHeader(uint8_t *payload);
uint8_t *getPayloadBody(uint8_t *payload, int payloadLength);
uint8_t *getPayloadHash(uint8_t *payload, int payloadLength);
uint8_t *getPayloadMessage(uint8_t *payload, int payloadLength);

uint8_t getMessageType(uint8_t header);
uint8_t getOperationType(uint8_t header);
unsigned int getUserId(uint8_t *body);
unsigned int getGeneratedAt(uint8_t *body);
unsigned int getSyncTime(uint8_t *body);
unsigned int getDebugBlink(uint8_t *body);
unsigned int getDebugSyncTime(uint8_t *body);
uint8_t *getNewKey(uint8_t *body);

void printDecodedQRCodeData(DecodedQRCodeData decodedQRCodeData);
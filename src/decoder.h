#include <stdint.h>
#include <iostream>

#define HEADER_LENGTH 1
#define HASH_LENGTH 20
#define USER_ID_LENGTH 4
#define DATETIME_LENGTH 4
#define INT_LENGTH 4

uint8_t getPayloadHeader(uint8_t *payload);
uint8_t *getPayloadBody(uint8_t *payload, int payloadSize);
uint8_t *getPayloadHash(uint8_t *payload, int payloadSize);

uint8_t getMessageType(uint8_t header);
uint8_t getOperationType(uint8_t header);
int getUserId(uint8_t *body);
int getGeneratedAt(uint8_t *body);
int getSyncTime(uint8_t *body);
int getDebugBlink(uint8_t *body);
int getDebugSyncTime(uint8_t *body);
uint8_t *getNewKey(uint8_t *body);
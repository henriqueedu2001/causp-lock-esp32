#include <decoder.h>

int getPayloadBodySize(uint8_t *payload, int payloadSize);

/**
 * @brief Extracts the header from the payload
 * @param payload The payload uint8_t array
 * @return The header of the payload
 */
uint8_t getPayloadHeader(uint8_t *payload) {
    return payload[0];
}

/**
 * @brief Extracts the body from the payload
 * @param payload The payload uint8_t array
 * @param payloadSize The payload size in bytes
 * @return The body of the payload
 */
uint8_t *getPayloadBody(uint8_t *payload, int payloadSize) {
    int bodySize = getPayloadBodySize(payload, payloadSize);
    uint8_t *body = (uint8_t *) malloc(bodySize * sizeof(uint8_t));
    for(int i = 0; i < bodySize; i++)
        body[i] = payload[i + HEADER_LENGTH];
    return body;
}

/**
 * @brief Extracts the hash from the payload
 * @param payload The payload uint8_t array
 * @param payloadSize The payload size in bytes
 * @return The hash of the payload
 */
uint8_t *getPayloadHash(uint8_t *payload, int payloadSize) {
    uint8_t *payloadHash = (uint8_t *) malloc(HASH_LENGTH * sizeof(uint8_t));
    for(int i = 0; i < HASH_LENGTH; i++)
        payloadHash[i] = payload[i + payloadSize - HASH_LENGTH];
    return payloadHash;
}

/**
 * @brief Extract the size of the QR Code payload body
 * @param payload The QR Code payload
 * @param payloadSize the size of the payload
 * @return The body size
 */
int getPayloadBodySize(uint8_t *payload, int payloadSize) {
    int bodySize = payloadSize - HEADER_LENGTH - HASH_LENGTH;
    return bodySize; 
}

/**
 * @brief Extracts the messageType from the QR Code payload header
 * @param header The payload header 
 * @return The messageType of the payload
 */
uint8_t getMessageType(uint8_t header) {
    uint8_t messageType = header >> 4;
    return messageType;
}

/**
 * @brief Extracts the operationType from the QR Code payload header
 * @param payload The payload header
 * @return The operationType of the payload
 */
uint8_t getOperationType(uint8_t header) {
    uint8_t operationType = header & 0x0F;
    return operationType;
}

/**
 * @brief Extracts the userId from the QR Code payload body
 * @param body The payload body
 * @return The userId of the payload body
 */
int getUserId(uint8_t *body) {
    int userId = 0;
    for(int i = 0; i < USER_ID_LENGTH; i++) {
        int byte = (int) body[i] << 8 * (USER_ID_LENGTH - i - 1);
        userId = userId | byte;
    }
    return userId;
}

/**
 * @brief Extracts the generatedAt datetime from the QR Code payload body
 * @param body The payload body
 * @return The generatedAt of the payload body
 */
int getGeneratedAt(uint8_t *body) {
    int generatedAt = 0;
    for(int i = 0; i < DATETIME_LENGTH; i++) {
        int byte = (int) body[i + USER_ID_LENGTH] << 8 * (USER_ID_LENGTH - i - 1);
        generatedAt = generatedAt | byte;
    }
    return generatedAt;
}

/**
 * @brief Extracts the generatedAt datetime from the QR Code payload body
 * @param body The payload body
 * @return The generatedAt of the payload body
 */
int getSyncTime(uint8_t *body) {
    int syncTime = 0;
    for(int i = 0; i < DATETIME_LENGTH; i++) {
        int byte = (int) body[i] << 8 * (DATETIME_LENGTH - i - 1);
        syncTime = syncTime | byte;
    }
    return syncTime;
}

/**
 * @brief Extracts the debugBlink from the QR Code payload body
 * @param body The payload body
 * @return The generatedAt of the payload body
 */
int getDebugBlink(uint8_t *body) {
    int debugBlink = 0;
    for(int i = 0; i < INT_LENGTH; i++) {
        int byte = (int) body[i] << 8 * (INT_LENGTH - i - 1);
        debugBlink = debugBlink | byte;
    }
    return debugBlink;
}

/**
 * @brief Extracts the debugSyncTime from the QR Code payload body
 * @param body The payload body
 * @return The debugSyncTime of the payload body
 */
int getDebugSyncTime(uint8_t *body) {
    int debugSyncTime = 0;
    for(int i = 0; i < DATETIME_LENGTH; i++) {
        int byte = (int) body[i] << 8 * (DATETIME_LENGTH - i - 1);
        debugSyncTime = debugSyncTime | byte;
    }
    return debugSyncTime;
}

/**
 * @brief Extracts the newKey from the QR Code payload body
 * @param body The payload body
 * @return The newKey of the payload body
 */
uint8_t *getNewKey(uint8_t *body) {
    uint8_t *newKey = (uint8_t *) malloc(HASH_LENGTH * sizeof(uint8_t));
    for(int i = 0; i < HASH_LENGTH; i++)
        newKey[i] = body[i];
    return newKey;
}
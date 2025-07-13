#include <decoder.h>

int getPayloadBodyLength(int payloadLength);

/**
 * @brief Decodes the raw QR Code payload
 * @param payload The raw QR Code payload uint8_t *buffer
 * @param payloadLength The payload Length
 * @return The DecodedQRCodeData struct
 */
DecodedQRCodeData getQRCodeData(uint8_t *payload, int payloadLength) {
    DecodedQRCodeData decodedQRCodeData = {
        0,    /* uint8_t payloadHeader  */
        NULL, /* uint8_t *payloadBody   */
        NULL, /* uint8_t *payloadHash   */
        0,    /* uint8_t messageType    */
        0,    /* uint8_t operationType  */
        0,    /* uint8_t userId         */
        0,    /* uint8_t generatedAt    */
        0,    /* uint8_t syncTime       */
        0,    /* uint8_t debugBlink     */
        0,    /* uint8_t debugSyncTime  */
        NULL, /* uint8_t *newKey        */
        false /* bool successfulDecoding    */
    };
    uint8_t payloadHeader = getPayloadHeader(payload);
    uint8_t *payloadBody = getPayloadBody(payload, payloadLength);
    uint8_t *payloadHash = getPayloadHash(payload, payloadLength);
    uint8_t messageType = getMessageType(payloadHeader);
    uint8_t operationType = getOperationType(payloadHeader);
    int bodyLength = getPayloadBodyLength(payloadLength);
    bool successfulDecoding = true;

    decodedQRCodeData.payloadHeader = payloadHeader;
    decodedQRCodeData.payloadBody = payloadBody;
    decodedQRCodeData.payloadHash = payloadHash;
    decodedQRCodeData.messageType = messageType;
    decodedQRCodeData.operationType = operationType;
    decodedQRCodeData.bodyLength = bodyLength;
    decodedQRCodeData.successfulDecoding = successfulDecoding;

    return decodedQRCodeData;
}

/**
 * @brief Frees the dinamically
 * @param decodedQRCodeData The DecodedQRCodeData struct
 */
void freeMallocData(DecodedQRCodeData *decodedQRCodeData) {
    free((*decodedQRCodeData).payloadBody);
    decodedQRCodeData->payloadBody = NULL;
    free((*decodedQRCodeData).payloadHash);
    decodedQRCodeData->payloadHash = NULL;
    free((*decodedQRCodeData).newKey);
    decodedQRCodeData->newKey = NULL;
}

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
 * @param payloadLength The payload size in bytes
 * @return The body of the payload
 */
uint8_t *getPayloadBody(uint8_t *payload, int payloadLength) {
    int bodySize = getPayloadBodyLength(payloadLength);
    uint8_t *body = (uint8_t *) malloc(bodySize * sizeof(uint8_t));
    for(int i = 0; i < bodySize; i++)
        body[i] = payload[i + HEADER_LENGTH];
    return body;
}

/**
 * @brief Extracts the hash from the payload
 * @param payload The payload uint8_t array
 * @param payloadLength The payload size in bytes
 * @return The hash of the payload
 */
uint8_t *getPayloadHash(uint8_t *payload, int payloadLength) {
    uint8_t *payloadHash = (uint8_t *) malloc(HASH_LENGTH * sizeof(uint8_t));
    for(int i = 0; i < HASH_LENGTH; i++)
        payloadHash[i] = payload[i + payloadLength - HASH_LENGTH];
    return payloadHash;
}

/**
 * @brief Extract the length of the QR Code payload body
 * @param payload The QR Code payload
 * @param payloadLength the length of the payload
 * @return The body length
 */
int getPayloadBodyLength(int payloadLength) {
    int bodySize = payloadLength - HEADER_LENGTH - HASH_LENGTH;
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

/**
 * @brief Prints the QR Code decoded data
 * @param decodedQRCodeData the DecodedQRCodeData struct
 */
void printDecodedQRCodeData(DecodedQRCodeData decodedQRCodeData) {
    uint8_t payloadHeader = decodedQRCodeData.payloadHeader;
    uint8_t *payloadBody = decodedQRCodeData.payloadBody;
    uint8_t *payloadHash = decodedQRCodeData.payloadHash;
    uint8_t messageType = decodedQRCodeData.messageType;
    uint8_t operationType = decodedQRCodeData.operationType;
    int userId = decodedQRCodeData.userId;
    int generatedAt = decodedQRCodeData.generatedAt;
    int syncTime = decodedQRCodeData.syncTime;
    int debugBlink = decodedQRCodeData.debugBlink;
    int debugSyncTime = decodedQRCodeData.debugSyncTime;
    uint8_t *newKey = decodedQRCodeData.newKey;
    bool successfulRead = decodedQRCodeData.successfulDecoding;
    int bodyLength = decodedQRCodeData.bodyLength;

    if(successfulRead == false) {
      Serial.println("Unsuccessful QR Code read");
      return;
    } else {
      Serial.println("QR Code decoded data:");
    }

    Serial.print("payloadHeader: ");
    Serial.println(payloadHeader, HEX);

    if(payloadBody != NULL) {
        Serial.print("payloadBody: ");
        for(int i = 0; i < bodyLength; i++) {
            Serial.print(payloadBody[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }

    if(payloadHash != NULL) {
        Serial.print("payloadHash: ");
        for(int i = 0; i < HASH_LENGTH; i++) {
            Serial.print(payloadHash[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}
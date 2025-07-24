#include <decoder.h>

int getPayloadBodyLength(int payloadLength);
int getPayloadMessageLength(int payloadLength);

uint8_t *rawPayloadPtr;
int rawPayloadLength;

/**
 * @brief Decodes the raw QR Code payload
 * @param payload The raw QR Code payload uint8_t *buffer
 * @param payloadLength The payload Length
 * @return The DecodedQRCodeData struct
 */
DecodedQRCodeData getQRCodeData(uint8_t *payload, int payloadLength) {
    DecodedQRCodeData decodedQRCodeData = {
        0,     /* uint8_t payloadHeader      */
        NULL,  /* uint8_t *payloadBody       */
        NULL,  /* uint8_t *payloadHash       */
        NULL,  /* uint8_t *payloadMessage    */
        0,     /* uint8_t messageType        */
        0,     /* uint8_t operationType      */
        0,     /* unsigned int userId        */
        0,     /* unsigned int generatedAt   */
        0,     /* unsigned int syncTime      */
        0,     /* unsigned int debugBlink    */
        0,     /* unsigned int debugSyncTime */
        NULL,  /* uint8_t *newKey            */
        false, /* bool successfulDecoding    */
        0,     /* int bodyLength             */
        0,     /* int messageLength          */
        false, /* bool needToAuthenticate    */
    };
    /* raw qr code payload data */
    rawPayloadPtr = payload;
    rawPayloadLength = payloadLength;
    
    /* message type independent fields */
    uint8_t payloadHeader = getPayloadHeader(payload);
    uint8_t *payloadBody = getPayloadBody(payload, payloadLength);
    uint8_t *payloadHash = NULL;
    uint8_t *payloadMessage = getPayloadMessage(payload, payloadLength);
    uint8_t messageType = getMessageType(payloadHeader);
    uint8_t operationType = getOperationType(payloadHeader);
    bool needToAuthenticate = false;

    /* extracts the hash only if the message */
    if(messageType == MESSAGE_TYPE_ACCESS || messageType == MESSAGE_TYPE_SYNC || messageType == MESSAGE_TYPE_CONFIG) {
        payloadHash = getPayloadHash(payload, payloadLength);
        needToAuthenticate = true;
    }

    /* message type dependent fields */
    unsigned int userId = 0;
    unsigned int generatedAt = 0;
    unsigned int syncTime = 0;
    unsigned int debugBlink = 0;
    unsigned int debugSyncTime = 0;
    uint8_t *newKey = NULL;

    /* only extracts the important data from the body, based on the messageType */
    switch(messageType) {
        case MESSAGE_TYPE_ACCESS:
            userId = getUserId(payloadBody);
            generatedAt = getGeneratedAt(payloadBody);
            break;
        case MESSAGE_TYPE_SYNC:
            syncTime = getSyncTime(payloadBody);
            break;
        case MESSAGE_TYPE_CONFIG:
            newKey = getNewKey(payloadBody);
            break;
        case MESSAGE_TYPE_DEBUG:
            switch(operationType) {
                case 0:
                    debugBlink = getDebugBlink(payloadBody);
                    break;
                case 1:
                    debugBlink = getDebugBlink(payloadBody);
            }
            break;
    }

    /* metadata */
    int bodyLength = getPayloadBodyLength(payloadLength);
    int messageLength = getPayloadMessageLength(payloadLength);
    bool successfulDecoding = true;

    /* writing the decoded data in the decodedQRCodeData struct */
    decodedQRCodeData.payloadHeader = payloadHeader;
    decodedQRCodeData.payloadBody = payloadBody;
    decodedQRCodeData.payloadHash = payloadHash;
    decodedQRCodeData.payloadMessage = payloadMessage;
    decodedQRCodeData.messageType = messageType;
    decodedQRCodeData.operationType = operationType;
    decodedQRCodeData.successfulDecoding = successfulDecoding;
    decodedQRCodeData.userId = userId;
    decodedQRCodeData.generatedAt = generatedAt;
    decodedQRCodeData.syncTime = syncTime;
    decodedQRCodeData.debugBlink = debugBlink;
    decodedQRCodeData.debugSyncTime = debugSyncTime;
    decodedQRCodeData.newKey = newKey;
    decodedQRCodeData.bodyLength = bodyLength;
    decodedQRCodeData.messageLength = messageLength;
    decodedQRCodeData.needToAuthenticate = needToAuthenticate;

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
    free((*decodedQRCodeData).payloadMessage);
    decodedQRCodeData->payloadMessage = NULL;
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
 * @brief Extracts the message (header + body) from the payload
 * @param payload The payload uint8_t array
 * @param payloadLength The payload size in bytes
 * @return The payload message
 */
uint8_t *getPayloadMessage(uint8_t *payload, int payloadLength) {
    int messageLength = getPayloadMessageLength(payloadLength);
    uint8_t *payloadMessage = (uint8_t *) malloc((messageLength) * sizeof(uint8_t));
    for(int i = 0; i < messageLength; i++)
        payloadMessage[i] = payload[i];
    return payloadMessage;
}

/**
 * @brief Extract the length of the QR Code payload body
 * @param payloadLength the length of the payload
 * @return The body length
 */
int getPayloadBodyLength(int payloadLength) {
    int bodyLength = payloadLength - HEADER_LENGTH - HASH_LENGTH;
    if(bodyLength <= 0) bodyLength = payloadLength - HEADER_LENGTH;
    return bodyLength; 
}

/**
 * @brief Extract the length of the QR Code payload message (header + body)
 * @param payloadLength the length of the payload
 * @return The message length
 */
int getPayloadMessageLength(int payloadLength) {
    int messageLength = payloadLength - HASH_LENGTH;
    if(messageLength <= 0) messageLength = payloadLength;
    return messageLength; 
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
unsigned int getUserId(uint8_t *body) {
    unsigned int userId = 0;
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
unsigned int getGeneratedAt(uint8_t *body) {
    unsigned int generatedAt = 0;
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
unsigned int getSyncTime(uint8_t *body) {
    unsigned int syncTime = 0;
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
unsigned int getDebugBlink(uint8_t *body) {
    unsigned int debugBlink = 0;
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
unsigned int getDebugSyncTime(uint8_t *body) {
    unsigned int debugSyncTime = 0;
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
    uint8_t *payloadMessage = decodedQRCodeData.payloadMessage;
    uint8_t messageType = decodedQRCodeData.messageType;
    uint8_t operationType = decodedQRCodeData.operationType;
    unsigned int userId = decodedQRCodeData.userId;
    unsigned int generatedAt = decodedQRCodeData.generatedAt;
    unsigned int syncTime = decodedQRCodeData.syncTime;
    unsigned int debugBlink = decodedQRCodeData.debugBlink;
    unsigned int debugSyncTime = decodedQRCodeData.debugSyncTime;
    uint8_t *newKey = decodedQRCodeData.newKey;
    bool successfulRead = decodedQRCodeData.successfulDecoding;
    int bodyLength = decodedQRCodeData.bodyLength;
    int messageLength = decodedQRCodeData.messageLength;

    if(successfulRead == false) {
      Serial.println("Unsuccessful QR Code read");
      return;
    } else {
      Serial.println("QR Code decoded data:");
    }

    Serial.print("rawPayload: ");
    if(rawPayloadPtr != NULL) {
        for(int i = 0; i < rawPayloadLength; i++) {
            Serial.print(rawPayloadPtr[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    } else {
        Serial.println("NULL");
    }

    Serial.print("payloadHeader: ");
    Serial.println(payloadHeader, HEX);

    Serial.print("payloadBody: ");
    if(payloadBody != NULL) {
        for(int i = 0; i < bodyLength; i++) {
            Serial.print(payloadBody[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    } else {
        Serial.println("NULL");
    }

    Serial.print("payloadHash: ");
    if(payloadHash != NULL) {
        for(int i = 0; i < HASH_LENGTH; i++) {
            Serial.print(payloadHash[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    } else {
        Serial.println("NULL");
    }

    Serial.print("payloadMessage: ");
    if(payloadMessage != NULL) {
        for(int i = 0; i < bodyLength + HEADER_LENGTH; i++) {
            Serial.print(payloadMessage[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    } else {
        Serial.println("NULL");
    }

    Serial.print("messageType: ");
    Serial.println(messageType);
    Serial.print("operationType: ");
    Serial.println(operationType);
    Serial.print("userId: ");
    Serial.println(userId);
    Serial.print("generatedAt: ");
    Serial.println(generatedAt);
    Serial.print("syncTime: ");
    Serial.println(syncTime);
    Serial.print("debugBlink: ");
    Serial.println(debugBlink);
    Serial.print("debugSyncTime: ");
    Serial.println(debugSyncTime);

    Serial.print("newKey: ");
    if(newKey != NULL) {
        for(int i = 0; i < NEW_KEY_LENGTH; i++) {
            Serial.print(newKey[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    } else {
        Serial.println("NULL");
    }

    Serial.print("bodyLength: ");
    Serial.println(bodyLength);
    Serial.print("messageLength: ");
    Serial.println(messageLength);
}
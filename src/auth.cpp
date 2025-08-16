#include <auth.h>
#include <Arduino.h>

#define SHA1_HASH_LENGTH 20

uint8_t DEFAULT_ACCESS_KEY[20] = {
  0x85, 0xf1, 0xe2, 0x04, 0xba,
  0x63, 0xfe, 0x41, 0xa0, 0xf0,
  0xda, 0x37, 0x74, 0x3e, 0x8d,
  0x1c, 0x6a, 0xf5, 0x33, 0xfc
};

uint8_t DEFAULT_SYNC_KEY[20] = {
  0xbf, 0x42, 0x9e, 0x35, 0x29,
  0xc5, 0xf1, 0x4e, 0xbb, 0x81,
  0x8c, 0x15, 0xa3, 0xcd, 0x98,
  0x04, 0xf6, 0x1d, 0x4b, 0x98
};

uint8_t DEFAULT_MASTER_KEY[20] = {
  0x9f, 0x96, 0x5e, 0x25, 0xbb,
  0xba, 0x22, 0xeb, 0x9e, 0x3f,
  0xa1, 0x32, 0x98, 0x11, 0x92,
  0x1e, 0xe0, 0xd9, 0xb2, 0x2e
};

uint8_t DEFAULT_CONFIG_KEY[20] = {
  0x79, 0x31, 0x8f, 0x33, 0x5e,
  0x6b, 0xf5, 0x37, 0xb9, 0xb6,
  0x2e, 0x56, 0xac, 0x54, 0xf8,
  0x36, 0xf4, 0x58, 0xf9, 0xdb
};

bool validateMessage(uint8_t *message, uint8_t *hash, int messageLength, uint8_t messageType) {
  bool validity = false;
  uint8_t computedHash[SHA1_HASH_LENGTH];
  uint8_t *key;

  switch(messageType) {
    case 0:
      /* MESSAGE_TYPE = ACCESS */
      key = DEFAULT_ACCESS_KEY;
      break;
    case 1:
      /* MESSAGE_TYPE = SYNC */
      key = DEFAULT_SYNC_KEY;
      break;
    case 2:
      /* MESSAGE_TYPE = CONFIG */
      key = DEFAULT_CONFIG_KEY;
      break;
    case 3:
      /* MESSAGE_TYPE = DEBUG */
      return true;
      break;
  }

  /* validating the hashes */
  getHMAC_SHA1(key, KEY_LENGTH, message, messageLength, computedHash);
  validity = validateSignature(hash, computedHash);

  /* trying the master key, if the previous keys failed */
  if(!validity) {
    key = DEFAULT_MASTER_KEY;
    getHMAC_SHA1(key, KEY_LENGTH, message, messageLength, computedHash);
    validity = validateSignature(hash, computedHash);
  }

  return validity;
}

/**
 * @brief Computes the signature of the message with the HMAC-SHA1 algorithm
 * @param key The secret key
 * @param keyLength The length of the key
 * @param message The payload message (header + body)
 * @param [out] outputHMAC The computed HMAC-SHA1 hash
 */
void getHMAC_SHA1(
  const uint8_t *key,
  int keyLength,
  const uint8_t *message,
  int messageLength,
  uint8_t *outputHMAC
) {
    const mbedtls_md_info_t *md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
    mbedtls_md_context_t ctx;
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, md_info, 1);
    mbedtls_md_hmac_starts(&ctx, key, keyLength);
    mbedtls_md_hmac_update(&ctx, message, messageLength);
    mbedtls_md_hmac_finish(&ctx, outputHMAC);
    mbedtls_md_free(&ctx);
}

/**
 * @brief Verifies if the HMAC-SHA1 signatures match
 * @param informedHash The informed hash
 * @param computedHash The computed hash
 */
bool validateSignature(uint8_t *informedHash, uint8_t *computedHash) {
  bool validity = true;
  /* no breaks to prevent timing attacks */
  for(int i = 0; i < SHA1_HASH_LENGTH; i++) {
    if(informedHash[i] != computedHash[i])
      validity = false;
  }
  return validity;
}

/**
 * @brief Prints the HMAC-SHA1 signature
 * @param hash The HMAC-SHA1 hash
 */
void printHMACSHA1(uint8_t *hash) {
  Serial.print("computed hash: ");
  for(int i = 0; i < SHA1_HASH_LENGTH; i++) {
    Serial.print(hash[i], HEX);
    Serial.print(" ");
  }
  Serial.print("\n");
}

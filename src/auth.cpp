#include <auth.h>
#include <Arduino.h>

#define SHA1_HASH_LENGTH 20

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

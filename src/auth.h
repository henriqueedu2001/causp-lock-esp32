#include "mbedtls/md.h"

#define KEY_LENGTH 20 

void getHMAC_SHA1(
  const uint8_t *key,
  int key_length,
  const uint8_t *message,
  int message_length,
  uint8_t *output_hmac
);
bool validateSignature(uint8_t *informedHash, uint8_t *computedHash);
void printHMACSHA1(uint8_t *hash);
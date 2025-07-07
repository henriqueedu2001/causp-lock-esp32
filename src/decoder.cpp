#include <decoder.h>

char *extractMsgType(char *payload);
char *extractSyncTime(char *payload);
char *extractNewPublicKey(char *payload);
char *extractBlinkNum(char *payload);
char *extractUserId(char *payload);
char *extractUserName(char *payload);
char *extractUserType(char *payload);

DecodedQRCodeData decode(char *payload) {
  DecodedQRCodeData qrcodeData;
  qrcodeData.msgType = extractMsgType(payload);
  qrcodeData.syncTime = extractSyncTime(payload);
  qrcodeData.newPublicKey = extractNewPublicKey(payload);
  qrcodeData.blinkNum = extractBlinkNum(payload);
  qrcodeData.userId = extractUserId(payload);
  qrcodeData.userName = extractUserName(payload);
  qrcodeData.userType = extractUserType(payload);
  return qrcodeData;
}

char *extractMsgType(char *payload) {
  return "not implemented";
}

char *extractSyncTime(char *payload) {
  return "not implemented";
}

char *extractNewPublicKey(char *payload) {
  return "not implemented";
}

char *extractBlinkNum(char *payload) {
  return "not implemented";
}

char *extractUserId(char *payload) {
  return "not implemented";
}

char *extractUserName(char *payload) {
  return "not implemented";
}

char *extractUserType(char *payload) {
  return "not implemented";
}

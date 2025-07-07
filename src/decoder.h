typedef struct {
  char *msgType;
  char *syncTime;
  char *newPublicKey;
  char *blinkNum;
  char *userId;
  char *userName;
  char *userType;
} DecodedQRCodeData;

DecodedQRCodeData decode(char *payload);
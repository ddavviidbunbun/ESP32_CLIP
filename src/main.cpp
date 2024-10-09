#include "CLIPmyBLE/CLIPmyBLESource.hpp"
#include "CLIPmyPN532/CLIPmyPN532Source.hpp"

void readTagIDCLIP(void);

bool deviceConnected = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("System initialized");

  initBLECLIP(deviceConnected);
  initNFCPN532CLIP(nfc);
}

void loop()
{
  if (deviceConnected && cState)
  {
    readTagIDCLIP();
    if (isMyTag(tagId, newTagIDs) && !(isMyTag(tagId, oldTagIDs)))
    {
      if (sendMSGBLECLIP(tagId))
      {
        registerID(tagId, oldTagIDs, oldCount);
        tone(BUZZER_PIN, NOTE_E1, 100);
      }
    }
    tagId = "";
    nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
    cState = false;
  }
}

void readTagIDCLIP(void)
{
  if (readTagID(uid, &uidLength, nfc))
  {
    Serial.println("Found a card!");
    // Serial.print("UID Length: ");
    // Serial.print(uidLength, DEC);
    // Serial.println(" bytes");
    // Serial.print("UID Value: ");
    tagToByte(nuidPICC, uid, uidLength);
    tagToString(nuidPICC, tagId);
    registerID(tagId, newTagIDs, newCount);
    cleanVarUID(nuidPICC, uid, uidLength);

    Serial.println(tagId);
    if (isMyTag(tagId, newTagIDs))
    {
      Serial.println("This is my Tag ID");
      // tone(BUZZER_PIN, NOTE_E1, 100);
      delay(100);
    }
    else
    {
      Serial.println("This is NOT my Tag ID");
      tagId = "NONE";
    }
  }
  else
  {
    Serial.println("MATI");
  }
}
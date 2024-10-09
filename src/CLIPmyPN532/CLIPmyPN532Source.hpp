#include "CLIPmyPN532.hpp"

void readInterrupt()
{
  cState = true;
}

void initNFCPN532CLIP(PN532 &nfcTemp)
{
  // Begin NFC, CONNECTING PN532 TO ESP32
  nfcTemp.begin();
  attachInterrupt(digitalPinToInterrupt(PN532IRQPIN), readInterrupt, FALLING);
  nfcTemp.SAMConfig();
  u_int32_t version = nfcTemp.getFirmwareVersion();
  Serial.println(version);
}

bool readTagID(uint8_t uidtemp[], uint8_t *uidLengthtemp, PN532 &nfcTemp)
{
  boolean success;
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfcTemp.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uidtemp[0], uidLengthtemp);
  return success;
}

void tone(int pin, int frequency, int duration)
{
  ledcSetup(SOUND_PWM_CHANNEL, frequency, SOUND_RESOLUTION); // Set up PWM channel
  ledcAttachPin(pin, SOUND_PWM_CHANNEL);                     // Attach channel to pin
  ledcWrite(SOUND_PWM_CHANNEL, SOUND_ON);
  delay(duration);
  ledcWrite(SOUND_PWM_CHANNEL, SOUND_OFF);
}

void cleanVarUID(byte nuidPICCTemp[], uint8_t uidtemp[], uint8_t &uidLengthtemp)
{
  for (uint8_t i = 0; i < uidLengthtemp; i++)
  {
    uidtemp[i] = 0;
    nuidPICCTemp[i] = 0;
  }
  uidLengthtemp = 0;
}

void tagToByte(byte nuidPICCtemp[], uint8_t uidtemp[], uint8_t uidLengthtemp)
{
  for (uint8_t i = 0; i < uidLengthtemp; i++)
  {
    Serial.print(" 0x");
    Serial.print(uidtemp[i]);
    nuidPICCtemp[i] = uidtemp[i];
  }
  Serial.println("");
}

void tagToString(byte nuidPICCtemp[], String &tagId)
{
  for (int i = 0; i < 8; i++)
  {
    if (i < 7)
      tagId += String(nuidPICCtemp[i]) + ".";
    else
      tagId += String(nuidPICCtemp[i]);
  }
}

void registerID(String id, String storageIDs[], uint8_t &count)
{
  if (count == MAXSPACE_TAGIDS)
    return; // Not Enough Space
  for (uint8_t i = 0; i <= count; i++)
  {
    if (storageIDs[i] == id)
    {
      Serial.println("ID has already been Registered");
      return;
    }
  }
  storageIDs[count] = id;
  // Serial.print("Before = ");
  // Serial.print(count);
  count++;
  // Serial.print(" ; After = ");
  // Serial.print(count);
  Serial.println("ID has been Registered");
}

bool isMyTag(String id, String tagIDs[])
{
  for (int i = 0; i < MAXSPACE_TAGIDS; i++)
  {
    String x = tagIDs[i];
    if (x == id)
      return true;
  }
  return false;
}
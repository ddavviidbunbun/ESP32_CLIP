#include "CLIPmyPN532.hpp"

void initNFCPN532CLIP(PN532 &nfcTemp)
{
  // Begin NFC, CONNECTING PN532 TO ESP32
  nfcTemp.begin();
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

void toneBuzzer(int durations[], int melodies[], int size)
{
  for (int thisNote = 0; thisNote < size; thisNote++)
  {
    // to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / durations[thisNote];
    tone(BUZZER_PIN, melodies[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
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
  count++;
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

bool readTagIDCLIP(bool &isRead)
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
    isRead = true;
    return true;
  }
  isRead = false;
  return false;
}
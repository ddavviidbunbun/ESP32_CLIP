#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include "pitches.h"

#define SS 5
#define SIZE_TAG_ID_PN532 8
#define MAXSPACE_TAGIDS 8
#define BUZZER_PIN 17
#define SOUND_PWM_CHANNEL 0
#define SOUND_RESOLUTION 8 // Numbers of Bit of Resolution
#define SOUND_ON (1 << (SOUND_RESOLUTION - 1))
#define SOUND_OFF 0

void initNFCPN532CLIP(PN532 &nfcTemp);
void registerID(String id, String storageIDs[], uint8_t &count);
bool isMyTag(String id, String tagIDs[]);
bool readTagID(uint8_t uidtemp[], uint8_t *uidLengthtemp, PN532 &nfcTemp);
void tone(int pin, int frequency, int duration);
void cleanVarUID(byte nuidPICCTemp[], uint8_t uidtemp[], uint8_t &uidLengthtemp);
void tagToByte(byte nuidPICCtemp[], uint8_t uidtemp[], uint8_t uidLengthtemp);
void tagToString(byte nuidPICCtemp[], String &tagId);
bool readTagIDCLIP(bool &isRead);

PN532_SPI pn532spi(SPI, SS); // create a PN532 SPI interface with the SPI CS terminal located at digital pin 10
PN532 nfc(pn532spi);

byte nuidPICC[SIZE_TAG_ID_PN532];
String newTagIDs[MAXSPACE_TAGIDS];
String oldTagIDs[MAXSPACE_TAGIDS];
String tagId = "";
uint8_t newCount = 0;
uint8_t oldCount = 0;
uint8_t uid[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
uint8_t uidLength;                         // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
int noteDurations[] = {
    8, 8, 4,
    8, 8, 4,
    8, 8, 8, 8,
    2,
    8, 8, 8, 8,
    8, 8, 8, 16, 16,
    8, 8, 8, 8,
    4, 4};
int melody[] = {
    NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
    NOTE_E5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
    NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
    NOTE_D5, NOTE_G5};
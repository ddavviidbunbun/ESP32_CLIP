#include "CLIPmyBLESource.hpp"
#include "CLIPmyPN532Source.hpp"
#include "driver/rtc_io.h"

#define timePerTick 1000000 // 1 second
#define timeToSleep 40      // 120 second
#define WAKEUP_GPIO 4

hw_timer_t *timer0 = NULL;
bool isConnectedBLE = false;
bool isReadTag = false;
uint count = 0;

void IRAM_ATTR Timer0_ISR()
{
  if (!isReadTag)
  {
    count++;
  }
  else
  {
    count = 0;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("System initialized");
  initFMCLIP();
  initBLECLIP(isConnectedBLE, readFMCLIP(true));
  initNFCPN532CLIP(nfc);

  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &Timer0_ISR, true);
  timerAlarmWrite(timer0, timePerTick, true);
  timerAlarmEnable(timer0);

  pinMode(WAKEUP_GPIO, INPUT_PULLUP);
}

void loop()
{
  if (isConnectedBLE && readTagIDCLIP(isReadTag))
  {
    if (isMyTag(tagId, newTagIDs) && !(isMyTag(tagId, oldTagIDs)))
    {
      if (sendMSGBLECLIP(tagId))
      {
        registerID(tagId, oldTagIDs, oldCount);
        int size = sizeof(noteDurations) / sizeof(int);
        for (int thisNote = 0; thisNote < size; thisNote++)
        {

          // to calculate the note duration, take one second divided by the note type.
          // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(BUZZER_PIN, melody[thisNote], noteDuration);

          // to distinguish the notes, set a minimum time between them.
          // the note's duration + 30% seems to work well:
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
        }
      }
    }
    tagId = "";
  }

  if (count == timeToSleep)
  {
    count = 0;
    Serial.println("X seconds have passed");
    esp_sleep_enable_ext0_wakeup((gpio_num_t)WAKEUP_GPIO, LOW);
    delay(1000);
    Serial.println("Started to Sleep");
    esp_deep_sleep_start();
  }
  if (digitalRead(WAKEUP_GPIO) == LOW)
  {
    delay(100);
    if (digitalRead(WAKEUP_GPIO) == LOW)
    {
      esp_sleep_enable_ext0_wakeup((gpio_num_t)WAKEUP_GPIO, LOW);
      delay(200);
      Serial.println("Started to Sleep");
      esp_deep_sleep_start();
    }
  }
}
#pragma once
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <Preferences.h>

#define SERVICE_UUID "65ed019f-49f7-4267-9b56-e90c2fd4b3e5"
#define CHARACTERISTIC_UUID "cfdf6ed8-a098-440b-a5a7-56496868a4de"

#define NAME_SPACE_FM "PrivateData"
#define KEY_1 "BLEName"
#define KEY_2 "MACAdd"
#define DEFAULT_KEY_1 "CLIP-123"
#define DEFAULT_KEY_2 "123456789"

Preferences preferences;
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

void initBLECLIP(bool &deviceConnected, String nameBLE);
bool sendMSGBLECLIP(String msg);
void initFMCLIP(void);
String readFMCLIP(bool check);

class MyCLIPServerCallBack : public BLEServerCallbacks
{
    void onConnect(BLEServer *server);
    void onDisconnect(BLEServer *server);

public:
    bool &isConnected;
    MyCLIPServerCallBack(bool &deviceConnected);
};

class MyCLIPCharacteristicCallBack : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *characteristic);

public:
    void writeFMCLIP(bool check, String value);
};
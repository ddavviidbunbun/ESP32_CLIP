#pragma once
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Arduino.h>

#define SERVICE_UUID "65ed019f-49f7-4267-9b56-e90c2fd4b3e5"
#define CHARACTERISTIC_UUID "cfdf6ed8-a098-440b-a5a7-56496868a4de"

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

void initBLECLIP(bool &deviceConnected);
bool sendMSGBLECLIP(String msg);

class MyCLIPServerCallBack : public BLEServerCallbacks
{
    void onConnect(BLEServer *server);
    void onDisconnect(BLEServer *server);

public:
    bool &isConnected;
    MyCLIPServerCallBack(bool &deviceConnected);
};

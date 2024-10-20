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
#define KEY_2 "imeiCLIP"
#define DEFAULT_KEY_1 "CLIP123"
#define DEFAULT_KEY_2 "1234567"

Preferences preferences;
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;

enum statusCode
{
    NONE,
    OPEN,
    CLOSE,
    WRITE
};

void initBLECLIP(bool &deviceConnected, bool &deviceNotify, String nameBLE);
bool sendMSGBLECLIP(String msg);
void sendREStoFE(String status);
void initFMCLIP(void);
String readFMCLIP(bool check);
String stdstringToString(std::string value);

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
    bool &isNotify;
    MyCLIPCharacteristicCallBack(bool &deviceNotify);
    void splitString(std::string arr[], std::string val, std::string delimiter);
    void writeFMCLIP(std::string value);
    statusCode whatIsTheStatus(std::string value);
    void allowNotify(void);
    void disallowNotify(void);
};
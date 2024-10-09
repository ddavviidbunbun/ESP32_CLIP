#include "CLIPmyBLE.hpp"

MyCLIPServerCallBack::MyCLIPServerCallBack(bool &deviceConnected) : isConnected(deviceConnected)
{
}

void MyCLIPServerCallBack::onConnect(BLEServer *server)
{
  isConnected = true;
  Serial.println("Connected");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
};

void MyCLIPServerCallBack::onDisconnect(BLEServer *server)
{
  isConnected = false;
  Serial.println("Disconnected");
  BLEDevice::startAdvertising();
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
};

void initBLECLIP(bool &deviceConnected)
{
  // BLE Init
  BLEDevice::init("CLIP");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCLIPServerCallBack(deviceConnected));
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  pinMode(LED_BUILTIN, OUTPUT);
  delay(200);
}

bool sendMSGBLECLIP(String msg)
{
  if (msg != "NONE" && msg != "TIMED_OUT")
  {
    pCharacteristic->setValue(msg.c_str());
    pCharacteristic->notify();
    return true;
  }
  return false;
}
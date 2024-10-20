#include "CLIPmyBLE.hpp"

MyCLIPServerCallBack::MyCLIPServerCallBack(bool &deviceConnected) : isConnected(deviceConnected) {}

MyCLIPCharacteristicCallBack::MyCLIPCharacteristicCallBack(bool &deviceNotify) : isNotify(deviceNotify) {}

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

void MyCLIPCharacteristicCallBack::onWrite(BLECharacteristic *characteristic)
{
  std::string value = characteristic->getValue();
  std::string val[2];
  splitString(val, value, "#");
  switch (whatIsTheStatus(val[0]))
  {
  case OPEN:
    allowNotify();
    sendMSGBLECLIP("OPEN#" + readFMCLIP(false));
    break;
  case CLOSE:
    disallowNotify();
    break;
  case WRITE:
    Serial.println(stdstringToString(val[1]));
    writeFMCLIP(val[1]);
    sendMSGBLECLIP("WRITE");
    break;
  default:
    break;
  }
  delay(100);
  return;
}

void MyCLIPCharacteristicCallBack::splitString(std::string arr[], std::string val, std::string delimiter)
{
  int start, end = -1 * delimiter.size(), i = 0;
  do
  {
    start = end + delimiter.size();
    end = val.find(delimiter, start);
    arr[i] = val.substr(start, end - start);
    i++;
  } while (end != -1 && i != 2);
}

statusCode MyCLIPCharacteristicCallBack::whatIsTheStatus(std::string value)
{
  if (value.compare("OPEN") == 0)
    return OPEN;
  if (value.compare("CLOSE") == 0)
    return CLOSE;
  if (value.compare("WRITE") == 0)
    return WRITE;
  return NONE;
}

void MyCLIPCharacteristicCallBack::allowNotify(void)
{
  isNotify = true;
}

void MyCLIPCharacteristicCallBack::disallowNotify(void)
{
  isNotify = false;
}

void MyCLIPCharacteristicCallBack::writeFMCLIP(std::string value)
{
  preferences.begin(NAME_SPACE_FM, false);
  preferences.putString(KEY_1, stdstringToString(value));
  preferences.end();
}

void initBLECLIP(bool &deviceConnected, bool &deviceNotify, String nameBLE)
{
  // BLE Init
  BLEDevice::init(nameBLE.c_str());
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCLIPServerCallBack(deviceConnected));
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
  pCharacteristic->setCallbacks(new MyCLIPCharacteristicCallBack(deviceNotify));
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();
  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);
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

void initFMCLIP(void)
{

  preferences.begin(NAME_SPACE_FM, false);
  if (preferences.isKey(KEY_1))
  {
    Serial.println("udah ada key 1");
  }
  else
  {
    preferences.putString(KEY_1, DEFAULT_KEY_1);
  }
  if (preferences.isKey(KEY_2))
  {
    Serial.println("udah ada key 2");
  }
  else
  {
    preferences.putString(KEY_2, DEFAULT_KEY_2);
  }
  preferences.end();
  Serial.println(readFMCLIP(true));
  Serial.println(readFMCLIP(false));
}

String readFMCLIP(bool check)
{
  preferences.begin(NAME_SPACE_FM, true);
  String res;
  if (check)
    res = preferences.getString(KEY_1);
  else
    res = preferences.getString(KEY_2);
  preferences.end();
  return res;
}

String stdstringToString(std::string value)
{
  String val = String(value.c_str());
  return val;
}

#include <Arduino.h>

#include <BLEPeripheral.h>
#include <iBeacon.h>

#define BLE_REQ   10
#define BLE_RDY   2
#define BLE_RST   9

// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);
// create one or more services
BLEService service = BLEService("00000000-0000-4000-A000-000000000000");

// create one or more characteristics
BLECharCharacteristic characteristic = BLECharCharacteristic("00000000-0000-4000-A000-000000000001", BLERead | BLEWrite);
BLECharCharacteristic characteristic2 = BLECharCharacteristic("00000000-0000-4000-A000-000000000002", BLERead | BLEWrite);

// create one or more descriptors (optional)
//BLEDescriptor descriptor = BLEDescriptor("2901", "value");



void LEDsetup()
{
  pinMode(21, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
}
// 4bitの数値を出力します
void LEDout(int value)
{
  digitalWrite(21, (value >> 0) & 1);
  digitalWrite(4, (value >> 1) & 1);
  digitalWrite(6, (value >> 2) & 1);
  digitalWrite(5, (value >> 3) & 1);
}
void written(BLECentral& central, BLECharacteristic& characteristic){
        LEDout(characteristic.value()[0]);
        characteristic2.setValue(characteristic.value()[0]+20);
}
  BLECentral* central=nullptr;

void setup()
{
  LEDsetup();
//   char* uuid                   = "a196c876-de8c-4c47-ab5a-d7afd5ae7127";
//   unsigned short major         = 0;
//   unsigned short minor         = 0;
//   unsigned short measuredPower = -55;

//   // beacon.begin(uuid, major, minor, measuredPower);
//   blePeripheral.setLocalName("local-name"); // optional
// //  blePeripheral.setAdvertisedServiceUuid(service.uuid()); // optional

//   // add attributes (services, characteristics, descriptors) to peripheral
//   blePeripheral.addAttribute(service);
//   blePeripheral.addAttribute(characteristic);
//   blePeripheral.addAttribute(characteristic2);
//   characteristic.setEventHandler(BLEWritten,written);
// //  blePeripheral.addAttribute(descriptor);

//   // set initial value
//   characteristic.setValue(0);

//   // begin initialization
//   blePeripheral.begin();




  }
void loop()
{
    __WFE(); //①スリープ命令(wait for event)挿入
 //   beacon.loop();

}  
extern "C" {
int k=0;
void RTC1_IRQHandler(void)
{
  overflow();
   k++;
   LEDout(k);
  //  *central=blePeripheral.central();
  //  if(central!=nullptr&&*central){
  //    central->connected();
  //  }



  // NRF_RTC1->TASKS_CLEAR = 1;
  // NRF_RTC1->EVENTS_COMPARE[0] = 0;

}

 }

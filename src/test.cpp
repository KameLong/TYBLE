// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Import libraries (BLEPeripheral depends on SPI)
#include<Arduino.h>
#include <SPI.h>
#include <BLEPeripheral.h>

// define pins (varies per shield/board)
#define BLE_REQ   -1
#define BLE_RDY   -1
#define BLE_RST   -1

// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

BLEService service = BLEService("fff0");

BLEIntCharacteristic characteristicR = BLEIntCharacteristic("fff1", BLERead | BLEWrite);
BLEIntCharacteristic characteristicW = BLEIntCharacteristic("fff2", BLERead | BLEWrite);
// BLECharacteristic characteristicW=BLECharacteristic("fff2",BLERead | BLEWrite,16);

BLEDescriptor descriptor = BLEDescriptor("2901", "value");

int k=0;//LEDのカウントアップ
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

// SVCALL(SD_FLASH_WRITE, uint32_t, sd_flash_write(uint32_t * const p_dst, uint32_t const * const p_src, uint32_t size));
// SVCALL(SD_FLASH_PAGE_ERASE, uint32_t, sd_flash_page_erase(uint32_t page_number));

int flashP=0x33800;
#define SIZE 20
uint32_t save[SIZE];
void setup() {
for(int i=0;i<SIZE;i++){
    save[i]=i;
}
  Serial.begin(9600);
  int ret=sd_flash_write((uint32_t *)flashP,save,SIZE);
  Serial.println(ret);
  Serial.println("start");
  memcpy(save,(uint32_t *)flashP,SIZE);
  delay(100);
  for(int i=0;i<SIZE;i++){
    Serial.println(save[i]);

  }


  LEDsetup();
  NRF_POWER->RESET = 1;
  blePeripheral.setLocalName("kamelong"); // optional
  blePeripheral.setAdvertisedServiceUuid(service.uuid());
  blePeripheral.addAttribute(service);
  blePeripheral.addAttribute(characteristicW);
  blePeripheral.addAttribute(characteristicR);
  blePeripheral.addAttribute(descriptor);

  // set initial value
  characteristicR.setValue(0);

  // begin initialization
  blePeripheral.begin();
  // sd_power_system_off();

}

void loop() {
  __WFE(); //①スリープ命令(wait for event)挿入
}

void tick(){

  k++;
  LEDout(k);
  BLECentral central = blePeripheral.central();
  if (central) {
    // central connected to peripheral
     Serial.print(F("Connected to central: "));
     Serial.println(central.address());
    while (central.connected()) {
      // central still connected to peripheral
      if (characteristicR.written()) {
        int input=characteristicR.value();
         Serial.println(characteristicR.value(), DEC);
        characteristicR.setValue(0);
        // set value on characteristic
        characteristicW.setValue(input+10);
      }
    }
   // central disconnected
     Serial.print(F("Disconnected from central: "));
     Serial.println(central.address());
  }

//NRF_UART0->POWER = 0;
sd_app_evt_wait();

}





extern "C" {
void RTC1_IRQHandler(void)
{
  NRF_RTC1->TASKS_CLEAR = 1;
  NRF_RTC1->EVENTS_COMPARE[0] = 0;
  overflow();
  tick();
}
}
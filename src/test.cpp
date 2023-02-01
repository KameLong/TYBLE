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

void setup() {
    NRF_POWER->RESET = 1;
   Serial.begin(9600);
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
  sd_power_system_off();

}

void loop() {
    delay(100);
    return;
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
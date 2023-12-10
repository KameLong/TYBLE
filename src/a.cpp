// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Import libraries (EddystoneBeacon depends on SPI)
#include <Arduino.h>
#include <BLEPeripheral.h>

// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <BLEPeripheral.h>
#include <iBeacon.h>

#include <nrf_sdm.h>
#include <nrf_soc.h>

#define WDT_REG 0x40010000
#define WDT_START_REG (WDT_REG + 0x000)
#define WDT_CRV_REG (WDT_REG + 0x504)
#define WDT_RR_REG (WDT_REG + 0x600)

void myDelay(int ms){
  for(int i=0;i<ms;i++){
    for(volatile int j=0;j<2*130;j++){

    } 
     }

}

void setOut()
{
  pinMode(21, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
}
void out(int a)
{
  digitalWrite(21, (a >> 0) & 1);
  digitalWrite(4, (a >> 1) & 1);
  digitalWrite(6, (a >> 2) & 1);
  digitalWrite(5, (a >> 3) & 1);
}

void setup()
{
//  NRF_POWER->RESET = 1;
  setOut();

  // *(int32_t *)(WDT_CRV_REG) = 1;
  // *(int32_t *)(WDT_CRV_REG) = 32000;
  //   nrf_clock_lf_cfg_t cfg = {
  //     .source        = NRF_CLOCK_LF_SRC_RC,
  //     .rc_ctiv       = 0,
  //     .rc_temp_ctiv  = 0,
  //     .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_500_PPM
  //   };

  // sd_softdevice_enable(&cfg, NULL);




}
  uint32_t a=0;
void loop()
{
  a++;

  //  sd_clock_hfclk_release();

  out(a);
  myDelay(500);




}

void RTC1_IRQHandler(void)
{

 }

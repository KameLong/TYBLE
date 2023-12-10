#include<Arduino.h>
#include <SPI.h>
#include <BLEPeripheral.h>

int time=0;
int counter=0;

// define pins (varies per shield/board)
#define BLE_REQ   -1
#define BLE_RDY   -1
#define BLE_RST   -1

const char* serviceUUID="10000000-0000-4000-8000-000000000000";
const char* commandInputUUID="20000001-0000-4000-8000-000000000000";
const char* commandOutputUUID="20000002-0000-4000-8000-000000000000";


// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

BLEService service = BLEService(serviceUUID);

BLEFixedLengthCharacteristic commandInput=BLEFixedLengthCharacteristic(commandInputUUID, BLERead | BLEWrite,20);
BLEFixedLengthCharacteristic commandOutput=BLEFixedLengthCharacteristic(commandOutputUUID, BLERead | BLEWrite,20);

void getCommand(const unsigned char* command){
    Serial.println("GET");
    if(strncmp((char*)command,"time",4)==0)
    {
        Serial.println("time");
        unsigned char res[20];
        memset(res,0,20);
        memcpy(res,&time,4);
        commandOutput.setValue(res,20);
        return;
    }
    if(strncmp((char*)command,"count",5)==0)
    {
        Serial.println("count");
        unsigned char res[20];
        memset(res,0,20);
        memcpy(res,&counter,4);
        commandOutput.setValue(res,20);
        return;
    }
}
void actionCommand(const unsigned char* command){
    if(strncmp((char*)command,"count",5)==0)
    {
        counter++;
        return;
    }
    if(strncmp((char*)command,"reset",5)==0)
    {
        counter=0;
        return;
    }
}


// BLEDescriptor descriptor = BLEDescriptor("2901", "value");

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
   Serial.begin(9600);
// for(int i=0;i<SIZE;i++){
//     save[i]=i;
// }
//   int ret=sd_flash_write((uint32_t *)flashP,save,SIZE);
//   Serial.println(ret);
   Serial.println("start");
//   memcpy(save,(uint32_t *)flashP,SIZE);
//   delay(100);
//   for(int i=0;i<SIZE;i++){
//     Serial.println(save[i]);

//   }


  LEDsetup();
  NRF_POWER->RESET = 1;
  blePeripheral.setLocalName("kamelong"); // optional
  blePeripheral.setAdvertisedServiceUuid(service.uuid());
  blePeripheral.addAttribute(service);
  blePeripheral.addAttribute(commandInput);
  blePeripheral.addAttribute(commandOutput);


  blePeripheral.begin();
  // sd_power_system_off();

}

void loop() {
  __WFE(); //①スリープ命令(wait for event)挿入
}

void tick(){
    time++;
  k++;
  LEDout(k);
  BLECentral central = blePeripheral.central();
  if (central) {
    // central connected to peripheral
     Serial.print(F("Connected to central: "));
     Serial.println(central.address());
    while (central.connected()) {
      if (commandInput.written()) {
        const unsigned char* command=commandInput.value();
        if(strncmp("GET ",(const char*)command,4)==0){
            getCommand(command+4);
        }
        if(strncmp("ACT ",(const char*)command,4)==0){
            actionCommand(command+4);
        }
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
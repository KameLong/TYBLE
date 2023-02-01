MAKE_DIR=make
BUILD_DIR=build
SRC_DIR=src

.PRECIOUS: $(BUILD_DIR)/%.o
.PRECIOUS: $(BUILD_DIR)/%.elf
.PRECIOUS: $(BUILD_DIR)/%.hex

USER_NAME=kamelong
LIB_PATH=C:/Users/${USER_NAME}/AppData/Local/Arduino15/packages/sandeepmistry
SDK_PATH=hardware/nRF5/0.7.0/cores/nRF5/SDK/components/toolchain

make/core.a:
	"C:\Users\kamelong\source\TYBLE16\MakeTYBLEcore\bin\Release\net6.0\MakeTYBLEcore.exe"
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp 
	${LIB_PATH}/tools/gcc-arm-none-eabi/5_2-2015q4/bin/arm-none-eabi-g++ -DUSE_LFRC -mcpu=cortex-m0 -mthumb -c -g -Os -w -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -MMD -DF_CPU=16000000 -DARDUINO=10819 -DARDUINO_GENERIC -DARDUINO_ARCH_NRF5 -DNRF51 -DS130 -DNRF51_S130 -DNRF5 "-I${LIB_PATH}/${SDK_PATH}/" "-I${LIB_PATH}/${SDK_PATH}/CMSIS/Include" "-I${LIB_PATH}/${SDK_PATH}/gcc/" "-I${LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/device/" "-I${LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/drivers_nrf/delay/" "-I${LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/softdevice/s130/headers/" "-I${LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5" "-I${LIB_PATH}/hardware/nRF5/0.7.0/variants/Generic" "-I${LIB_PATH}/hardware/nRF5/0.7.0/libraries/SPI" "-IC:/Users/kamelong/Documents/Arduino/libraries/BLEPeripheral/src" $< -o $@

$(BUILD_DIR)/%.elf: $(BUILD_DIR)/%.o make/core.a
	${LIB_PATH}/tools/gcc-arm-none-eabi/5_2-2015q4/bin/arm-none-eabi-gcc  -Os -Wl,--gc-sections -save-temps "-L${LIB_PATH}/${SDK_PATH}/gcc/" "-L${LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/softdevice/s130/toolchain/armgcc/" -Tarmgcc_s130_nrf51822_xxaa.ld "-Wl,-Map,map.map" --specs=nano.specs --specs=nosys.specs -mcpu=cortex-m0 -mthumb -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -o $@ $< -Wl,--start-group -lm make/core.a -Wl,--end-group

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	${LIB_PATH}/tools/gcc-arm-none-eabi/5_2-2015q4/bin/arm-none-eabi-objcopy -O ihex $< $@

write:$(addprefix write-, $(NAMES))
write-%: $(BUILD_DIR)/%.hex
	 ${LIB_PATH}/tools/openocd/0.10.0-dev.nrf5/bin/openocd.exe -d2 -f interface/stlink-v2.cfg -c "transport select hla_swd; set WORKAREASIZE 0x4000;" -f target/nrf51.cfg -c "program $< verify reset; shutdown; "
	del make\\core.a

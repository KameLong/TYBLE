using Microsoft.VisualBasic;
using static System.Collections.Specialized.BitVector32;
using static System.Reflection.Metadata.BlobBuilder;
using System.Net;
using System.Reflection.Metadata;
using System.Xml.Linq;
using System;
using System.Diagnostics;
using System.Collections.ObjectModel;
using System.Collections.Generic;

System.IO.Directory.SetCurrentDirectory(@"C:\Users\kamelong\source\TYBLE16\MakeTYBLEcore\bin\Release\net6.0");


const string USER_NAME = "kamelong";
var set = new HashSet<string>();
//foreach(var file in Directory.GetFiles(@"C:\SoftWare\nRF5SDK110089a8197", "*.h", SearchOption.AllDirectories))
//{
//    set.Add(Path.GetDirectoryName(file));
//}

//foreach (var file in set)
//{
//    Console.WriteLine(file);
//}
//return;





const string LIB_PATH = $"C:/Users/{USER_NAME}/AppData/Local/Arduino15/packages/sandeepmistry";
const string GPP = $"{LIB_PATH}/tools/gcc-arm-none-eabi/5_2-2015q4/bin/arm-none-eabi-g++.exe";
const string GPP_ARG = "-mcpu=cortex-m0 -mthumb -c -g -Os -w -std=gnu++11 -ffunction-sections -fdata-sections -fno-threadsafe-statics -nostdlib --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -MMD -DUSE_LFRC -DF_CPU=16000000 -DARDUINO=10819 -DARDUINO_GENERIC -DARDUINO_ARCH_NRF5 -DNRF51 -DS130 -DNRF51_S130 -DNRF5";

const string GCC = $"{LIB_PATH}/tools/gcc-arm-none-eabi/5_2-2015q4/bin/arm-none-eabi-gcc";
const string GCC_ARG = "-mcpu=cortex-m0 -mthumb -c -g -Os -w -std=gnu11 -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -MMD -DUSE_LFRC -DF_CPU=16000000 -DARDUINO=10819 -DARDUINO_GENERIC -DARDUINO_ARCH_NRF5 -DNRF51 -DS130 -DNRF51_S130 -DNRF5";
const string AS_ARG = "-c -g -x assembler-with-cpp -DF_CPU=16000000 -DARDUINO=10819 -DUSE_LFRC -DARDUINO_GENERIC -DARDUINO_ARCH_NRF5 -DNRF51 -DS130 -DNRF51_S130";

List<string> includePath =new List<string>(){
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/toolchain/",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/toolchain/CMSIS/Include",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/toolchain/gcc/",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/device/",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/drivers_nrf/delay/",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/softdevice/s130/headers/",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/variants/Generic",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/libraries/SPI",
    $"C:/Users/{USER_NAME}/Documents/Arduino/libraries/BLEPeripheral/src"};
string includeStr()
{
    string result = "";
    foreach(var path in includePath)
    {
        result += "\"-I" + path + "\" "; 
    }
    return result;
}

List<string> buildPath = new List<string>()
{
    $"C:/Users/{USER_NAME}/Documents/Arduino/libraries/BLEPeripheral/src",
    $"C:/Users/{USER_NAME}/AppData/Local/Arduino15/packages/sandeepmistry/hardware/nRF5/0.7.0/libraries/SPI/",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/toolchain",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/variants/Generic",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5/SDK/components/toolchain/gcc",
    $"{LIB_PATH}/hardware/nRF5/0.7.0/cores/nRF5"
};

const string OBJ_DIR= "../../../../obj";

List<string> buildFiles = new List<string>();
foreach(var dir in buildPath)
{
    foreach (var file in Directory.GetFiles(dir, "*.*"))
    {
        if (Path.GetExtension(file) == ".c" || Path.GetExtension(file) == ".cpp"||Path.GetExtension(file)==".S")
        {
            buildFiles.Add(file);
        }
    }
}
Parallel.ForEach(buildFiles, file =>
//foreach (var file in buildFiles)
{
    makeCore(file);
    Console.WriteLine(file);
}
);

File.Delete("../../../../make/core.a");
foreach (var file in Directory.GetFiles(OBJ_DIR, "*.o"))
{
    ProcessStartInfo processStartInfo;
    processStartInfo = new ProcessStartInfo($"{LIB_PATH}/tools/gcc-arm-none-eabi/5_2-2015q4/bin/arm-none-eabi-ar",
      $"rcs ../../../../make/core.a {file}");
    Process p = Process.Start(processStartInfo);
    p.WaitForExit();
}






void makeCore(string filePath)
{
    Process proc = new Process();
    // 第1引数がコマンド、第2引数がコマンドの引数
    ProcessStartInfo processStartInfo;
    if (Path.GetExtension(filePath) == ".c")
    {
//        Console.WriteLine($"{GCC_ARG} {includeStr()} {filePath} -o {OBJ_DIR}/{Path.GetFileName(filePath)}.o");
        processStartInfo = new ProcessStartInfo(GCC, $"{GCC_ARG} {includeStr()} {filePath} -o {OBJ_DIR}/{Path.GetFileName(filePath)}.o");
    }
    else if(Path.GetExtension(filePath) == ".cpp")
    {
//        Console.WriteLine($"{GPP} {GPP_ARG} {includeStr()} {filePath} -o {OBJ_DIR}/{Path.GetFileName(filePath)}.o");
        processStartInfo = new ProcessStartInfo(GPP, $"{GPP_ARG} {includeStr()} {filePath} -o {OBJ_DIR}/{Path.GetFileName(filePath)}.o");
    }
    else
    {
 //       Console.WriteLine($"{AS_ARG} {includeStr()} {filePath} -o {OBJ_DIR}/{Path.GetFileName(filePath)}.o");
        processStartInfo = new ProcessStartInfo(GCC, $"{AS_ARG} {includeStr()} {filePath} -o {OBJ_DIR}/{Path.GetFileName(filePath)}.o");

    }
    System.Diagnostics.Process p = System.Diagnostics.Process.Start(processStartInfo);
    p.WaitForExit();

}

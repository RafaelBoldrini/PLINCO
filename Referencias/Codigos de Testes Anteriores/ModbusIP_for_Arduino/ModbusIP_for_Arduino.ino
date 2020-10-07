/*
  Modbus IP to Arduino - Example
  github.com/andresarmento/modbus-arduino

  Here you can:
  Send Button status by Modbus(0002)
  Get Led status by Modbus(0000)
  Send Timer information by Modbus(0003)
  Get the command to stop and
  reset the Timer while 
  Modbus(0001) is iqual to Reset_Timer
*/

// Modbus Configuration
#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>
ModbusIP mb;

// Define Modbus Registers Offsets (0-9999)
const int Led_OffsetToGet     = 0000;
const int Info_OffsetToGet    = 0001;
const int Button_OffsetToSend = 0002;
const int Timer_OffsetToSend  = 0003;

// Define Arduino Pins
const int Led_ArduinoPin  = 13;
const int Button_ArduinoPin = 3;

// Indicate Program Datas
int Led_Data = 0; int Info_Data = 0;
int Button_Data = 0; int Timer_Data = 0;

// Indicate Program Stuffs
long PreviousMillis = 0;
long IntervalMillis = 1000;
unsigned long CurrentMillis = 0;
int Reset_Timer = 1000;

void setup(){
  
  // Config MAC address
  byte mac[] = { 0xDE, 0xAD, 0xBE, 
                 0xEF, 0xFE, 0xED }; 
                 
  // Config IP address 
  byte ip[] = { 192, 168, 0, 100 };

  // Config Modbus IP
  mb.config(mac, ip);

  // Add Registers to
  // Get Data from Modbus
  mb.addHreg(Led_OffsetToGet);
  mb.addHreg(Info_OffsetToGet);

  // Add Registers to
  // Send Data to Modbus
  mb.addIreg(Button_OffsetToSend);
  mb.addIreg(Timer_OffsetToSend);
  
  // Set Arduino Pins
  pinMode(Led_ArduinoPin, OUTPUT);
  pinMode(Button_ArduinoPin, INPUT);
  
}

void loop(){

  // Call once inside loop()
  // All magic is here
  mb.task();
  
//---------------- Get Datas

  // Get Data from Modbus
  Led_Data = mb.Hreg(Led_OffsetToGet);
  Info_Data = mb.Hreg(Info_OffsetToGet);

  // Get Data from Arduino
  Button_Data = 
  digitalRead(Button_ArduinoPin);

//---------------- Calculations

  CurrentMillis = millis();
  if(CurrentMillis - PreviousMillis >= 
  IntervalMillis){
    PreviousMillis = CurrentMillis;
    Timer_Data = Timer_Data + 2;
  }
  if(Info_Data == Reset_Timer){
    Timer_Data = 0;
  }
  
//---------------- Do Things

  // Send Data to Arduino
  digitalWrite(Led_ArduinoPin,
  Led_Data);

  // Send Data to Modbus
  mb.Ireg(Button_OffsetToSend, Button_Data);
  mb.Ireg(Timer_OffsetToSend, Timer_Data);

}

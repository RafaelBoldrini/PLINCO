////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Slave RS485
//   Software for Arduino NANO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 1; // Replace with the ID of Module

#include <Arduino.h> // necessário para o platform.io
#include <SoftwareSerial.h>

int RS485_T_RO_Pin = 2;
int RS485_T_DI_Pin = 3;
SoftwareSerial RS485_Transmit(RS485_T_RO_Pin, RS485_T_DI_Pin);

bool Monitor_Program = false;
bool Monitor_Protocol = false;

unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

String Memory_01 = "XXXX";

#include <Wire.h>
#include <VL53L0X.h>
VL53L0X VL53L0X_1;

String Value_Format(int Value)
{
  String Value_Formated = String(Value);

  if (Value < 1000)
  {
    Value_Formated = "0" + Value_Formated;
  }
  if (Value < 100)
  {
    Value_Formated = "0" + Value_Formated;
  }
  if (Value < 10)
  {
    Value_Formated = "0" + Value_Formated;
  }
  return Value_Formated;
}

void Monitor_Control()
{
  while (Serial.available())
  {
    char byteRec = Serial.read();

    if (byteRec == 'p')
    {
      Monitor_Program = !Monitor_Program;
    }
    if (byteRec == 'P')
    {
      Monitor_Protocol = !Monitor_Protocol;
    }
  }
}

void RS485_Write(int Module_Number, String Memory_Number, String Memory_Value)
{
  String Command = "#" + String(Module_Number) + "." + Memory_Number + "=" + Memory_Value + ";";

  RS485_Transmit.print(Command);

  if (Monitor_Protocol == true)
  {
    Serial.print("\t\t\t Sent: ");
    Serial.println(Command);
  }
}

void Timer_1_Set()
{
  unsigned long currentMillis = millis();
  Timer_1_Control = currentMillis;
  Timer_1_Value = 1;
}

void Timer_1(unsigned long Interval)
{
  unsigned long currentMillis = millis();
  unsigned long Timer_Comparation = currentMillis - Timer_1_Control;
  if (Timer_Comparation > Interval)
  {
    Timer_1_Value = 0;
  }
}

void Process()
{
  Timer_1(1000);
  if (Timer_1_Value == 0)
  {
    int VL53L0X_Info = VL53L0X_1.readRangeSingleMillimeters();
    if (VL53L0X_Info <= 2500)
    {
      Memory_01 = Value_Format(VL53L0X_Info);
    }
    if (VL53L0X_Info > 2500)
    {
      Memory_01 = "XXXX";
    }
    if (VL53L0X_1.timeoutOccurred())
    {
      Memory_01 = "XXXX";
    }

    RS485_Write(1, "01", Memory_01);
    Timer_1_Set();
  }
}

void Program_Monitor()
{
  if (Monitor_Program == true)
  {
    Serial.print("M_01 = ");
    Serial.print(Memory_01);
    Serial.println("");
  }
}

void setup()
{
  RS485_Transmit.begin(9600);

  Serial.begin(9600);

  Wire.begin();
  VL53L0X_1.setTimeout(500);
  if (!VL53L0X_1.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1)
    {
    }
  }
  //Define LONG_RANGE
  //Lower the return signal rate limit (default is 0.25 MCPS)
  VL53L0X_1.setSignalRateLimit(0.1);
  //Increase laser pulse periods (defaults are 14 and 10 PCLKs)
  VL53L0X_1.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  VL53L0X_1.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  //Define HIGH_ACCURACY
  //Increase timing budget to 500000 µs (default is 33000 µs)
  VL53L0X_1.setMeasurementTimingBudget(500000);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
  Process();
} // loop()
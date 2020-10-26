////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Module of Modbus TCP IP
//   Software for Arduino UNO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 1; // Replace with the ID of Module

#include <Arduino.h> // necessário para o platform.io
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>
ModbusIP Modbus_IP;

// Config MAC and IP address
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 0, 100};

// Define Modbus Registers Offsets (0-9999)
const int Memory_01_Modbus_IP_Offset = 0000;
const int Memory_02_Modbus_IP_Offset = 0001;
const int Memory_03_Modbus_IP_Offset = 0002;
const int Memory_04_Modbus_IP_Offset = 0003;

bool Monitor_Program = false;
bool Monitor_Protocol = false;

bool Correct_Command = false;
String Command_Type = "/XXX.";
String Memory_Number = "XX";
String Memory_Value = "XXXX";

String Memory_01 = "XXXX";
String Memory_02 = "XXXX";
String Memory_03 = "XXXX";
String Memory_04 = "XXXX";

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

void Modbus_IP_Config()
{
  // Add Registers to Receive Data
  Modbus_IP.addHreg(Memory_01_Modbus_IP_Offset);
  Modbus_IP.addHreg(Memory_02_Modbus_IP_Offset);

  // Add Registers to Send Data
  Modbus_IP.addIreg(Memory_03_Modbus_IP_Offset);
  Modbus_IP.addIreg(Memory_04_Modbus_IP_Offset);
}

void Send_to_ModbusIP()
{
  Modbus_IP.Ireg(Memory_03_Modbus_IP_Offset, Memory_03.toInt());
  Modbus_IP.Ireg(Memory_04_Modbus_IP_Offset, Memory_04.toInt());
}

void Memorys_to_GET()
{
  /*
  if (Memory_Number == "01")
  {
    Correct_Command = true;
    Memory_Value = ...;
    Memory_01 = Memory_Value;
  }
  if (Memory_Number == "02")
  {
    Correct_Command = true;
    Memory_Value = ...;
    Memory_02 = Memory_Value;
  }
  */
}

void Memorys_to_SET()
{
  if (Memory_Number == "03" && Memory_Value.toInt() <= 9999)
  {
    Correct_Command = true;
    Memory_03 = Memory_Value;
  }
  if (Memory_Number == "04" && Memory_Value.toInt() <= 9999)
  {
    Correct_Command = true;
    Memory_04 = Memory_Value;
  }
}

void ReceiveEvent(int howMany)
{
  String Wire_Read = "";
  Correct_Command = false;

  while (Wire.available())
  {
    char byteRec = Wire.read();
    Wire_Read = Wire_Read + byteRec;
  }

  if (Wire_Read.substring(0, 5) == "/GET.")
  {
    if (Wire_Read.substring(5, 6) == String(Module_Number))
    {
      if (Wire_Read.substring(6, 7) == ".")
      {
        Memory_Number = Wire_Read.substring(7, 9);
        if (Wire_Read.substring(9, 10) == ":")
        {
          Memorys_to_GET();
        }
      }
    }
  }

  if (Wire_Read.substring(0, 5) == "/SET.")
  {
    if (Wire_Read.substring(5, 6) == String(Module_Number))
    {
      if (Wire_Read.substring(6, 7) == ".")
      {
        Memory_Number = Wire_Read.substring(7, 9);
        if (Wire_Read.substring(9, 10) == "=")
        {
          Memory_Value = Wire_Read.substring(10, 14);
          if (Wire_Read.substring(14, 15) == ":")
          {
            Memorys_to_SET();
          }
        }
      }
    }
  }

  Command_Type = Wire_Read.substring(0, 5);

  if (Monitor_Protocol == true)
  {
    Serial.print("Received: ");
    Serial.println(Wire_Read);
  }
}

void RequestEvent()
{
  String Info_to_Send = "X.XX=XXXX;";

  if (Correct_Command == false)
  {
    Info_to_Send = String(Module_Number) + "." + Memory_Number + "." + "XXXX" + ";";
  }

  if (Command_Type == "/SET.")
  {
    if (Correct_Command == true)
    {
      Info_to_Send = String(Module_Number) + "." + Memory_Number + "." + "DONE" + ";";
      Correct_Command = false;
    }
  }

  if (Command_Type == "/GET.")
  {
    if (Correct_Command == true)
    {
      Info_to_Send = String(Module_Number) + "." + Memory_Number + "=" + Memory_Value + ";";
      Correct_Command = false;
    }
  }

  Wire.write(Info_to_Send.c_str());

  if (Monitor_Protocol == true)
  {
    Serial.print("Sent: ");
    Serial.println(Info_to_Send);
  }
}

void Program_Monitor()
{
  if (Monitor_Program == true)
  {
    Serial.print("M_01 = ");
    Serial.print(Memory_01);
    Serial.print("  ");
    Serial.print("M_02 = ");
    Serial.print(Memory_02);
    Serial.print("  ");
    Serial.print("M_03 = ");
    Serial.print(Memory_03);
    Serial.print("  ");
    Serial.print("M_04 = ");
    Serial.print(Memory_04);
    Serial.println("");
  }
}

void setup()
{
  Wire.begin(Module_Number);
  Wire.onReceive(ReceiveEvent);
  Wire.onRequest(RequestEvent);

  Serial.begin(9600);

  Modbus_IP.config(mac, ip);
  Modbus_IP_Config();
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
  Send_to_ModbusIP();
  Modbus_IP.task();
} // loop()
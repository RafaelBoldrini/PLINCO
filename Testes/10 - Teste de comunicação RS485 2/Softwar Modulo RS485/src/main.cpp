////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Module of Master RS485
//   Software for Arduino UNO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 1; // Replace with the ID of Module

#include <Arduino.h> // necessário para o platform.io
#include <Wire.h>
#include <SoftwareSerial.h>

int RS485_T_RO_Pin = 2;
int RS485_T_DI_Pin = 3;
int RS485_R_RO_Pin = 4;
int RS485_R_DI_Pin = 5;
SoftwareSerial RS485_Transmit(RS485_T_RO_Pin, RS485_T_DI_Pin);
SoftwareSerial RS485_Receive(RS485_R_RO_Pin, RS485_R_DI_Pin);

bool Monitor_Program = false;
bool Monitor_Protocol = false;

bool Correct_Command = false;
String Memory_Number = "XX";
String Memory_Value = "XXXX";
String Command_Type = "XXXX";

unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

String Memory_01 = "XXXX";
String Memory_02 = "XXXX";

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

void Memorys_to_GET()
{
  if (Memory_Number == "01")
  {
    Correct_Command = true;
    Memory_Value = Memory_01;
  }
}

void Memorys_to_SET()
{
  if (Memory_Number == "02" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_02 = Memory_Value;
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
  String Info_to_Send = "X.XXXXXXX;";

  if (Correct_Command == false)
  {
    Info_to_Send = String(Module_Number) + "." + Memory_Number + "X" + "XXXX" + ";";
  }

  if (Correct_Command == true)
  {
    if (Command_Type == "/GET.")
    {
      Info_to_Send = String(Module_Number) + "." + Memory_Number + "=" + Memory_Value + ";";
      Correct_Command = false;
    }
    if (Command_Type == "/SET.")
    {
    Info_to_Send = String(Module_Number) + "." + Memory_Number + "." + "DONE" + ";";
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

void Memorys_of_RS485(int Module_Number, String Memory_Number, String Memory_Value)
{
  if (Module_Number == 1)
  {
    if (Memory_Number == "01")
    {
      if (Memory_Value == "XXXX")
      {
        Memory_Value = Memory_01;
      }
      Memory_01 = Memory_Value;
    }
  }
}

void RS485_Read()
{
  String RS485_Read = "";
  String Memory_Value = "XXXX";

  while (RS485_Receive.available())
  {
    char byteRec = RS485_Receive.read();
    if (byteRec == '#')
    {
      RS485_Read = "";
    }
    RS485_Read = RS485_Read + byteRec;
    if (byteRec == ';')
    {
      break;
    }
  }

  if (RS485_Read.substring(0, 1) == "#")
  {
    int Module_Number = RS485_Read.substring(1, 2).toInt();
    if (RS485_Read.substring(2, 3) == ".")
    {
      String Memory_Number = RS485_Read.substring(3, 5);
      if (RS485_Read.substring(5, 6) == "=")
      {
        String Memory_Value = RS485_Read.substring(6, 10);
        if (RS485_Read.substring(10, 11) == ";")
        {
          Memorys_of_RS485(Module_Number, Memory_Number, Memory_Value);
        }
      }
    }
  }

  if (Monitor_Protocol == true)
  {
    Serial.print("\t\t\t Received: ");
    Serial.println(RS485_Read);
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
    RS485_Write(1, "01", Memory_02);
    Timer_1_Set();
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
    Serial.println("");
  }
}

void setup()
{
  Wire.begin(Module_Number);
  Wire.onReceive(ReceiveEvent);
  Wire.onRequest(RequestEvent);

  RS485_Transmit.begin(9600);
  RS485_Receive.begin(9600);

  Serial.begin(9600);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
  Process();
  RS485_Read();
} // loop()
////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Module of Digital Input
//   Software for Arduino NANO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 1; // Replace with the ID of Module

#include <Arduino.h>
#include <Wire.h>

bool Monitor_Program = false;
bool Monitor_Protocol = false;

bool Correct_Command = false;
String Memory_Number = "XX";
String Memory_Value = "XXXX";

String Memory_01 = "0000";
String Memory_02 = "0000";
String Memory_03 = "0000";
String Memory_04 = "0000";
String Memory_05 = "0000";
String Memory_06 = "0000";
String Memory_07 = "0000";
String Memory_08 = "0000";

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
    if (Wire_Read.substring(5, 2) == String(Module_Number))
    {
      if (Wire_Read.substring(6, 7) == ".")
      {
        Memory_Number = Wire_Read.substring(7, 9);
        if (Wire_Read.substring(9, 10) == ":")
        {
          if (Memory_Number == "01")
          {
            Correct_Command = true;
            Memory_Value = Value_Format(digitalRead(2));
            Memory_01 = Memory_Value;
          }
          if (Memory_Number == "02")
          {
            Correct_Command = true;
            Memory_Value = Value_Format(digitalRead(3));
            Memory_02 = Memory_Value;
          }
          if (Memory_Number == "03")
          {
            Correct_Command = true;
            Memory_Value = Value_Format(digitalRead(4));
            Memory_03 = Memory_Value;
          }
          if (Memory_Number == "04")
          {
            Correct_Command = true;
            Memory_Value = Value_Format(digitalRead(5));
            Memory_04 = Memory_Value;
          }
          if (Memory_Number == "05")
          {
            Correct_Command = true;
            Memory_Value = Value_Format(digitalRead(6));
            Memory_05 = Memory_Value;
          }
          if (Memory_Number == "06")
          {
            Correct_Command = true;
            Memory_Value = Value_Format(digitalRead(7));
            Memory_06 = Memory_Value;
          }
          if (Memory_Number == "07")
          {
            Correct_Command = true;
            Memory_Value = Value_Format(digitalRead(8));
            Memory_07 = Memory_Value;
          }
          if (Memory_Number == "08")
          {
            Correct_Command = true;
            Memory_Value = Value_Format(digitalRead(9));
            Memory_08 = Memory_Value;
          }
        }
      }
    }
  }

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

  if (Correct_Command == true)
  {
    Info_to_Send = String(Module_Number) + "." + Memory_Number + "." + Memory_Value + ";";
    Correct_Command = false;
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
    Serial.print(Memory_01.toInt());
    Serial.print("  ");
    Serial.print("M_02 = ");
    Serial.print(Memory_02.toInt());
    Serial.print("  ");
    Serial.print("M_03 = ");
    Serial.print(Memory_03.toInt());
    Serial.print("  ");
    Serial.print("M_04 = ");
    Serial.print(Memory_04.toInt());
    Serial.print("  ");
    Serial.print("M_05 = ");
    Serial.print(Memory_05.toInt());
    Serial.print("  ");
    Serial.print("M_06 = ");
    Serial.print(Memory_06.toInt());
    Serial.print("  ");
    Serial.print("M_07 = ");
    Serial.print(Memory_07.toInt());
    Serial.print("  ");
    Serial.print("M_08 = ");
    Serial.print(Memory_08.toInt());
    Serial.print("  ");
    Serial.println("");
  }
}

void setup()
{
  Wire.begin(Module_Number);
  Wire.onReceive(ReceiveEvent);
  Wire.onRequest(RequestEvent);
  Serial.begin(9600);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
}
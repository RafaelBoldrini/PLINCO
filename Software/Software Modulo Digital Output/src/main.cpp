////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Module of Digital Output
//   Software for Arduino NANO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 2; // Replace with the ID of Module

#include <Arduino.h>
#include <Wire.h>

bool Monitor_Program = false;
bool Monitor_Protocol = false;

bool Correct_Command = false;
String Memory_Number = "XX";
String Memory_Value = "XXXX";

String Memory_01 = "XXXX";
String Memory_02 = "XXXX";
String Memory_03 = "XXXX";
String Memory_04 = "XXXX";
String Memory_05 = "XXXX";
String Memory_06 = "XXXX";
String Memory_07 = "XXXX";
String Memory_08 = "XXXX";

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

void Memorys_to_SET()
{
  if (Memory_Number == "01" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_01 = Memory_Value;
    digitalWrite(2, Memory_01.toInt());
  }
  if (Memory_Number == "02" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_02 = Memory_Value;
    digitalWrite(3, Memory_02.toInt());
  }
  if (Memory_Number == "03" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_03 = Memory_Value;
    digitalWrite(4, Memory_03.toInt());
  }
  if (Memory_Number == "04" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_04 = Memory_Value;
    digitalWrite(5, Memory_04.toInt());
  }
  if (Memory_Number == "05" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_05 = Memory_Value;
    digitalWrite(6, Memory_05.toInt());
  }
  if (Memory_Number == "06" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_06 = Memory_Value;
    digitalWrite(7, Memory_06.toInt());
  }
  if (Memory_Number == "07" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_07 = Memory_Value;
    digitalWrite(8, Memory_07.toInt());
  }
  if (Memory_Number == "08" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    Memory_08 = Memory_Value;
    digitalWrite(9, Memory_08.toInt());
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
    Info_to_Send = String(Module_Number) + "." + Memory_Number + "." + "DONE" + ";";
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

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
}
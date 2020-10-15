////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Module of Analogic Output
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
    Serial.print("  ");
    Serial.print("M_05 = ");
    Serial.print(Memory_05);
    Serial.print("  ");
    Serial.print("M_06 = ");
    Serial.print(Memory_06);
    Serial.print("  ");
    Serial.println("");
  }
}

void Process()
{
  analogWrite(3, Memory_01.toInt());
  analogWrite(5, Memory_02.toInt());
  analogWrite(6, Memory_03.toInt());
  analogWrite(9, Memory_04.toInt());
  analogWrite(10, Memory_05.toInt());
  analogWrite(11, Memory_06.toInt());
}

void Memorys_to_SET()
{
  if (Memory_Number == "01" && Memory_Value.toInt() <= 255)
  {
    Correct_Command = true;
    Memory_01 = Memory_Value;
  }
  if (Memory_Number == "02" && Memory_Value.toInt() <= 255)
  {
    Correct_Command = true;
    Memory_02 = Memory_Value;
  }
  if (Memory_Number == "03" && Memory_Value.toInt() <= 255)
  {
    Correct_Command = true;
    Memory_03 = Memory_Value;
  }
  if (Memory_Number == "04" && Memory_Value.toInt() <= 255)
  {
    Correct_Command = true;
    Memory_04 = Memory_Value;
  }
  if (Memory_Number == "05" && Memory_Value.toInt() <= 255)
  {
    Correct_Command = true;
    Memory_05 = Memory_Value;
  }
  if (Memory_Number == "06" && Memory_Value.toInt() <= 255)
  {
    Correct_Command = true;
    Memory_06 = Memory_Value;
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

void setup()
{
  Wire.begin(Module_Number);
  Wire.onReceive(ReceiveEvent);
  Wire.onRequest(RequestEvent);

  Serial.begin(9600);

  pinMode(3, OUTPUT);
  analogWrite(3, 0);
  pinMode(5, OUTPUT);
  analogWrite(5, 0);
  pinMode(6, OUTPUT);
  analogWrite(6, 0);
  pinMode(9, OUTPUT);
  analogWrite(9, 0);
  pinMode(10, OUTPUT);
  analogWrite(10, 0);
  pinMode(11, OUTPUT);
  analogWrite(11, 0);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
  Process();
}
////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Module of Analogic Input
//   Software for Arduino NANO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 3; // Replace with the ID of Module

#include <Arduino.h>
#include <Wire.h>

bool Monitor_Program = false;
bool Monitor_Protocol = false;

bool Correct_Command = false;
String Memory_Number = "XX";
String Memory_Value = "XXXX";

unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

String Memory_01 = "XXXX";
String Memory_02 = "XXXX";
String Memory_03 = "XXXX";
String Memory_04 = "XXXX";
String Memory_05 = "XXXX";
String Memory_06 = "XXXX";

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

void Memorys_to_GET()
{
  if (Memory_Number == "01")
  {
    Correct_Command = true;
    Memory_01 = Value_Format(analogRead(A0));
    Memory_Value = Memory_01;
  }
  if (Memory_Number == "02")
  {
    Correct_Command = true;
    Memory_02 = Value_Format(analogRead(A1));
    Memory_Value = Memory_02;
  }
  if (Memory_Number == "03")
  {
    Correct_Command = true;
    Memory_03 = Value_Format(analogRead(A2));
    Memory_Value = Memory_03;
  }
  if (Memory_Number == "04")
  {
    Correct_Command = true;
    Memory_04 = Value_Format(analogRead(A3));
    Memory_Value = Memory_04;
  }
  if (Memory_Number == "05")
  {
    Correct_Command = true;
    Memory_05 = Value_Format(analogRead(A6));
    Memory_Value = Memory_05;
  }
  if (Memory_Number == "06")
  {
    Correct_Command = true;
    Memory_06 = Value_Format(analogRead(A7));
    Memory_Value = Memory_06;
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
    Info_to_Send = String(Module_Number) + "." + Memory_Number + "=" + "XXXX" + ";";
  }

  if (Correct_Command == true)
  {
    Info_to_Send = String(Module_Number) + "." + Memory_Number + "=" + Memory_Value + ";";
    Correct_Command = false;
  }

  Wire.write(Info_to_Send.c_str());

  if (Monitor_Protocol == true)
  {
    Serial.print("Sent: ");
    Serial.println(Info_to_Send);
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

void Program_Monitor()
{
  if (Monitor_Program == true)
  {
    Timer_1(1000);
    if (Timer_1_Value == 0)
    {
      Memory_01 = Value_Format(analogRead(A0));
      Memory_02 = Value_Format(analogRead(A1));
      Memory_03 = Value_Format(analogRead(A2));
      Memory_04 = Value_Format(analogRead(A3));
      Memory_05 = Value_Format(analogRead(A6));
      Memory_06 = Value_Format(analogRead(A7));

      Timer_1_Set();
    }

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
} // loop()

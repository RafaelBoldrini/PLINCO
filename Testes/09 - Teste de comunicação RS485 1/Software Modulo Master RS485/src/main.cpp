////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Module of Master RS485
//   Software for Arduino NANO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 1; // Replace with the ID of Module

#include <Arduino.h> // necessário para o platform.io
#include <Wire.h>
#include <SoftwareSerial.h>

int Pino_RS485_RO = 2; 
int Pino_RS485_DI = 3;
int RS485_Control = 4;
bool RS485_Transmit = HIGH;
bool RS485_Receive = LOW;
SoftwareSerial RS485_Serial(Pino_RS485_RO, Pino_RS485_DI);
bool RS485_Wait = false;
unsigned long RS485_Timeout = 0;

String RS485_Memory_00 = "XXXX";

bool Monitor_Program = false;
bool Monitor_Protocol = false;

bool Correct_Command = false;
String Command_Type = "/XXX.";
String Memory_Number = "XX";
String Memory_Value = "XXXX";

String Memory_01 = "XXXX";
String Memory_02 = "XXXX";

String RS485_Memory_01 = "XXXX";
String RS485_Memory_02 = "XXXX";

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
    Serial.print(RS485_Memory_01);
    Serial.print("  ");
    Serial.print("M_02 = ");
    Serial.print(RS485_Memory_02);
    Serial.println("");
  }
}

void RS485_GET(int Module_Number, String Memory_Number, String Reference)
{
  String Command = "/.485.GET." + String(Module_Number) + "." + Memory_Number + ":";

  digitalWrite(RS485_Control, RS485_Transmit);
  RS485_Serial.print(Command);
  digitalWrite(RS485_Control, RS485_Receive);

  if (Monitor_Protocol == true)
  {
    Serial.print("Sent: ");
    Serial.println(Command);
  }

  String RS485_Read = "";
  RS485_Memory_00 = "XXXX";

  RS485_Wait = true;
  unsigned long Current_Millis = millis();
  unsigned long Timeout_Control = Current_Millis;

  while (RS485_Wait == true)
  {
    while (RS485_Serial.available())
    {
      char byteRec = RS485_Serial.read();
      RS485_Read = RS485_Read + byteRec;
    }

    if (RS485_Read.substring(5, 6) == String(Module_Number))
    {
      if (RS485_Read.substring(6, 7) == ".")
      {
        if (RS485_Read.substring(7, 9) == Memory_Number)
        {
          if (RS485_Read.substring(9, 10) == "=")
          {
            if (RS485_Read.substring(14, 15) == ";")
            {
              RS485_Memory_00 = RS485_Read.substring(10, 14);
              RS485_Wait = false;
            }
          }
        }
      }
    }

    Current_Millis = millis();
    unsigned long Timer_Comparation = Current_Millis - Timeout_Control;
    if (Timer_Comparation > 1000)
    {
      RS485_Wait = false;
    }
  }

  if (Monitor_Protocol == true)
  {
    Serial.print("Received: ");
    Serial.println(RS485_Read);
  }

  if (RS485_Memory_00 == "XXXX")
  {
    RS485_Memory_00 = Reference;
  }
}

void RS485_SET(int Module_Number, String Memory_Number, String Memory_Value, String Reference)
{
  String Command = "/.485.SET." + String(Module_Number) + "." + Memory_Number + "=";
  Command = Command + Memory_Value + ":";

  digitalWrite(RS485_Control, RS485_Transmit);
  RS485_Serial.print(Command);
  digitalWrite(RS485_Control, RS485_Receive);

  if (Monitor_Protocol == true)
  {
    Serial.print("Sent: ");
    Serial.println(Command);
  }

  String RS485_Read = "";
  RS485_Memory_00 = "XXXX";

  RS485_Wait = true;
  unsigned long Current_Millis = millis();
  unsigned long Timeout_Control = Current_Millis;

  while (RS485_Wait == true)
  {
    while (RS485_Serial.available())
    {
      char byteRec = RS485_Serial.read();
      RS485_Read = RS485_Read + byteRec;
    }

    if (RS485_Read.substring(5, 6) == String(Module_Number))
    {
      if (RS485_Read.substring(6, 7) == ".")
      {
        if (RS485_Read.substring(7, 9) == Memory_Number)
        {
          if (RS485_Read.substring(9, 10) == ".")
          {
            if (RS485_Read.substring(14, 15) == ";")
            {
              RS485_Memory_00 = RS485_Read.substring(10, 14);
              RS485_Wait = false;
            }
          }
        }
      }
    }

    Current_Millis = millis();
    unsigned long Timer_Comparation = Current_Millis - Timeout_Control;
    if (Timer_Comparation > 1000)
    {
      RS485_Wait = false;
    }
  }

  if (Monitor_Protocol == true)
  {
    Serial.print("Received: ");
    Serial.println(RS485_Read);
  }

  if (RS485_Memory_00 == "XXXX")
  {
    RS485_Memory_00 = Reference;
  }
  if (RS485_Memory_00 == "DONE")
  {
    RS485_Memory_00 = Memory_Value;
  }
}

void Process()
{
  RS485_GET(1, "01", RS485_Memory_01);
  RS485_Memory_01 = RS485_Memory_00;
  Memory_01 = RS485_Memory_01;

  RS485_SET(2, "01", Memory_02, RS485_Memory_02);
  RS485_Memory_02 = RS485_Memory_00; 
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
  /*
  if (Memory_Number == "01")
  {
    Correct_Command = true;
    Memory_01 = Memory_Value;
  }
  */
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

void setup()
{
  Wire.begin(Module_Number);
  Wire.onReceive(ReceiveEvent);
  Wire.onRequest(RequestEvent);

  Serial.begin(9600);

  RS485_Serial.begin(9600);
  pinMode(RS485_Control, OUTPUT);
  digitalWrite(RS485_Control, RS485_Receive);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
  Process();
} // loop()

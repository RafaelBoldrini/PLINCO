////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Slave RS485
//   Software for Arduino NANO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 2; // Replace with the ID of Module

#include <Arduino.h> // necessário para o platform.io
#include <SoftwareSerial.h>

int Pino_RS485_RO = 2;
int Pino_RS485_DI = 3;
int RS485_Control = 4;
bool RS485_Transmit = HIGH;
bool RS485_Receive = LOW;
SoftwareSerial RS485_Serial(Pino_RS485_RO, Pino_RS485_DI);

bool Monitor_Program = false;
bool Monitor_Protocol = false;

bool Correct_Command = false;
String Command_Type = "/XXX.";
String Memory_Number = "XX";
String Memory_Value = "XXXX";

String RS485_Memory_01 = "XXXX";

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
    Serial.println("");
  }
}

void Process()
{
  digitalWrite(13, RS485_Memory_01.toInt());
}

void Memorys_to_GET()
{
  /*
  if (Memory_Number == "01")
  {
    Correct_Command = true;
    Memory_Value = RS485_Memory_01;
  }
  */
}

void Memorys_to_SET()
{
  if (Memory_Number == "01" && Memory_Value.toInt() <= 1)
  {
    Correct_Command = true;
    RS485_Memory_01 = Memory_Value;
  }
}

void RS485_Write()
{
  String Info_to_Send = "X.XX=XXXX;";

  if (Correct_Command == false)
  {
    Info_to_Send = String(Module_Number) + "." + Memory_Number + "." + "XXXX" + ";";
  }

  if (Command_Type == "/.485.SET.")
  {
    if (Correct_Command == true)
    {
      Info_to_Send = ".485." + String(Module_Number) + "." + Memory_Number + "." + "DONE" + ";";
      Correct_Command = false;
    }
  }

  if (Command_Type == "/.485.GET.")
  {
    if (Correct_Command == true)
    {
      Info_to_Send = ".485." + String(Module_Number) + "." + Memory_Number + "=" + Memory_Value + ";";
      Correct_Command = false;
    }
  }

  digitalWrite(RS485_Control, RS485_Transmit);
  RS485_Serial.print(Info_to_Send);
  digitalWrite(RS485_Control, RS485_Receive);

  if (Monitor_Protocol == true)
  {
    Serial.print("Sent: ");
    Serial.println(Info_to_Send);
  }
}

void RS485_Read()
{
  String RS485_Read = "";
  Correct_Command = false;

  while (RS485_Serial.available())
  {
    char byteRec = RS485_Serial.read();
    if (byteRec == '/')
    {
      RS485_Read = "";
    }
    RS485_Read = RS485_Read + byteRec;
    if (byteRec == ':')
    {
      break;
    }
  }

  if (RS485_Read.substring(0, 10) == "/.485.GET.")
  {
    if (RS485_Read.substring(10, 11) == String(Module_Number))
    {
      if (RS485_Read.substring(11, 12) == ".")
      {
        Memory_Number = RS485_Read.substring(12, 14);
        if (RS485_Read.substring(14, 15) == ":")
        {
          Memorys_to_GET();
        }
      }
    }
  }

  if (RS485_Read.substring(0, 10) == "/.485.SET.")
  {
    if (RS485_Read.substring(10, 11) == String(Module_Number))
    {
      if (RS485_Read.substring(11, 12) == ".")
      {
        Memory_Number = RS485_Read.substring(12, 14);
        if (RS485_Read.substring(14, 15) == "=")
        {
          Memory_Value = RS485_Read.substring(15, 19);
          if (RS485_Read.substring(19, 20) == ":")
          {
            Memorys_to_SET();
          }
        }
      }
    }
  }

  if (RS485_Read.substring(0, 6) == "/.485.")
  {
    if (RS485_Read.substring(10, 11) == String(Module_Number))
    {
      if (RS485_Read.substring(11, 12) == ".")
      {
        if (Monitor_Protocol == true)
        {
          Serial.print("Received: ");
          Serial.println(RS485_Read);
        }

        Command_Type = RS485_Read.substring(0, 10);

        RS485_Write();
      }
    }
  }
}

void setup()
{
  RS485_Serial.begin(9600);
  pinMode(RS485_Control, OUTPUT);
  digitalWrite(RS485_Control, RS485_Receive);

  Serial.begin(9600);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
  Process();
  RS485_Read();
} // loop()
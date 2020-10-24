////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - Slave RS485
//   Software for Arduino NANO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 1; // Replace with the ID of Module

#include <Arduino.h> // necessário para o platform.io
#include <SoftwareSerial.h>

int RS485_R_RO_Pin = 4;
int RS485_R_DI_Pin = 5;
SoftwareSerial RS485_Receive(RS485_R_RO_Pin, RS485_R_DI_Pin);

bool Monitor_Program = false;
bool Monitor_Protocol = false;

bool Correct_Command = false;
String Command_Type = "/XXX.";
String Memory_Number = "XX";
String Memory_Value = "XXXX";

unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

String Memory_01 = "XXXX";

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

void Memorys_of_RS485(int Module_Number, String Memory_Number, String Memory_Value)
{
  if (Module_Number == 1)
  {
    if (Memory_Number == "01")
    {
      if (Memory_Value != "XXXX")
      {
        Memory_01 = Memory_Value;
      }
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
  digitalWrite(13, Memory_01.toInt());
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
  RS485_Receive.begin(9600);

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
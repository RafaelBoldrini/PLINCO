////////////////////////////////////////////////////////////////////////
//
//   Converter RS485
//   Software for Arduino UNO
//
////////////////////////////////////////////////////////////////////////

int Module_Number = 2; // Replace with the ID of Module

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
String Memory_02 = "XXXX";
String Memory_03 = "XXXX";

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
    Memory_01 = Value_Format(analogRead(A0));
    Memory_02 = Value_Format(analogRead(A1));
    Memory_03 = Value_Format(analogRead(A2));

    RS485_Write(Module_Number, "01", Memory_01);
    RS485_Write(Module_Number, "02", Memory_02);
    RS485_Write(Module_Number, "03", Memory_03);
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
    Serial.print("  ");
    Serial.print("M_03 = ");
    Serial.print(Memory_03);
    Serial.println("");
  }
}

void setup()
{
  RS485_Transmit.begin(9600);

  Serial.begin(9600);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
  Process();
} // loop()
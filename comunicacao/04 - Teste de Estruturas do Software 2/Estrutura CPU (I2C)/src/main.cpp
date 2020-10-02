///////////////////////////////////////////////////////////////////////////////
//
// Comunicação entre um arduino principal
// e vários arduinos secundários
//
// Codigo para arduino PRINCIPAL
// Arduino MEGA2560
//
///////////////////////////////////////////////////////////////////////////////

#include <Arduino.h> // necessário para o platform.io
#include <Wire.h>

String Module_and_Port = "";
String Command = "";
String Wire_Read = ""; // commando recebido
int Value = 0;

int Led_1_1 = 0;
int Led_2_1 = 0;
int Potenciometro_3_1 = 0;

bool Timer_1_Set = false;
unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

int Process_1_Control = 0;

bool Monitor_Program = false;
bool Monitor_Protocol = false;

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

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

void Wait_Answer(int Module_Number)
{
  Wire_Read = "";
  Value = 0;

  Wire.requestFrom(Module_Number, 9); // (slave device, number max of bytes)
  while (Wire.available())
  {
    char byteRec = Wire.read();
    Wire_Read = Wire_Read + byteRec;
  }

  if (Wire_Read.substring(0, 1) == "/" && Wire_Read.substring(8, 9) == ";")
  {
    if (Wire_Read.substring(3, 4) == ".")
    {
      if (Wire_Read.substring(1, 2) == "0")
      {
        Value = Wire_Read.substring(4, 8).toInt();
      }
    }
  }

  if (Monitor_Protocol == true)
  {
    Serial.print("Received: ");
    Serial.println(Wire_Read);
  }
}

void Send_Command(int Module_Number, int Port_Number, String Type_Info)
{
  Module_and_Port = String(Module_Number) + "." + String(Port_Number);
  Command = "/" + Module_and_Port + "." + Type_Info + ";";

  Wire.beginTransmission(Module_Number);
  Wire.write(Command.c_str());
  Wire.endTransmission();

  if (Monitor_Protocol == true)
  {
    Serial.print("Sent: ");
    Serial.println(Command);
  }

  Wait_Answer(Module_Number);
}

void Timer_1(int Interval)
{
  unsigned long currentMillis = millis();

  if (Timer_1_Set == false)
  {
    Timer_1_Control = currentMillis;
    Timer_1_Value = 1;
    Timer_1_Set = true;
  }

  if (Timer_1_Set == true)
  {
    if ((currentMillis - Timer_1_Control) > Interval)
    {
      Timer_1_Value = 0;
      Timer_1_Set = false;
    }
  }
}

void Information()
{
  Send_Command(3, 1, "IA");
  Potenciometro_3_1 = Value;
}

void Process()
{
  if (Process_1_Control == 0)
  {
    Process_1_Control = 1;
  }

  if (Process_1_Control == 1)
  {
    Send_Command(1, 1, "OH");
    Led_1_1 = Value;
    Timer_1(Potenciometro_3_1);
    if (Timer_1_Value == 0)
    {
      Send_Command(1, 1, "OL");
      Led_1_1 = Value;
      Process_1_Control = 2;
    }
  }

  if (Process_1_Control == 2)
  {
    Send_Command(2, 1, "OH");
    Led_2_1 = Value;
    Timer_1(Potenciometro_3_1);
    if (Timer_1_Value == 0)
    {
      Send_Command(2, 1, "OL");
      Led_2_1 = Value;
      Process_1_Control = 0;
    }
  }
}

void Program_Monitor()
{
  if (Monitor_Program == true)
  {
    Serial.print("Potenciometro = ");
    Serial.print(Potenciometro_3_1);
    Serial.print("     ");
    Serial.print("Led 1 = ");
    Serial.print(Led_1_1);
    Serial.print("     ");
    Serial.print("Led 2 = ");
    Serial.print(Led_2_1);
    Serial.println("     ");
  }
}

///////////////////////////////////////////////////////////////////////////////

void setup()
{
  Wire.begin();
  Serial.begin(9600);
} // setup()

void loop()
{
  Monitor_Control();
  Information();
  Process();
  Program_Monitor();
} // loop()
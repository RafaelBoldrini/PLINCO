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
String Serial_Read = ""; // commando recebido
int Value = 0;

int Potenciometro1 = 0;
int Potenciometro2 = 0;

bool Timer_1_Set = false;
unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

int Process_1_Control = 0;

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

void Wait_Answer(int Module_Number)
{
  Serial_Read = "";
  Value = 0;

  Wire.requestFrom(Module_Number, 9); // (slave device, number max of bytes)
  while (Wire.available())
  {
    char byteRec = Wire.read();
    Serial_Read = Serial_Read + byteRec;
  }

  if (Serial_Read.substring(0, 1) == "/" && Serial_Read.substring(8, 9) == ";")
  {
    if (Serial_Read.substring(3, 4) == ".")
    {
      if (Serial_Read.substring(1, 2) == "0")
      {
        Value = Serial_Read.substring(4, 8).toInt();
      }
    }
  }

  Serial.print("Received: ");
  Serial.println(Serial_Read);
}

void Send_Command(int Module_Number, int Port_Number, String Type_Info)
{
  Module_and_Port = String(Module_Number) + "." + String(Port_Number);
  Command = "/" + Module_and_Port + "." + Type_Info + ";";

  Wire.beginTransmission(Module_Number);
  Wire.write(Command.c_str());
  Wire.endTransmission();

  Serial.print("Sent: ");
  Serial.println(Command);

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

void Process_1()
{
  if (Process_1_Control == 0)
  {
    Process_1_Control = 1;
  }

  if (Process_1_Control == 1)
  {
    Send_Command(1, 1, "OH");
    Timer_1(1000);
    if (Timer_1_Value == 0)
    {
      Process_1_Control = 2;
    }
  }

  if (Process_1_Control == 2)
  {
    Send_Command(1, 1, "OL");
    Timer_1(1000);
    if (Timer_1_Value == 0)
    {
      Process_1_Control = 0;
    }
  }
}

void Inputs()
{
  Send_Command(2, 1, "IA");
  Potenciometro1 = Value;

  Send_Command(3, 1, "IA");
  Potenciometro2 = Value;
}

void Outputs()
{
  Process_1();
}

///////////////////////////////////////////////////////////////////////////////

void setup()
{
  Wire.begin();
  Serial.begin(9600);
} // setup()

void loop()
{
  Inputs();
  Outputs();
  while (Serial.available())
  {
    char x = Serial.read();
    Serial.print("-------------------------------------- key: ");
    Serial.println(x);
  }
} // loop()

//Fazer com que o monitoramento dos comandos seja opcional
//Fazer com que tenha um monitoramento de informações (por exemplo dos potenciometros)
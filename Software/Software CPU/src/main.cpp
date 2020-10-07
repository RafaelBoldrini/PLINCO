////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - CPU
//   Software for Arduino MEGA
//
////////////////////////////////////////////////////////////////////////

#include <Arduino.h> // necessário para o platform.io
#include <Wire.h>

bool Monitor_Program = false;
bool Monitor_Protocol = false;

String Memory_00 = "0000";

unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

int Process_1_Control = 0;

String Led_1 = "XXXX";
String Led_2 = "X";
String Led_3 = "X";
String Potenciometro_1 = "XXXX";
String Potenciometro_2 = "XXXX";
String Botao_1 = "XXXX";

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

void GET(int Module_Number, String Memory_Number, String Reference)
{
  String Command = "/GET." + String(Module_Number) + "." + Memory_Number + ":";

  Wire.beginTransmission(Module_Number);
  Wire.write(Command.c_str());
  Wire.endTransmission();

  if (Monitor_Protocol == true)
  {
    Serial.print("Sent: ");
    Serial.println(Command);
  }

  String Wire_Read = "";
  Memory_00 = "XXXX";

  Wire.requestFrom(Module_Number, 10); // (ID of Module, Number of Bytes)
  while (Wire.available())
  {
    char byteRec = Wire.read();
    Wire_Read = Wire_Read + byteRec;
  }

  if (Wire_Read.substring(0, 1) == String(Module_Number))
  {
    if (Wire_Read.substring(1, 2) == ".")
    {
      if (Wire_Read.substring(2, 4) == Memory_Number)
      {
        if (Wire_Read.substring(4, 5) == ".")
        {
          if (Wire_Read.substring(9, 10) == ";")
          {
            Memory_00 = Wire_Read.substring(5, 9);
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

  if (Memory_00 == "XXXX")
  {
    Memory_00 = Reference;
  }
}

void SET(int Module_Number, String Memory_Number, String Memory_Value, String Reference)
{
  String Command = "/SET." + String(Module_Number) + "." + Memory_Number + "=";
  Command = Command + Memory_Value + ":";

  Wire.beginTransmission(Module_Number);
  Wire.write(Command.c_str());
  Wire.endTransmission();

  if (Monitor_Protocol == true)
  {
    Serial.print("Sent: ");
    Serial.println(Command);
  }

  String Wire_Read = "";
  Memory_00 = "XXXX";

  Wire.requestFrom(Module_Number, 10); // (ID of Module, Number of Bytes)
  while (Wire.available())
  {
    char byteRec = Wire.read();
    Wire_Read = Wire_Read + byteRec;
  }

  if (Wire_Read.substring(0, 1) == String(Module_Number))
  {
    if (Wire_Read.substring(1, 2) == ".")
    {
      if (Wire_Read.substring(2, 4) == Memory_Number)
      {
        if (Wire_Read.substring(4, 5) == ".")
        {
          if (Wire_Read.substring(9, 10) == ";")
          {
            Memory_00 = Wire_Read.substring(5, 9);
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

  if (Memory_00 == "XXXX")
  {
    Memory_00 = Reference;
  }
  if (Memory_00 == "DONE")
  {
    Memory_00 = Memory_Value;
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

void Information()
{
  GET(3, "01", Potenciometro_1);
  Potenciometro_1 = Memory_00;

  GET(3, "02", Potenciometro_2);
  Potenciometro_2 = Memory_00;

  // GET(3, "01", Botao_1);
  // Botao_1 = Memory_00;
}

void Process()
{
  Timer_1(Potenciometro_1.toInt());

  SET(4, "03", Potenciometro_1, Potenciometro_1);
  SET(4, "04", Potenciometro_2, Potenciometro_2);

  String Led_1_Valor = Value_Format(Potenciometro_2.toInt() / 4);
  SET(1, "01", Led_1_Valor, Led_1);
  Led_1 = Memory_00;

  if (Process_1_Control == 0)
  {
    Process_1_Control = 1;
  }

  if (Process_1_Control == 1)
  {
    SET(2, "01", "0001", Led_2);
    Led_2 = Memory_00;
    Process_1_Control = 2;
    Timer_1_Set();
  }
  if (Process_1_Control == 2 && Timer_1_Value == 0)
  {
    SET(2, "01", "0000", Led_2);
    Led_2 = Memory_00;
    Process_1_Control = 3;
  }

  if (Process_1_Control == 3)
  {
    SET(2, "02", "0001", Led_3);
    Led_3 = Memory_00;
    Process_1_Control = 4;
    Timer_1_Set();
  }
  if (Process_1_Control == 4 && Timer_1_Value == 0)
  {
    SET(2, "02", "0000", Led_3);
    Led_3 = Memory_00;
    Process_1_Control = 1;
  }


  // String Led_1_Valor = Value_Format(Botao_1.toInt());
  // SET(2, "03", Led_1_Valor, Led_1);
  // Led_1 = Memory_00;
}

void Program_Monitor()
{
  if (Monitor_Program == true)
  {
    Serial.print("Potenciometro 1 = ");
    Serial.print(Potenciometro_1);
    Serial.print("\t");
    Serial.print("Potenciometro 2 = ");
    Serial.print(Potenciometro_2);
    Serial.print("\t");
    Serial.print("Led 1 = ");
    Serial.print(Led_1);
    Serial.print("\t");
    Serial.print("Led 2 = ");
    Serial.print(Led_2.toInt());
    Serial.print("\t");
    Serial.print("Led 3 = ");
    Serial.print(Led_3.toInt());
    Serial.println("");
    
    // Serial.print("Botao 1 = ");
    // Serial.print(Botao_1.toInt());
    // Serial.println("");
  }
}

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
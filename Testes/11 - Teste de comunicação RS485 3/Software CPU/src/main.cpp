////////////////////////////////////////////////////////////////////////
//
//   ArduinoPLC - CPU
//   Software for Arduino UNO
//
////////////////////////////////////////////////////////////////////////

#include <Arduino.h> // necessário para o platform.io
#include <Wire.h>
#include <SoftwareSerial.h>

int RS485_T_RO_Pin = 2;
int RS485_T_DI_Pin = 3;
int RS485_R_RO_Pin = 4;
int RS485_R_DI_Pin = 5;
SoftwareSerial RS485_Transmit(RS485_T_RO_Pin, RS485_T_DI_Pin);
SoftwareSerial RS485_Receive(RS485_R_RO_Pin, RS485_R_DI_Pin);

String Memory_00 = "XXXX";

bool Monitor_Program = false;
bool Monitor_Protocol = false;

unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

int Process_1_Control = 0;

String Led_1 = "XXXX";
String Led_2 = "XXXX";
String Led_3 = "XXXX";
String Potenciometro_1 = "XXXX";
String Potenciometro_2 = "XXXX";
String Sensor_Distancia_1 = "XXXX";
String Input_RS485_1 = "XXXX";
String Input_RS485_2 = "XXXX";
String Input_RS485_3 = "XXXX";

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
        if (Wire_Read.substring(4, 5) == "=")
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

void Memorys_of_RS485(int Module_Number, String Memory_Number, String Memory_Value)
{
  if (Module_Number == 1)
  {
    if (Memory_Number == "01")
    {
      if (Memory_Value == "XXXX")
      {
        Memory_Value = Sensor_Distancia_1;
      }
      Sensor_Distancia_1 = Memory_Value;
    }
  }

  if (Module_Number == 2)
  {
    if (Memory_Number == "01")
    {
      if (Memory_Value == "XXXX")
      {
        Memory_Value = Input_RS485_1;
      }
      Input_RS485_1 = Memory_Value;
    }
    if (Memory_Number == "02")
    {
      if (Memory_Value == "XXXX")
      {
        Memory_Value = Input_RS485_2;
      }
      Input_RS485_2 = Memory_Value;
    }
    if (Memory_Number == "03")
    {
      if (Memory_Value == "XXXX")
      {
        Memory_Value = Input_RS485_3;
      }
      Input_RS485_3 = Memory_Value;
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
  Timer_1(Potenciometro_1.toInt());

  GET(3, "01", Potenciometro_1);
  Potenciometro_1 = Memory_00;

  GET(3, "02", Potenciometro_2);
  Potenciometro_2 = Memory_00;

  SET(4, "03", Potenciometro_1, Potenciometro_1);
  SET(4, "04", Potenciometro_2, Potenciometro_2);

  if (Potenciometro_1.toInt() >= 1000)
  {
    SET(2, "01", "0000", Led_1);
    Led_1 = Memory_00;
    SET(2, "02", "0000", Led_2);
    Led_2 = Memory_00;
    RS485_Write(1, "01", "0000");
    Led_3 = "0000";
    Process_1_Control = 0;
  }
  if (Potenciometro_1.toInt() < 1000)
  {
    if (Process_1_Control == 0 && Timer_1_Value == 0)
    {
      Process_1_Control = 1;
      Timer_1_Set();
    }
    if (Process_1_Control == 1 && Timer_1_Value == 1)
    {
      SET(2, "01", "0001", Led_1);
      Led_1 = Memory_00;
      SET(2, "02", "0000", Led_2);
      Led_2 = Memory_00;
      RS485_Write(1, "01", "0001");
      Led_3 = "0001";
    }
    if (Process_1_Control == 1 && Timer_1_Value == 0)
    {
      Process_1_Control = 2;
      Timer_1_Set();
    }
    if (Process_1_Control == 2 && Timer_1_Value == 1)
    {
      SET(2, "01", "0000", Led_1);
      Led_1 = Memory_00;
      SET(2, "02", "0001", Led_2);
      Led_2 = Memory_00;
      RS485_Write(1, "01", "0000");
      Led_3 = "0000";
    }
    if (Process_1_Control == 2 && Timer_1_Value == 0)
    {
      Process_1_Control = 0;
    }
  }
}

void Program_Monitor()
{
  if (Monitor_Program == true)
  {
    Serial.print("Potenciometro 1 = ");
    Serial.print(Potenciometro_1);
    Serial.print("  ");
    Serial.print("Potenciometro 2 = ");
    Serial.print(Potenciometro_2);
    Serial.print("  ");
    Serial.print("Led 1 = ");
    Serial.print(Led_1.toInt());
    Serial.print("  ");
    Serial.print("Led 2 = ");
    Serial.print(Led_2.toInt());
    Serial.print("  ");
    Serial.print("Sensor 1 = ");
    Serial.print(Sensor_Distancia_1);
    Serial.print("  ");
    Serial.print("I_RS485 1 = ");
    Serial.print(Input_RS485_1);
    Serial.print("  ");
    Serial.print("I_RS485 2 = ");
    Serial.print(Input_RS485_2);
    Serial.print("  ");
    Serial.print("I_RS485 3 = ");
    Serial.print(Input_RS485_3);
    Serial.print("  ");
    Serial.print("Led 3 = ");
    Serial.print(Led_3.toInt());
    Serial.println("");
  }
}

void setup()
{
  Wire.begin();

  RS485_Transmit.begin(9600);
  RS485_Receive.begin(9600);

  Serial.begin(9600);
} // setup()

void loop()
{
  Monitor_Control();
  Program_Monitor();
  Process();
  RS485_Read();
} // loop()
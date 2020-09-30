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
#include <SoftwareSerial.h>

SoftwareSerial Serial_1(2, 3); // software Serial no NANO: pinos 2 (RX) e 3 (TX)

String Module_and_Port = "";
String Command = "";
bool Waiting_Answer = false;
String Serial_Read = ""; // commando recebido
int Value = 0;

int Potenciometro1 = 0;
int Potenciometro2 = 0;

bool Timer_1_Set = false;
unsigned long Timer_1_Control = 0;
int Timer_1_Value = 0;

int Process_1_Control = 0;
unsigned long Wait_Answer_Control = 0;

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

void Wait_Answer(int Interval)
{
  unsigned long currentMillis = millis();
  Wait_Answer_Control = currentMillis;
  while (Waiting_Answer == true)
  {
    unsigned long currentMillis = millis();
    char byteRec = Serial_1.read();
    Serial_Read = Serial_Read + byteRec;
    if (byteRec == '/')
    {
      Serial_Read = "/";
    }
    if (Serial_Read.substring(0, 1) == "/" && byteRec == ';')
    {
      if (Serial_Read.substring(3, 4) == ".")
      {
        if (Serial_Read.substring(1, 3) == "0A")
        {
          Value = Serial_Read.substring(4, 8).toInt();
          Waiting_Answer = false;
        }
      }
      Serial.print("Received: ");
      Serial.println(Serial_Read);
      Serial_Read = "";
    }

    if ((currentMillis - Wait_Answer_Control) >= Interval)
    {
      Waiting_Answer = false;
      Serial.print("[Timeout] Received: ");
      Serial.println(Serial_Read);
      Serial_Read = "";
    }
  }
}

void Send_Command(int Module_Number, int Port_Number, String Type_Info)
{
  Module_and_Port = String(Module_Number) + "." + String(Port_Number);
  Command = "/" + Module_and_Port + "." + Type_Info + ";";
  Serial_1.print(Command);
  Serial.print("Sent: ");
  Serial.println(Command);
  Waiting_Answer = true;
  Wait_Answer(1000);
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
    if ((currentMillis - Timer_1_Control) >= Interval)
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
  Serial_1.begin(9600); // hardware Serial_1 no MEGA: pinos 18 (TX1) e 19 (RX1)
  Serial.begin(9600);
} // setup()

void loop()
{
  Inputs();
  Outputs();

//Campo de testes
//Serial_1.println("Teste");
//Serial.println("Teste");
//delay(100);
//char byteRec = Serial_1.read();
//Serial_Read = Serial_Read + byteRec;
//Serial.println(byteRec);
//Serial.println(Serial_Read);

} // loop()
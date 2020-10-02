///////////////////////////////////////////////////////////////////////////////
//
// Comunicação entre um arduino principal
// e vários arduinos secundários
//
// Codigo para arduino SECUNDÁRIO
// Arduino NANO
//
///////////////////////////////////////////////////////////////////////////////

#include <Arduino.h> // necessário para o platform.io
#include <Wire.h>

int Module_Number = 1;   // id do servo
String Serial_Read = ""; // commando recebido

String Command_Type = "";
bool Correct_Command = false;
int Info = 0;
String Info_Format = "";
String Info_to_Send = "";

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

void ReceiveEvent(int howMany)
{
  Serial_Read = "";
  Info = 0;

  while (Wire.available())
  {
    char byteRec = Wire.read();
    Serial_Read = Serial_Read + byteRec;
  }

  if (Serial_Read.substring(0, 1) == "/" && Serial_Read.substring(7, 8) == ";")
  {
    if (Serial_Read.substring(1, 2) == String(Module_Number))
    {
      if ((Serial_Read.substring(2, 3) == ".") && (Serial_Read.substring(4, 5) == "."))
      {
        if (Serial_Read.substring(5, 6) == "I")
        {
          Command_Type = Serial_Read.substring(5, 6);
          if (Serial_Read.substring(6, 7) == "A")
          {
            if (Serial_Read.substring(3, 4) == "1")
            {
              Correct_Command = true;
              Info = analogRead(A0);
            }
            if (Serial_Read.substring(3, 4) == "2")
            {
              Correct_Command = true;
              Info = analogRead(A1);
            }
            if (Serial_Read.substring(3, 4) == "3")
            {
              Correct_Command = true;
              Info = analogRead(A2);
            }
            if (Serial_Read.substring(3, 4) == "4")
            {
              Correct_Command = true;
              Info = analogRead(A3);
            }
            if (Serial_Read.substring(3, 4) == "5")
            {
              Correct_Command = true;
              Info = analogRead(A4);
            }
            if (Serial_Read.substring(3, 4) == "6")
            {
              Correct_Command = true;
              Info = analogRead(A5);
            }
            if (Serial_Read.substring(3, 4) == "7")
            {
              Correct_Command = true;
              Info = analogRead(A6);
            }
            if (Serial_Read.substring(3, 4) == "8")
            {
              Correct_Command = true;
              Info = analogRead(A7);
            }
          }
          if (Serial_Read.substring(6, 7) == "D")
          {
            if (Serial_Read.substring(3, 4) == "1")
            {
              Correct_Command = true;
              Info = digitalRead(A0);
            }
            if (Serial_Read.substring(3, 4) == "2")
            {
              Correct_Command = true;
              Info = digitalRead(A1);
            }
            if (Serial_Read.substring(3, 4) == "3")
            {
              Correct_Command = true;
              Info = digitalRead(A2);
            }
            if (Serial_Read.substring(3, 4) == "4")
            {
              Correct_Command = true;
              Info = digitalRead(A3);
            }
            if (Serial_Read.substring(3, 4) == "5")
            {
              Correct_Command = true;
              Info = digitalRead(A4);
            }
            if (Serial_Read.substring(3, 4) == "6")
            {
              Correct_Command = true;
              Info = digitalRead(A5);
            }
            if (Serial_Read.substring(3, 4) == "7")
            {
              Correct_Command = true;
              Info = digitalRead(A6);
            }
            if (Serial_Read.substring(3, 4) == "8")
            {
              Correct_Command = true;
              Info = digitalRead(A7);
            }
          }
        }
        if (Serial_Read.substring(5, 6) == "O")
        {
          Command_Type = Serial_Read.substring(5, 6);
          if (Serial_Read.substring(6, 7) == "H")
          {
            Correct_Command = true;
            digitalWrite(((Serial_Read.substring(3, 4)).toInt() + 3), HIGH);
            Info = 1;
          }
          if (Serial_Read.substring(6, 7) == "L")
          {
            Correct_Command = true;
            digitalWrite(((Serial_Read.substring(3, 4)).toInt() + 3), LOW);
            Info = 0;
          }
        }
      }
    }
  }

  Serial.print("Received: ");
  Serial.println(Serial_Read);
}

void RequestEvent()
{
  if (Correct_Command == false)
  {
    Info_to_Send = "ERROR";
  }

  if (Correct_Command == true)
  {
    Info_Format = String(Info);
    if (Info < 1000)
    {
      Info_Format = "0" + Info_Format;
    }
    if (Info < 100)
    {
      Info_Format = "0" + Info_Format;
    }
    if (Info < 10)
    {
      Info_Format = "0" + Info_Format;
    }
    Info_to_Send = "/0" + Command_Type + "." + Info_Format + ";";

    Correct_Command = false;
  }

  Wire.write(Info_to_Send.c_str());

  Serial.print("Sent: ");
  Serial.println(Info_to_Send);
}

///////////////////////////////////////////////////////////////////////////////

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
  //pinMode(A4, INPUT);
  //pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
} // setup()

void loop()
{
}
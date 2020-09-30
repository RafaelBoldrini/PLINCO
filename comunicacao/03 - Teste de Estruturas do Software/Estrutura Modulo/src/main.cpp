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
#include <SoftwareSerial.h>

String Module_Number = "1"; // id do servo
String Serial_Read = "";    // commando recebido

SoftwareSerial Serial_Bus(2, 3); // software Serial no NANO: pinos 2 (RX) e 3 (TX)

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial_Bus.begin(4800);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
} // setup()

void loop()
{
  while (Serial_Bus.available())
  {

    char byteRec = Serial_Bus.read();
    Serial_Read = Serial_Read + byteRec;
    if (byteRec == '/')
    {
      Serial_Read = "/";
    }
    if (Serial_Read.substring(0, 1) == "/" && byteRec == ';')
    {
      if (Serial_Read.substring(1, 2) == Module_Number)
      {
        if ((Serial_Read.substring(2, 3) == ".") && (Serial_Read.substring(4, 5) == "."))
        {
          if (Serial_Read.substring(5, 6) == "I")
          {
            if (Serial_Read.substring(6, 7) == "A")
            {
              if (Serial_Read.substring(3, 4) == "1")
              {
                analogRead(A0);
              }
              if (Serial_Read.substring(3, 4) == "2")
              {
                analogRead(A1);
              }
              if (Serial_Read.substring(3, 4) == "3")
              {
                analogRead(A2);
              }
              if (Serial_Read.substring(3, 4) == "4")
              {
                analogRead(A3);
              }
              if (Serial_Read.substring(3, 4) == "5")
              {
                analogRead(A4);
              }
              if (Serial_Read.substring(3, 4) == "6")
              {
                analogRead(A5);
              }
              if (Serial_Read.substring(3, 4) == "7")
              {
                analogRead(A6);
              }
              if (Serial_Read.substring(3, 4) == "8")
              {
                analogRead(A7);
              }
            }
            if (Serial_Read.substring(6, 7) == "D")
            {
              if (Serial_Read.substring(3, 4) == "1")
              {
                digitalRead(A0);
              }
              if (Serial_Read.substring(3, 4) == "2")
              {
                digitalRead(A1);
              }
              if (Serial_Read.substring(3, 4) == "3")
              {
                digitalRead(A2);
              }
              if (Serial_Read.substring(3, 4) == "4")
              {
                digitalRead(A3);
              }
              if (Serial_Read.substring(3, 4) == "5")
              {
                digitalRead(A4);
              }
              if (Serial_Read.substring(3, 4) == "6")
              {
                digitalRead(A5);
              }
              if (Serial_Read.substring(3, 4) == "7")
              {
                digitalRead(A6);
              }
              if (Serial_Read.substring(3, 4) == "8")
              {
                digitalRead(A7);
              }
            }
          }
          if (Serial_Read.substring(5, 6) == "O")
          {
            if (Serial_Read.substring(6, 7) == "H")
            {
              digitalWrite(((Serial_Read.substring(3, 4)).toInt() + 3), HIGH);
            }
            if (Serial_Read.substring(6, 7) == "L")
            {
              digitalWrite(((Serial_Read.substring(3, 4)).toInt() + 3), LOW);
            }
          }
        }
      }
      Serial_Read = "";
    }

  } // loop()
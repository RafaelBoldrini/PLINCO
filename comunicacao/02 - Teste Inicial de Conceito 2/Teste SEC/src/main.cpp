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

String ID = "1";        // id do servo
String Serial_Read = ""; // commando recebido
bool Serial_Done = false; // commando recebido

SoftwareSerial SerialH(2, 3); // software Serial no NANO: pinos 2 (RX) e 3 (TX)

#define LED (LED_BUILTIN) // pino 13 no arduino NANO

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

void cmd_liga()
{
  digitalWrite(LED, HIGH);
  Serial_Done = false;
}

void cmd_desl()
{
  digitalWrite(LED, LOW);
  Serial_Done = false;
}

///////////////////////////////////////////////////////////////////////////////
void setup()
{
  SerialH.begin(4800);
  pinMode(LED, OUTPUT);
} // setup()

void loop()
{
  while (SerialH.available())
  {

    char lido = SerialH.read();
    Serial_Read = Serial_Read + lido;
    if (lido == '/')
    {
      Serial_Read = "/";
    }
    if (lido == ';')
    {
      Serial_Done = true;
    }

    // parser de comandos
    if (Serial_Done == true && Serial_Read == "/" + ID + ".1;")
    {
      cmd_liga();
    }
    else if (Serial_Done == true && Serial_Read == "/" + ID + ".0;")
    {
      cmd_desl();
    }
  }
} // loop()
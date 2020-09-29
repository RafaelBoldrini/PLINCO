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

SoftwareSerial SerialH(2, 3); // software Serial no NANO: pinos 2 (RX) e 3 (TX)

#define LED (LED_BUILTIN) // pino 13 no arduino NANO

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

void cmd_liga()
{
  digitalWrite(LED, HIGH);
}

void cmd_desl()
{
  digitalWrite(LED, LOW);
}

///////////////////////////////////////////////////////////////////////////////
void setup()
{
  SerialH.begin(4800); 
  pinMode(LED, OUTPUT);
} // setup()

void loop()
{
  if (SerialH.available()) {

    char lido = SerialH.read();

   // parser de comandos
    if(lido == '0') {
      cmd_liga();
    }
    else if(lido == '1') {
      cmd_desl();
    }
  }
} // loop()
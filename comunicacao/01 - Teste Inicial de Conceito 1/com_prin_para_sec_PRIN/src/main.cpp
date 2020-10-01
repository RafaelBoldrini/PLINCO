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

#define LED (LED_BUILTIN) // pino 13 no arduino MEGA
#define DT (1000)

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

void cmd_liga()
{
  Serial1.write('1');
  digitalWrite(LED, HIGH);
}

void cmd_desl()
{
  Serial1.write('0');
  digitalWrite(LED, LOW);
}

///////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial1.begin(4800); // hardware Serial1 no MEGA: pinos 18 (TX1) e 19 (RX1)
  pinMode(LED, OUTPUT);
} // setup()

void loop()
{
  cmd_liga();
  delay(DT);

  cmd_desl();
  delay(DT);
} // loop()

// TODO: será que oarduino envia o dado da serial imediatamente ou aguarda encher o buffer?
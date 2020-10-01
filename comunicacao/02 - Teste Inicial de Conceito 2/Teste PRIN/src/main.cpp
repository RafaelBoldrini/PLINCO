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

String comando = "";

#define LED (LED_BUILTIN) // pino 13 no arduino MEGA
#define DT (250)

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

void cmd(int Servo_ID, int Power)
{
  comando = "/" + String(Servo_ID) + "." + String(Power) + ";";
  Serial1.print(comando);
}

///////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial1.begin(4800); // hardware Serial1 no MEGA: pinos 18 (TX1) e 19 (RX1)
  pinMode(LED, OUTPUT);
} // setup()

void loop()
{
  digitalWrite(LED, HIGH);
  delay(DT);
  digitalWrite(LED, LOW);
  delay(DT);

  cmd(1,1);
  delay(DT);
  cmd(1,0);
  delay(DT);
  
  cmd(2,1);
  delay(DT);
  cmd(2,0);
  delay(DT);
  
  cmd(3,1);
  delay(DT);
  cmd(3,0);
  delay(DT);
  
  cmd(4,1);
  delay(DT);
  cmd(4,0);
  delay(DT);
} // loop()

// TODO: será que o arduino envia o dado da serial imediatamente ou aguarda encher o buffer?
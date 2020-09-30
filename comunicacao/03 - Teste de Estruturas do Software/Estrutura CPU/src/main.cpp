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

String Module_and_Port = "";
String Command = "";

#define LED (LED_BUILTIN) // pino 13 no arduino MEGA
#define DT (250)

///////////////////////////////////////////////////////////////////////////////
// funções devem ser declaradas antes de ser chamadas
///////////////////////////////////////////////////////////////////////////////

void Send_Command(int Module_Number, int Port_Number, String Type_Info)
{
  Module_and_Port = String(Module_Number) + "." + String(Port_Number);
  Command = "/" + Module_and_Port + "." + Type_Info + ";";
  Serial1.print(Command);
}

///////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial1.begin(4800); // hardware Serial1 no MEGA: pinos 18 (TX1) e 19 (RX1)
} // setup()

void loop()
{
  Send_Command(1,1,"OH");
  delay(DT);
  Send_Command(1,1,"OL");
  delay(DT);
  
  Send_Command(2,1,"OH");
  delay(DT);
  Send_Command(2,1,"OL");
  delay(DT);
  
  Send_Command(3,1,"OH");
  delay(DT);
  Send_Command(3,1,"OL");
  delay(DT);

} // loop()

// Fazer o Serial1 pedir um input e entrar em um while com milles e IFs 
//   para esperar o retorno correto antes de mandar outro comando
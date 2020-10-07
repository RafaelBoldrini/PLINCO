//Programa: Modulo CPU (A00)

//Configuração de comunicacao serial do modulo RS485
#include <SoftwareSerial.h>
#define Pino_RS485_RO    9
#define Pino_RS485_DI    8
#define RS485Control     7
#define RS485Transmit    HIGH
#define RS485Receive     LOW
SoftwareSerial RS485Serial(Pino_RS485_RO, Pino_RS485_DI);
//Configuração de variaveis para o RS485
String StringRS485="";
String SubStringRS485="";
int ReadDone = 0;

String I01="";
String I02="";
String O01="";




void GetData(String AXX, String Name_I_O)
{
  //Configura o modulo para mandar dados
  digitalWrite(RS485Control, RS485Transmit);
  int RepeatSend = 0;
  while(RepeatSend <= 0)
  {
    //Envia a string para algum modulo
    RS485Serial.print("S=");
    RS485Serial.print("<PING.");
    RS485Serial.print(AXX);
    RS485Serial.print(".PING>");
    RS485Serial.print(";");
    //Manda a string para o monitor
    Serial.print("String enviada: ");
    Serial.print("<PING.");
    Serial.print(AXX);
    Serial.print(".PING>");
    Serial.println(";");
    //Controla o tempo de envio
    RepeatSend = RepeatSend+1;
  }
}



//Fazer uma função de SendData que manda apenas um dado
//Fazer uma função de GetData que pinga e recebe apenas um dado




void SendPing(String AXX)
{
  //Configura o modulo para mandar dados
  digitalWrite(RS485Control, RS485Transmit);
  int RepeatSend = 0;
  while(RepeatSend <= 0)
  {
    //Envia a string para algum modulo
    RS485Serial.print("S=");
    RS485Serial.print("<PING.");
    RS485Serial.print(AXX);
    RS485Serial.print(".PING>");
    RS485Serial.print(";");
    //Manda a string para o monitor
    Serial.print("String enviada: ");
    Serial.print("<PING.");
    Serial.print(AXX);
    Serial.print(".PING>");
    Serial.println(";");
    //Controla o tempo de envio
    RepeatSend = RepeatSend+1;
  }
}



void ReadStringRS485()
{
  //Configura o modulo para receber dados
  digitalWrite(RS485Control, RS485Receive);
  StringRS485 = ""; ReadDone = 0;
  while(ReadDone == 0)
  {
    while(RS485Serial.available()) //&& ReadDone == 0)
    {
      //Recebe os dados e monta a string
      char inChar = (char)RS485Serial.read();
      StringRS485 += inChar;
      //Identifica iniciador da string
      if(inChar == '=')
      {
        StringRS485 = "";
      }
      //Identifica terminador da string
      if(inChar == ';')
      {
        ReadDone = 1;
      }
    }
  }
  //Manda a string para o monitor
  Serial.print("String recebida: ");
  Serial.println(StringRS485);
}



void GetData(String Name_I_O)
{
  //Verifica a string recebida
  if(StringRS485.indexOf("<"+Name_I_O+".") >= 0
  && StringRS485.indexOf("."+Name_I_O+">") >= 0)
  {
    //Identifica o dado recebido
    SubStringRS485 = StringRS485.substring(
    StringRS485.indexOf("<"+Name_I_O+".")+5,
    StringRS485.indexOf("."+Name_I_O+">"));
  }
}





void SendString(String Name_I_O, String Value_I_O)
{
  //Configura o modulo para mandar dados
  digitalWrite(RS485Control, RS485Transmit);
  int RepeatSend = 0;
  while(RepeatSend <= 0)
  {
    //Envia a string para algum modulo
    RS485Serial.print("S=");
    RS485Serial.print("<"+Name_I_O+".");
    RS485Serial.print(Value_I_O);
    RS485Serial.print("."+Name_I_O+">");
    RS485Serial.print(";");
    //Manda a string para o monitor
    Serial.print("String enviada: ");
    Serial.print("<"+Name_I_O+".");
    Serial.print(Value_I_O);
    Serial.print("."+Name_I_O+">");
    Serial.println(";");
    //Controla o tempo de envio
    RepeatSend = RepeatSend+1;
  }
}

      


void setup()
{
  //Inicializa a serial do Arduino
  Serial.begin(9600);
  
  //Inicializa a serial do modulo RS485
  RS485Serial.begin(4800);
  pinMode(RS485Control, OUTPUT);
  
  //Configura os pinos usados
}



void loop()
{
  SendPing("A01");
  ReadStringRS485();
  if(ReadDone == 1)
  {
    GetData("I01");
    I01 = SubStringRS485;
    GetData("I02");
    I02 = SubStringRS485;
  }
  
//----------------------------------------------------------------
      //Executa logica do processo
      if(I01 == "0" && I02 == "1")
      {
        O01 = "1";
      }
      if(I01 == "1" && I02 == "1")
      {
        O01 = "0";
      }
      if(I02 == "0")
      {
        O01 = "0";
      }
//----------------------------------------------------------------
  
  SendString("O01", O01);
  Serial.println("---");
  delay(500);
}

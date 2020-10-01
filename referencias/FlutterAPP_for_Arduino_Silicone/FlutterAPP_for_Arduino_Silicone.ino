#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 200); String Ip = "192.168.0.200";
EthernetServer server(9000); String Port = "9000";

int Serial_Mode = 0;
char byteRec = ""; 
String Serial_Read = ""; 
String Ethernet_Read = ""; String Request = "";
int Ethernet_Read_Erro = 0;
String Ethernet_Write = "";
int Led_Process = 0;
int Led_Control = 1; int Led_Flag = 1; 

int interval = 250; unsigned long previousMillis = 0;

int AnalogInput_Pin = A0; 
int Led_1_Pin = 2;
int Led_2_Pin = 3;
int Led_3_Pin = 4;
int Led_State = 0;

void setup(){

  Pin_Mode();
  
  Server_Begin();

  Serial_Begin();
  
}

void loop(){
  
  Monitor_Commands();

  Ethernet_Commands();

  Processes();
  
}

//-------------------------------------------------------------------------------------------------------------

void Server_Begin(){
  
  Ethernet.begin(mac, ip);

  // Verifica se o Hardware esta fuincionando
  if(Ethernet.hardwareStatus() == EthernetNoHardware){
    Serial.print("\t"); Serial.println("Hardware Ethernet não foi encontrado");
    Serial.println();
    while(true){delay(1);}
  }

  // Verifica a conexão Ethernet
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.print("\t"); Serial.println("Conexão Ethernet não foi encontrada");
    Serial.println();
  }
  
  server.begin();
  
}

void Pin_Mode(){
  
  pinMode(AnalogInput_Pin, INPUT_PULLUP);
  pinMode(Led_1_Pin, OUTPUT);
  pinMode(Led_2_Pin, OUTPUT);
  pinMode(Led_3_Pin, OUTPUT);
  
}

void Serial_Begin(){
  
  Serial.begin(9600);
  Serial.println("--------------------------------------------------------------"); Serial.println();
  Serial.print("\t"); Serial.println("Arduino PLC /// IP: " + Ip + " Port: " + Port); Serial.println();
  
}

void Monitor_Commands(){
  
  while(Serial.available()){
    byteRec = Serial.read(); 
    Serial_Read = (Serial_Read+byteRec);
    if(byteRec == '/'){Serial_Read = "/";}
    if(byteRec == ';'){
      
      if(Serial_Read == "/SM1;"){Serial_Mode = 1; 
        Serial.print("\t"); Serial.println("Serial_Mode On (Simple Monitoring)");
        Serial.println();
      }
      
      if(Serial_Read == "/SM2;"){Serial_Mode = 2; 
        Serial.print("\t"); Serial.println("Serial_Mode On (Greater Monitoring)");
        Serial.println();
      }
      
      if(Serial_Read == "/SM0;"){Serial_Mode = 0; 
        Serial.print("\t"); Serial.println("Serial_Mode Off (Monitoring Disabled)");
        Serial.println();
      }
      
      //if(Serial_Read == "IP_Config"){IP_Config = 1;}
      
      Serial_Read = "";
    }
  }
}

void Ethernet_Commands(){
  
  //Entende que o Cliente começou a mandar dados
  EthernetClient client = server.available();
  if(client){
    
    //Espera até todos os dados do Cliente chegar
    while(client.available()){
      byteRec = client.read();
      Ethernet_Read = (Ethernet_Read+byteRec);
      Request = (Request+byteRec);

      //Informa ao monitor os dados que estão vindo do Cliente
      if(Serial_Mode == 2){
        Serial.print("\t"); Serial.print("Cliente IP: "); 
        Serial.print("\t"); Serial.print(client.remoteIP()); 
        Serial.print("\t"); Serial.print(" -> "); Serial.println(byteRec);
      }
      
      //Limita a quantidados que o Cliente pode mandar
      if(Ethernet_Read.length() >= 20){Ethernet_Read_Erro = 1;
          
      }

      //Identifica o inicio e o fim de um comando valido
      if(byteRec == '/'){Request = "/";}
      if(byteRec == ';' && Ethernet_Read_Erro == 0){
        
        if(Request == "/START;"){Led_Process = 1;
          Ethernet_Write = "/LED PROCESS STARTED;";
        }
        
        if(Request == "/STOP;"){Led_Process = 0;
          Ethernet_Write = "/LED PROCESS STOPPED;";
        }

        if(Request == "/INFO;"){
          Ethernet_Write = "/A0." + String(analogRead(AnalogInput_Pin)) + ";";
        }

        client.print(Ethernet_Write);
      }
    }

    //Informa ao monitor os dados que estão vindo do Cliente
    if(Serial_Mode == 1){
      Serial.print("\t"); Serial.print("Cliente IP: "); 
      Serial.print("\t"); Serial.print(client.remoteIP()); 
      Serial.print("\t"); Serial.print(" -> "); Serial.println(Ethernet_Read);
    }
  
    if((Serial_Mode == 1 || Serial_Mode == 2) && Ethernet_Read_Erro == 0){
      Serial.print("\t"); Serial.print("Server IP: "); 
      Serial.print("\t"); Serial.print(Ethernet.localIP()); 
      Serial.print("\t"); Serial.print(" -> "); Serial.println(Ethernet_Write);
      Serial.println();
    }

    //Informa ao monitor os dados que estão vindo do Cliente
    if(Ethernet_Read_Erro == 1){
      Serial.print("\t"); Serial.print("Muitas informações erradas recebidas");
      Serial.println(" -> Desativando Conexão");
      Serial.println();
      Ethernet_Read_Erro = 0; client.stop();
    }
    
    byteRec = ""; Ethernet_Read = ""; Request = ""; Ethernet_Write = "";
  }
}

void Processes(){
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;

    if(Led_Process == 1){

      if(Led_Control == 1 && Led_Flag == 0){
        digitalWrite(Led_1_Pin,1);
        digitalWrite(Led_2_Pin,0);
        digitalWrite(Led_3_Pin,0);
        Led_Control = Led_Control+1;
        Led_Flag = 1;
      }
      
      if(Led_Control == 2 && Led_Flag == 0){
        digitalWrite(Led_1_Pin,0);
        digitalWrite(Led_2_Pin,1);
        digitalWrite(Led_3_Pin,0);
        Led_Control = Led_Control+1;
        Led_Flag = 1;
      }
      
      if(Led_Control == 3 && Led_Flag == 0){
        digitalWrite(Led_1_Pin,0);
        digitalWrite(Led_2_Pin,0);
        digitalWrite(Led_3_Pin,1);
        Led_Control = 1;
        Led_Flag = 1;
      }
      
      Led_Flag = 0;
    }
  }
}

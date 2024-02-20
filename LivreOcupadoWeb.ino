
#include <SoftwareSerial.h>

#define ledVerde 10
#define botaoVerde 9

#define ledVermelho 2
#define botaoVermelho 3

#define TIMEOUT 5000 
SoftwareSerial mySerial(6, 7); 

int varBotaoVerdeApertado=0;
int varBotaoVerdeLiberado=0;

int varBotaoVermelhoApertado=0;
int varBotaoVermelhoLiberado=0;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(botaoVerde, INPUT);

  pinMode(ledVermelho, OUTPUT);
  pinMode(botaoVermelho, INPUT);

 Serial.begin(9600);
 mySerial.begin(9600);
 
 SendCommand("AT+RST", "Ready");
 delay(5000);

 SendCommand("AT+CWMODE=1","OK");
 SendCommand("AT+CIFSR", "OK");
 SendCommand("AT+CIPMUX=1","OK");
 SendCommand("AT+CIPSERVER=1,80","OK");  

 Serial.println("Connected to wifi");
  
  acenderLedVerde();
  apagarLedVermelho();
}

void acenderLedVerde(){
  if(digitalRead(ledVerde)==LOW){
    digitalWrite(ledVerde,HIGH);
  }
}

void apagarLedVerde(){
  if(digitalRead(ledVerde)==HIGH){
    digitalWrite(ledVerde,LOW);
  }
}

void acenderLedVermelho(){
  if(digitalRead(ledVermelho)==LOW){
    digitalWrite(ledVermelho,HIGH);
  }
}

void apagarLedVermelho(){
  if(digitalRead(ledVermelho)==HIGH){
    digitalWrite(ledVermelho,LOW);
  }
}

void loop() {

 String IncomingString="";
 boolean StringReady = false;
 
 while (mySerial.available()){
   IncomingString=mySerial.readString();
   StringReady= true;
  }
 
  if (StringReady){
    Serial.println("Received String: " + IncomingString);

  if (IncomingString.indexOf("LIVRE=NAO") != -1) {
    acenderLedVermelho();
    apagarLedVerde();
    delay(1000);
    SendCommand("AT+CIPCLOSE=0","OK");
   }

  if (IncomingString.indexOf("LIVRE=SIM") != -1) {
    acenderLedVerde();
    apagarLedVermelho();
    delay(1000);
    SendCommand("AT+CIPCLOSE=0","OK");
   }
    
  }
   
  if (digitalRead(botaoVerde)==HIGH){
    varBotaoVerdeApertado=1;
    varBotaoVerdeLiberado=0;
    
    varBotaoVermelhoApertado=0;
    varBotaoVermelhoLiberado=0;
  }else{
    varBotaoVerdeLiberado=1;
  }

  if ((varBotaoVerdeApertado==1)and(varBotaoVerdeLiberado==1)and(digitalRead(ledVerde)==LOW)){
    varBotaoVerdeApertado=0;
    varBotaoVerdeLiberado=0;
    acenderLedVerde();
    apagarLedVermelho();
  }


  if (digitalRead(botaoVermelho)==HIGH){
    varBotaoVermelhoApertado=1;
    varBotaoVermelhoLiberado=0;

    varBotaoVerdeApertado=0;
    varBotaoVerdeLiberado=0;
  }else{
    varBotaoVermelhoLiberado=1;
  }

  if ((varBotaoVermelhoApertado==1)and(varBotaoVermelhoLiberado==1)and(digitalRead(ledVermelho)==LOW)){
    varBotaoVermelhoApertado=0;
    varBotaoVermelhoLiberado=0;
    acenderLedVermelho();
    apagarLedVerde();
  }

}

boolean SendCommand(String cmd, String ack){
  mySerial.println(cmd); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}
 
boolean echoFind(String keyword){
 byte current_char = 0;
 byte keyword_length = keyword.length();
 long deadline = millis() + TIMEOUT;
 while(millis() < deadline){
  if (mySerial.available()){
    char ch = mySerial.read();
    Serial.write(ch);
    if (ch == keyword[current_char])
      if (++current_char == keyword_length){
       Serial.println();
       return true;
    }
   }
  }
 return false; // Timed out
}

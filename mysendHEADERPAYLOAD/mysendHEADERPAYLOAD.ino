
#include <SoftwareSerial.h>


unsigned long sendtime=0;

byte  NODE_ID = 0;
byte RECIEVER_ID = 10;
byte END_MESSAGE[3] ={0xff,0xfe,0xff};

byte MESSAGE_ID = 0;

bool RETRANSLATE = false;
char PAYLOAD = 22;



SoftwareSerial mySerial(8, 9);
unsigned char messageToSend[8];
bool sendNewMessage = true;
void printMessage(){
  // Формируем массив сообщение
  //////////////////////HEADER
  // Если отправляем новое сообщение
  if (sendNewMessage){
    messageToSend[0] = MESSAGE_ID++;
    if (MESSAGE_ID > 255){
      MESSAGE_ID = 0 ;
    }
    messageToSend[1] = NODE_ID;
    messageToSend[2] = RECIEVER_ID;
    messageToSend[3] = RETRANSLATE;
    ////////////////////////////////////////////
    messageToSend[4] = PAYLOAD;
    ////////////////////////////////////////////
    for (int i=0;i<3;i++){
      messageToSend[5+i] = END_MESSAGE[i]; 
    } 
    // Отправляем
    for (int i =0;i<8;i++){
      mySerial.write(messageToSend[i]);
    } 
    sendNewMessage = false;
  }  
  else{
    messageToSend[3] = !RETRANSLATE; // retranslate = true
    for (int i =0;i<8;i++){
      mySerial.write(messageToSend[i]);
    } 
    sendNewMessage = true;
  }
}
void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
}


unsigned char alo[8];
int count=0;

void recieveMessage(){
  while(mySerial.available()) {
  
      unsigned char c = mySerial.read(); // читаем байт
      //Serial.print(c);
      alo[count] = c;
      if (count == 7){
        /*for (int i=0;i<8;i++){
            Serial.println(alo[i],HEX);
        }*/
        // Сюда надо придумать и написать как отличить сообщение от шлюза что типо пришло
        if (alo[2] == NODE_ID){
          // Проверяем мне ли это сообщение адресовано. Адресованное сообщение приходит только от шлюза.
          // Сюда написать сообщение от шлюза.
          sendNewMessage = true;
          Serial.println("Пришло от шлюза");
        };
        count = 0;
      }
      
      else{
          count++;  
      }
      
    }  
  
}


void loop()
{
  if ((millis()-sendtime)>=10000){
    Serial.println(sendNewMessage);
    printMessage();
    sendtime = millis();
    //readMessagee();
  }
  // Прием сообщение возможно стоит переделать
  recieveMessage();
  



}

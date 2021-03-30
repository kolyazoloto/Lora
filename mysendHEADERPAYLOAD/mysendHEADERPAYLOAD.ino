#include <Vector.h>
#include <SoftwareSerial.h>


unsigned long sendtime=0;

byte  NODE_ID = 15;
byte RECIEVER_ID = 10;
byte END_MESSAGE[3] ={0xff,0xfe,0xff};

byte MESSAGE_ID = 0;

bool RETRANSLATE = false;
char PAYLOAD = 22;

SoftwareSerial mySerial(8, 9);
// Место под переменные

// вектор на принятые сообщения
const int ELEMENT_COUNT_MAX = 10;
unsigned char storage_array[ELEMENT_COUNT_MAX];
Vector<unsigned char> recievedMessage(storage_array);

// Вектор для зранения сообщений
const int ELEMENT_COUNT_MAXM = 100;
unsigned char enternal_memory_array[ELEMENT_COUNT_MAXM];
Vector<unsigned char> enternal_memory_message(enternal_memory_array);


// Вектор для хранения приходящих сообщений на долгое время. Нужен для сортировки сообщений , перетусовки
const int ELEMENT_COUNT_MAXC = 100;
unsigned char enternal_memory_array_incame[ELEMENT_COUNT_MAXC];
Vector<unsigned char> enternal_memory_message_incame(enternal_memory_array_incame);



// Вектор для хранения приходящих сообщений на долгое время. Нужен для сортировки сообщений , перетусовки
const int ELEMENT_COUNT_MAXU = 100;
unsigned char enternal_memory_array_id[ELEMENT_COUNT_MAXU];
Vector<unsigned char> enternal_memory_message_id(enternal_memory_array_id);


bool sendNewMessage = true;
unsigned char messageToSend[8];

//функцпия проверки сообщение на повторения. Если сообщение новое то 1
bool isMesOrig(unsigned char one,unsigned char two){

  for (int i=0;i<enternal_memory_message_id.size();i+=2){
    if (one == enternal_memory_message_id[i] && two == enternal_memory_message_id[i+1]) return false;
  }
  return true;
}

void sendMessage(){
  // добавляем пришедшее сообщение к нашему
  for (auto i:enternal_memory_message_incame){
    enternal_memory_message.push_back(i);
  }
  enternal_memory_message_incame.clear();

  
  for (auto i:enternal_memory_message){
    mySerial.write(i);
    Serial.print(i,HEX);
    Serial.print(" ");
  }
  enternal_memory_message.clear();
  Serial.println("");
}

void makeMessage(){
  // Формируем массив сообщение
  //////////////////////HEADER
  // Если отправляем новое сообщение
  if (sendNewMessage){
    messageToSend[0] = MESSAGE_ID++;
    // Сделать лучше.Сделать двойной
    if (MESSAGE_ID > 254){
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
    
    for (int i =0;i<8;i++){
      enternal_memory_message.push_back(messageToSend[i]);
    } 
    // Отправляем
    sendMessage();
    //
    sendNewMessage = false;
  }  
  else{
    messageToSend[3] = !RETRANSLATE; // retranslate = true
    for (int i =0;i<8;i++){
      enternal_memory_message.push_back(messageToSend[i]);
    } 
    // Отправляем
    sendMessage();
    //
    sendNewMessage = true;
  }
}

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
}


void recieveMessage(){

  //Serial.println("");
  while(mySerial.available()) {
      unsigned char c = mySerial.read(); // читаем байт
      recievedMessage.push_back(c);
      // Определяем конец сообщения
      int lengthvec = recievedMessage.size();
      
       
      //Фиксируем приход сообщения
      if ((recievedMessage[lengthvec-3]==0xFF && recievedMessage[lengthvec-2]==0xFE && recievedMessage[lengthvec-1]==0xFF) == 1){
        /*for (auto elem:recievedMessage){
          Serial.print(elem,HEX);  
        }*/
        //Serial.println("");
        // Условия принятия сообщения
        // Если адресовано мне
        if (recievedMessage[2] == NODE_ID){
          // Проверяем мне ли это сообщение адресовано. Адресованное сообщение приходит только от шлюза.
          // Сюда написать сообщение от шлюза.
          sendNewMessage = true;
          //Serial.println("reply from gateway");
          /*for (auto elem:recievedMessage){
            Serial.print(elem,HEX);  
          }*/
          
        }
        // или если сообщение ретранслировано и не от меня, и не было ли оно ретранслировано ранее
        else if (recievedMessage[3] == 1 && recievedMessage[1] != NODE_ID && isMesOrig(recievedMessage[0],recievedMessage[1])){
          sendNewMessage = true;
          //Serial.println("Retrans message");
          // Запоминаем пришедшее сообщение
          // Кладем данные переретранслированных сообщений в память

          enternal_memory_message_id.push_back(recievedMessage[0]);
          enternal_memory_message_id.push_back(recievedMessage[1]);

          /*for (auto elem:enternal_memory_message_id){
            Serial.print(elem,HEX);
            Serial.print(" ");
          }
          Serial.println(" ");*/

          

          // где то надо брать из памяти
 
          for (auto elem:recievedMessage){
            enternal_memory_message_incame.push_back(elem);
          }
          //сохранить в память и отправить при следующей возможности
        }
        recievedMessage.clear();
      }

    
   }
}


void loop()
{
  if ((millis()-sendtime)>=10000){
    makeMessage();
    sendtime = millis();
  }
  // Прием сообщение возможно стоит переделать
  
  recieveMessage();




}

#include <SoftwareSerial.h>
//#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // Подключение библиотеки
LiquidCrystal_I2C lcd(0x27,16,2);

SoftwareSerial mySerial(8, 9);
void setup()
{

  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(2,OUTPUT);
  lcd.init();                      // Инициализация дисплея  
  lcd.backlight();                 // Подключение подсветки
  lcd.setCursor(0,0);  
}

bool state = false;
unsigned char content[8];
int count = 0;
void loop()
{
  while(mySerial.available()) {

    unsigned char c = mySerial.read(); // читаем байт
    //Serial.print(c);
    content[count] = c;
    if (count == 7){
      lcd.setCursor(0,0);
      lcd.clear();
      for (int i=0;i<8;i++){
          Serial.println(content[i],HEX);
          lcd.print(content[i],16);
      }
      count = 0;
        
        //
    }
    else{
      
        count++;  
    }
    
  }
  /*if (content.length()>= 5){
    //Serial.print(content);
    lcd.clear();



    lcd.setCursor(0,0); 
    //Serial.print(content);
    //Serial.print(content.length());
    
    lcd.print(content[0]); 
    //Serial.print(content[0]); 
    lcd.setCursor(0,1); 
    //lcd.print(content[1],16);
    //Serial.print(content[1],HEX); 
    //lcd.print(content[2],16); 
    //Serial.print(content[2],HEX);
   //lcd.print(content[3],16); 
    //Serial.print(content[3],HEX);
    lcd.print(content[4]); 
     //Serial.print(content[4]);
    //Serial.println("");
    
    content = "";
    digitalWrite(2,state);
    state = !state;
  }*/
  
  

}

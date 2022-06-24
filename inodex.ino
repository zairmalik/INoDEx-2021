#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Blynk.h>
#include <BlynkSimpleStream.h>
#define TempPin A0

char auth[] = "zG4XSyKVsTcOtRiMGBUUFoSL46Ri86JS"; // Put your Blynk Auth token
char* password = "1234";

Servo myservo;

int pos = 0;
int button = 0;
int pozisyen = 0;
int counter = 0;
int TempValue;
int ir_input = HIGH;
int ir = A1;
int LED = 49;
int limit = 0;

const byte rows = 4;
const byte cols = 4;

char keyMap[rows][cols] =
{
  {'7', '8', '9', 'A'},
  {'4', '5', '6', 'B'},
  {'1', '2', '3', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[rows] = {14, 15, 16, 17}; //connect to the row pinouts of the keypad
byte colPins[cols] = {18, 19, 20, 21};  //connect to the column pinouts of the keypad

Keypad myKeypad = Keypad(makeKeymap(keyMap), rowPins, colPins, rows, cols);
LiquidCrystal lcd(36, 38, 45, 46, 47, 48);

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("SMART LETTERBOX");
  lcd.setCursor(0,1);
  lcd.print("Letter=");
  
  delay(100);
  
  Blynk.begin(auth, Serial);
  Blynk.email("Capstone's Project", "System is start ...");
  myservo.attach(9);
  pinMode(49, OUTPUT);
  pinMode(ir, INPUT);
}

void loop() {
  Blynk.run();
  char whichKey = myKeypad.getKey();

  LETTER();
   
  if(whichKey == '#')
  {
    PASS();
  }
    
  temp();
}

void LETTER(){
  int ir_input = digitalRead(ir);
  delay(200);
  
  if(ir_input){
    lcd.setCursor(0,1);
    counter = counter + 1;
    Blynk.virtualWrite(V1,counter);
    lcd.setCursor(7,1);
    lcd.print(counter);

    if(counter >= 3){
      Blynk.notify("letter box is full!!!"); 
      Blynk.email("uzairnemo98@gmail.com", "letter box is full!!!");
    }

 }
 else{
  lcd.setCursor(0,1);
 }
}

void PASS(){
  char whichKey = myKeypad.getKey();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  lcd.setCursor(0,1);
  lcd.print(whichKey);
  lcd.display();
  delay(100);

  while(poziyen < 5){
    if(whichKey == '*' || whichKey == '#' || whichKey == 'A' || whichKey == 'C' || whichKey == 'D'|| whichKey == 'B'){
    pozisyen = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Invalid Key");
    delay (1000);
    lcd.clear();
    }

    if(whichKey == password[pozisyen]){
      pozisyen++;
    }

    if(poziyen == 4){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Verified!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print( "Box Open");
      servo();                              
      delay(500);
    }

    if(button){
      counter = 0;
      lcd.setCursor(0,0);
      lcd.print("SMART LETTERBOX");
      lcd.setCursor(0,1);
      lcd.print("Letter=");
      delay(100);
    }
  }
}

void temp(){
  TempValue = analogRead(TempPin);
  float TempCel = (TempValue/1024.0)*500;
  Blynk.virtualWrite(V0, TempCel);
  Serial.print ("TEMPERATURE = ");
  Serial.print (TempCel);
  Serial.print ("*c");
  Serial.println();
  
  if(TempCel >=50){
    digitalWrite(49, HIGH);
    Blynk.notify("water detected in letter box!!!"); 
    Blynk.email("uzairnemo98@gmail.com", "water detected in letter box!!!");
  }
  else{
    digitalWrite(49, LOW);
  }
  
  delay (500);
}

void servo(){
 for(pos = 0; pos <= 180; pos += 10){
  myservo.write(pos);
  delay(5);
 } 
}

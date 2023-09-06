#include <LiquidCrystal.h>

#define COLS 16
#define ROWS 2

LiquidCrystal lcd(10, 9, 8, 4, 3, 2);

int serialData = 7;
int shiftClock = 5;
int latchClock = 6;
int pinReset = 11;
int buzzer = 12;
int ledGreen = 13;
int ledYellow = A2;
int ledRed = A3;

int thresholds[16] = {559, 541, 522, 500, 481, 456, 429, 399, 371, 334, 293, 248, 204, 146, 78, 0};
char keypad[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

int datArray[4] = {B00000000, B11001100, B10101010, B11111111};

String inputString;
long inputInt;
long delayTime;
int startPoint = 8;
int celsius;
int initialTemp;

void setup(){
  digitalWrite(pinReset, HIGH);
  delay(100);
  Serial.begin(9600);
  lcd.begin(COLS, ROWS);
  
  pinMode(A1, INPUT);
  pinMode(pinReset, OUTPUT);
  pinMode(serialData, OUTPUT);
  pinMode(shiftClock, OUTPUT);
  pinMode(latchClock, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("* to select mode");
  lcd.setCursor(0,1);
  lcd.print("timer = ");
}

void loop(){
  tempSafety();
  int value = analogRead(A0);
  for(int i = 0; i < 16; i++){
    if(abs(value - thresholds[i]) < 2){
      char key = keypad[i];

    if (key){
    lcd.print(String(key) + String(" sec"));
    lcd.setCursor (++startPoint, 1);

    if (key >= '0' && key <= '9'){
      inputString += key;
      
    } else if (key == '*'){
        lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A cleaning mode");
      lcd.setCursor(0,1);
      lcd.print("B drying mode");
      if (inputString.length() > 0) {
        inputInt = inputString.toInt();
        delayTime = inputInt*1000;
        inputString = "";
      }
    } else if (key == '#'){
      digitalWrite(pinReset, LOW);
      delay(100);
    } else if (key == 'A'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("IN PROGRESS ...");
      Cleaning();}
      else if (key == 'B'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("IN PROGRESS ...");
      Drying();}
  }
      while(analogRead(A0) < 1000){
        delay(100);
      }
    }
  }
}

void tempSafety(){
  initialTemp = 40;
  celsius = map(((analogRead(A1) - 20) * 3.04), 0, 1023, -40, 125);
  if (celsius < initialTemp) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    delay(100);
  }
  if (celsius >= initialTemp && celsius < initialTemp + 20) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
    delay(100);
  }
  if (celsius >= initialTemp + 20 && celsius < initialTemp + 40) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    delay(100);
  }
  if (celsius >= initialTemp + 50) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(pinReset, LOW);
    delay(100);
  }
  delay(100);
}

void Cleaning(){ 
  digitalWrite(latchClock, LOW);
  shiftOut(serialData, shiftClock, LSBFIRST, datArray[2]);
  digitalWrite(latchClock, HIGH);
  delay(delayTime);
  
  digitalWrite(latchClock, LOW);
  shiftOut(serialData, shiftClock, LSBFIRST, datArray[1]);
  digitalWrite(latchClock, HIGH);
  delay(delayTime);
  
  digitalWrite(latchClock, LOW);
  shiftOut(serialData, shiftClock, LSBFIRST, datArray[2]);
  digitalWrite(latchClock, HIGH);
  delay(delayTime);
  
  digitalWrite(latchClock, LOW);
  shiftOut(serialData, shiftClock, LSBFIRST, datArray[0]);
  digitalWrite(latchClock, HIGH);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CLEANING DONE");
  lcd.setCursor(0,1);
  lcd.print("# to return");
  
  tone(buzzer, 220, 3000);
  delay(200);
}

void Drying(){
  digitalWrite(latchClock, LOW);
  shiftOut(serialData, shiftClock, LSBFIRST, datArray[2]);
  digitalWrite(latchClock, HIGH);
  delay(delayTime);

  digitalWrite(latchClock, LOW);
  shiftOut(serialData, shiftClock, LSBFIRST, datArray[3]);
  digitalWrite(latchClock, HIGH);
  delay(delayTime);
  
  digitalWrite(latchClock, LOW);
  shiftOut(serialData, shiftClock, LSBFIRST, datArray[1]);
  digitalWrite(latchClock, HIGH);
  delay(delayTime);
  
  digitalWrite(latchClock, LOW);
  shiftOut(serialData, shiftClock, LSBFIRST, datArray[0]);
  digitalWrite(latchClock, HIGH);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DRYING DONE");
  lcd.setCursor(0,1);
  lcd.print("# to return");
  
  tone(buzzer, 220, 3000);
  delay(200);
}

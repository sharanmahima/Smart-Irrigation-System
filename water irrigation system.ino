#include <LiquidCrystal.h>
#define maxdry 460
#define minrainthreshold 300

//note that driving the motor without an external power source will generate noises
//that will disrupt the workings of the LCD and driving the generator directly
//from the arduino risks damaging the pins. So we must implement a
//driver circuit for the generator to work and we must run the generator with an external
//power source. We can use mosfet for the driver circuit operation or we can create
//our own driver circuit following the steps from here : https://www.tutorialspoint.com/arduino/arduino_dc_motor.htm

const int rs = 7; const int en = 6; const int d4 =5; const int d5 = 4;

//int A0; int A1;

const int d6 = 3; const int d7 = 2; const int LED1=10; const int LED2=9 ; const int rainPin=A0; 
//const int soilPin = A1; 
int soilPin;
const int pumpPin = 11; const int definedVal=5;
int hourCount=definedVal;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);   

void setup() {
    lcd.begin(16,2);
    pinMode(LED1, OUTPUT);
    pinMode(pumpPin, OUTPUT);
    Serial.begin(9600);    
}

int getRainData(){
  int data = analogRead(rainPin);
  return data;
}


int getSoilData(){
  int data = analogRead(soilPin);
  return data;
}

void doCheckRain(int rainValue){
if(rainValue<=minrainthreshold){
    lcd.clear();
   digitalWrite(LED2,HIGH);
   lcd.print("Rained or is ");
   lcd.setCursor(0,2);
   lcd.print("Raining.");
   hourCount=definedVal;
   delay(2000);
}else{
   digitalWrite(LED2,LOW);
}
}


void loop() {
  digitalWrite(LED2,LOW);
  lcd.clear();

  int rainValue =  getRainData();
  Serial.println(rainValue);
  lcd.print(hourCount);
  if(hourCount>1){
  lcd.print(" hours till");
  }else{
  lcd.print(" hour till");
  }
  Serial.println(hourCount);  
  doCheckRain(rainValue);
  lcd.setCursor(0,2);
  lcd.print("RC:");
  lcd.print(rainValue);

  if(hourCount<=0){
  int soilValue =  getSoilData();

  if(soilValue<=maxdry){
    lcd.clear();
   lcd.print("Soil is wet.");
   delay(2000);
   digitalWrite(LED1,LOW);
  }else{
//  while(soilValue>maxdry)?
   lcd.clear();
   lcd.print("Running pump...");
   lcd.setCursor(0,2);
   lcd.print("SC:");
   lcd.print(soilValue);
   digitalWrite(LED1,HIGH);
   digitalWrite(pumpPin,HIGH);
   delay(5000);
   digitalWrite(pumpPin,LOW);
   digitalWrite(LED1,LOW);
    lcd.clear();
   lcd.print("pump stopped...");
  }
  }

  if(hourCount>0){
  hourCount--;
  }else{
    hourCount=definedVal;
  }
  delay(1000);
}
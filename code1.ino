#include <Adafruit_CircuitPlayground.h>
#include <RTClib.h> //Library for RTC
#include <Wire.h> //Allows comunication with TWI (Two Wire Interface)
#include <LiquidCrystal.h> //Used for DS3231 and thermister
#include<Servo.h>
Servo myservo;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Pins used by the LCD
DS3231 rtc(SDA, SCL);
Time t;
#define buz 8
int tempPin = 0;
int Hor;
int Min;
int Sec;
int tempC;
int tempF;
int tempCDS3231;
int tempFDS3231;
int TabletNo=2;
int pos = 0; // variable to store the servo position
void setup()
{
 myservo.attach(9); // attaches the servo on pin 9 to the servo object
Serial.begin(9600);
while(!Serial){}
rtc.begin();
//used to fix time
 rtc.setDOW(WEDNESDAY); // Set Day-of-Week to SUNDAY
 rtc.setTime(15, 43, 00); // Set the time to 12:00:00 (24hr format)
 rtc.setDate(04, 03, 2021); // Set the date to August 8th, 2018
 Wire.begin();
 rtc.begin();
 Serial.begin(9600);
pinMode(buz, OUTPUT);
 lcd.begin(16,2);
 lcd.setCursor(0,0);
t = rtc.getTime();
 Hor = t.hour;
 Min = t.min;
 Sec = t.sec;
}
void loop()
{
 int tempReading = analogRead(tempPin);
 // This is OK
 double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
 tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );
// Temp Kelvin
 int tempC = tempK - 273.15; // Convert Kelvin to Celcius
//-------------------------------------------------------------------------------------------
//Can change the ABOVE tempC and tempF "int" to "float" this can give you temperatures
with two decimal points.
//Can also use "lcd.print(tempF, 1);" BELOW to print with one decimal place or
"lcd.print(tempF, 0);" // print without decimal place
//-------------------------------------------------------------------------------------------

lcd.setCursor(0,0); //Top line of the LCD
lcd.print(" ");
lcd.print(rtc.getTimeStr());
lcd.print(" ");
lcd.setCursor(0,1);
lcd.print(" ");
lcd.print(rtc.getDateStr());
//----------------------------------------------------------
 lcd.setCursor(0,1); //Second line of the LCD
tempFDS3231 = (tempCDS3231 * 1.8) + 32.0; // Convert C to F
tempCDS3231 = (rtc.getTemp());
lcd.setCursor(0,1);
lcd.print(tempC);
lcd.print((char)223); //This creates a Degree symbol
lcd.print("C ");
lcd.print(rtc.getDateStr());
//--------------------------------------------------------------------------
 t = rtc.getTime();
 Hor = t.hour;
 Min = t.min;
 Sec = t.sec;
//ALARM CONDITION
if( Hor == 15 && (Min == 44 || Min ==00 )){ //Comparing the current time with the Alarm
time
lcd.setCursor(0,0); //Top line of the LCD
lcd.print(" ");
lcd.print(rtc.getTimeStr());
lcd.print(" ");
lcd.setCursor(0,1);
lcd.print(" ");
lcd.print(rtc.getDateStr());
//for tempreture
lcd.setCursor(0,1); //Second line of the LCD
tempFDS3231 = (tempCDS3231 * 1.8) + 32.0; // Convert C to F
tempCDS3231 = (rtc.getTemp());
lcd.setCursor(0,1);
lcd.print(tempC);
lcd.print((char)223); //This creates a Degree symbol
lcd.print("F ");
lcd.print(rtc.getDateStr());
if(TabletNo==1){
 for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
 // in steps of 1 degree
 myservo.write(pos); // tell servo to go to position in variable 'pos'
 delay(15); // waits 15ms for the servo to reach the position
 }
}
else if(TabletNo==2){
 for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
 myservo.write(pos); // tell servo to go to position in variable 'pos'
 delay(15); // waits 15ms for the servo to reach the position
 }
}
//delay(60000);
tone(buz,1000);
delay(10000);
noTone(buz);
delay(1000);
if(TabletNo==1){
 for (pos = 90; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
 // in steps of 1 degree
 myservo.write(pos); // tell servo to go to position in variable 'pos'
 delay(15); // waits 15ms for the servo to reach the position
 }
}
else if(TabletNo==2){
 for (pos = 180; pos >= 90; pos -= 1) { // goes from 0 degrees to 180 degrees
 myservo.write(pos); // tell servo to go to position in variable 'pos'
 delay(15); // waits 15ms for the servo to reach the position
 }
}
delay(500000000000);
}
}

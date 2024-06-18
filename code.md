#include <Adafruit_CircuitPlayground.h>
#include <RTClib.h> //Library for RTC
#include <Wire.h> //Allows communication with TWI (Two Wire Interface)
#include <LiquidCrystal.h> //Used for DS3231 and thermistor
#include <Servo.h>

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
int TabletNo = 2;
int pos = 0; // variable to store the servo position

void setup() {
    myservo.attach(9); // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);
    while (!Serial) {}

    rtc.begin();
    // used to fix time if necessary
    // rtc.setDOW(WEDNESDAY); // Set Day-of-Week to WEDNESDAY
    // rtc.setTime(15, 43, 00); // Set the time to 15:43:00 (24hr format)
    // rtc.setDate(04, 03, 2021); // Set the date to March 4th, 2021

    Wire.begin();
    pinMode(buz, OUTPUT);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    t = rtc.getTime();
    Hor = t.hour;
    Min = t.min;
    Sec = t.sec;
}

void loop() {
    int tempReading = analogRead(tempPin);
    // Calculate temperature from the analog reading
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK)) * tempK); // Temp Kelvin
    tempC = tempK - 273.15; // Convert Kelvin to Celsius

    // Update LCD with current time and date
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.print(rtc.getTimeStr());
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.print(rtc.getDateStr());

    // Get temperature from DS3231
    tempCDS3231 = rtc.getTemp();
    tempFDS3231 = (tempCDS3231 * 1.8) + 32.0; // Convert C to F

    // Display temperature on LCD
    lcd.setCursor(0, 1); // Second line of the LCD
    lcd.print(tempC);
    lcd.print((char)223); // Degree symbol
    lcd.print("C ");
    lcd.print(rtc.getDateStr());

    // Get current time
    t = rtc.getTime();
    Hor = t.hour;
    Min = t.min;
    Sec = t.sec;

    // Alarm condition
    if (Hor == 15 && (Min == 44 || Min == 00)) { // Comparing the current time with the Alarm time
        lcd.setCursor(0, 0); // Top line of the LCD
        lcd.print(" ");
        lcd.print(rtc.getTimeStr());
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(rtc.getDateStr());

        // Display temperature on LCD
        lcd.setCursor(0, 1); // Second line of the LCD
        lcd.print(tempC);
        lcd.print((char)223); // Degree symbol
        lcd.print("F ");
        lcd.print(rtc.getDateStr());

        // Move servo for TabletNo
        if (TabletNo == 1) {
            for (pos = 0; pos <= 90; pos += 1) { // Move from 0 to 90 degrees
                myservo.write(pos);
                delay(15);
            }
        } else if (TabletNo == 2) {
            for (pos = 0; pos <= 180; pos += 1) { // Move from 0 to 180 degrees
                myservo.write(pos);
                delay(15);
            }
        }

        tone(buz, 1000);
        delay(10000);
        noTone(buz);
        delay(1000);

        if (TabletNo == 1) {
            for (pos = 90; pos >= 0; pos -= 1) { // Move back from 90 to 0 degrees
                myservo.write(pos);
                delay(15);
            }
        } else if (TabletNo == 2) {
            for (pos = 180; pos >= 90; pos -= 1) { // Move back from 180 to 90 degrees
                myservo.write(pos);
                delay(15);
            }
        }
        delay(60000); // Delay for 1 minute before next check
    }
}

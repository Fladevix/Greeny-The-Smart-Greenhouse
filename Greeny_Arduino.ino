/*Created by Khoirul Iman December 2015
  Greeny Smart Greenhouse Using Intel Galileo Gen 2 & Genuino Uno */
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define I2C_ADDR    0x27 
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
int n = 1;

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

const int Soil = A0;
byte temp[8] = 
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};
byte hum[8] = 
{
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};

void setup()
{
  Serial.begin(9600);
  dht.begin();
  lcd.begin (20,4); 
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();
  lcd.setCursor(2,0); 
  lcd.print("Welcome To Greeny");
  lcd.setCursor(0,1);
  lcd.createChar(1,temp);
  lcd.createChar(2,hum);  
  pinMode(Soil, INPUT);
 delay(2000); 
}


void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float m = analogRead(Soil);
  m = m / 10 ;

  Serial.println(h);
  Serial.println(t);
  Serial.println(m);
  
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Greeny Greenhouse");
  lcd.setCursor(3, 1);
  lcd.write(1);
  lcd.setCursor(8,1);
  lcd.print(":");
  lcd.setCursor(10, 1);
  lcd.print(t,0);
  lcd.setCursor(12, 1);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(3, 2);
  lcd.write(2);
  lcd.setCursor(8,2);
  lcd.print(":");
  lcd.setCursor(10,2);
  lcd.print(h,0);
  lcd.print("%");
  lcd.setCursor(3,3);
  lcd.print("Soil : ");
  lcd.setCursor(10,3);
  lcd.print(m);
  lcd.print("%");
  delay(10000); 
}


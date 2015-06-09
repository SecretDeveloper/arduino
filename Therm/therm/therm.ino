/*
** Example Arduino sketch for SainSmart I2C LCD2004 adapter for HD44780 LCD screens
** Readily found on eBay or http://www.sainsmart.com/ 
** The LCD2004 module appears to be identical to one marketed by YwRobot
**
** Address pins 0,1 & 2 are all permenantly tied high so the address is fixed at 0x27
**
** Written for and tested with Arduino 1.0
** This example uses F Malpartida's NewLiquidCrystal library. Obtain from:
** https://bitbucket.org/fmalpartida/new-liquidcrystal 
**
** Edward Comer
** LICENSE: GNU General Public License, version 3 (GPL-3.0)
**
** NOTE: TEsted on Arduino NANO whose I2C pins are A4==SDA, A5==SCL
*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>

#define I2C_ADDR    0x27  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define DHT22_PIN 2

int n = 1;
float maxT = 0.0;
float minT = 0.0;
float minH = 0.0;
float maxH = 0.0;

dht DHT;
LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

byte thermo[8] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte humid[8] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110
};

byte degree[8] = { 
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};


void setup()
{
  lcd.begin (16,2);
  
// Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.createChar(0, thermo);
  lcd.createChar(1, humid);
  lcd.createChar(2, degree);
}

void loop()
{
  DHT.read22(DHT22_PIN);
//  lcd.print(n++,DEC);
  updateMinMax();
  
  lcd.setCursor (0,0);
  lcd.write(byte(1));
  lcd.print(DHT.humidity,1);

  lcd.setCursor (0,1);
  lcd.print(minH,0);
  lcd.print("-");
  lcd.print(maxH,0);
  
  lcd.setCursor (11,0);
  lcd.write(byte(0));
  lcd.print(DHT.temperature,1);

  lcd.setCursor (11,1);
  lcd.print(minT,0);
  lcd.print("-");
  lcd.print(maxT,0);
  


  delay(3000);
}

void updateMinMax(){
 if(minT==0.0) minT = DHT.temperature;
 if(maxT==0.0) maxT = DHT.temperature;  
 if(minT>DHT.temperature) minT = DHT.temperature;
 if(maxT<DHT.temperature) maxT = DHT.temperature;
 
 if(minH==0.0) minH = DHT.humidity;
 if(maxH==0.0) maxH = DHT.humidity;  
 if(minH>DHT.temperature) minH = DHT.humidity;
 if(maxH<DHT.temperature) maxH = DHT.humidity; 
}

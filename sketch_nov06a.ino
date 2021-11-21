
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
#include <TinyGPS++.h> 

const String PHONE = "+8801973421167";
//GSM Module RX pin to Arduino 3
//GSM Module TX pin to Arduino 2

#define rxPin 2
#define txPin 3
const int buttonPin = 10;
SoftwareSerial sim800(rxPin,txPin);

//GPS Module RX pin to Arduino 9
//GPS Module TX pin to Arduino 8
AltSoftSerial neogps;
int buttonState;
TinyGPSPlus gps;

void setup() {

  Serial.begin(115200);
  sim800.begin(9600);
  neogps.begin(9600);

  pinMode(buttonPin, INPUT);
  
  sim800.print("AT+CMGF=1"); //SMS text mode
  delay(1000);

}

void loop() {

  buttonState = digitalRead(buttonPin);
if(buttonState == HIGH){
 Serial.print("button pressed");


    makeCall();
  sendSmsGPS("Location");
  
  
  }
}
void sendSmsGPS(String text)
{
  // Can take up to 60 seconds
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 2000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;
      }
    }
  }
  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    Serial.print("Latitude= "); 
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Longitude= "); 
    Serial.println(gps.location.lng(), 6);
    newData = false;
    delay(300);
    ///*
    sim800.print("AT+CMGF=1\r");
    delay(1000);
    sim800.print("AT+CMGS=\""+PHONE+"\"\r");
    delay(1000);
    sim800.print("I am in Danger, I Need Help. ");
    sim800.print("http://maps.google.com/maps?q=loc:");
    sim800.print(gps.location.lat(), 6);
    sim800.print(",");
    sim800.print(gps.location.lng(), 6);
    delay(100);
    sim800.write(0x1A); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
    delay(1000);

  }
}

void makeCall() {
  Serial.begin(9600); //Baud rate of the GSM/GPRS Module
  sim800.println("ATD+8801973421167;"); //Number to which you want to make call
  //sim800.print("ATD"+PHONE+";");
  delay(15000);
  sim800.println("ATH");
  delay(1000);
  sendSmsGPS("Location");
  delay(1000);
}

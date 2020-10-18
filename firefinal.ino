//Arduino Fire Detector
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 
SoftwareSerial mySerial(9, 10);
#define led 7
int sensor=A1;
float temp_read;
float Temp_alert_val;
float Temp_shut_val;
int sms_count=0,Fire_Set;

void setup()
{
  pinMode(sensor,INPUT);
  mySerial.begin(9600);   
  Serial.begin(9600);    
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print("Arduino Fire"); 
  lcd.setCursor(0,1);  
  lcd.print("Detector"); 
  delay(500);
}

void loop()
{
CheckFire();// Constantly checks for LM35 temperature 
CheckShutDown();//
}

void CheckFire()
{
lcd.setCursor(0,0);
lcd.print("Fire Scan - ON");
Temp_alert_val=CheckTemp();
if(Temp_alert_val>40)
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, i t will back to OK
  
  
  mySerial.println("ATD+ +254790876773;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
  
  delay(20000); // wait for 20 seconds...
  mySerial.println("ATH"); //hang up
  
 
 digitalWrite(led, HIGH);
}
}


float CheckTemp()
{
temp_read=analogRead(sensor); // reads LM35 Vout
temp_read=temp_read*5;    // converts the sensor reading to temperature
temp_read=temp_read/10;  // adds the decimal point
return temp_read; // returns temperature value in degree celsius
}


void CheckShutDown()//Checks if temperature is <30
{
if(Fire_Set==1)
{
Temp_shut_val=CheckTemp();
if(Temp_shut_val<40)// sets fire shut down at 28 degrees C
{
digitalWrite(led, LOW);
lcd.setCursor(0,1);
lcd.print("Fire OFF! SAFE NOW");
sms_count=0;//reset
Fire_Set=0;//reset
}}}

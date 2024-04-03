#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;
#define ir_sen 8
#define buz1 9
#define buz2 12
#include <LiquidCrystal.h>
//At Work Place AmBTech
//ZBT TG621
//LAT:17.368839  
//LON:78.530784
//NEO Tiny
//LAT:17.368835  
//LON:78.530761
int flag1=1;
//LiquidCrystal lcd (8,9,10,11,12,13);
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
#define m1 5
#define m2 4 
#define m3 6
#define m4 7

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
void buz_beep(int t){
  for(int bt=0;bt<t;bt++){
  digitalWrite(buz1,1);digitalWrite(buz2,0);delay(500);
  digitalWrite(buz1,0);digitalWrite(buz2,0);delay(100);}
}
void setup()
{
  pinMode(ir_sen,INPUT_PULLUP);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);  
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  
  pinMode(buz1, OUTPUT);
  pinMode(buz2, OUTPUT);
  digitalWrite(buz1,0);
  digitalWrite(buz2,0);
  digitalWrite(m1,0);
  digitalWrite(m2,0);
  digitalWrite(m3,0);
  digitalWrite(m4,0);
  buz_beep(1);
    Serial.begin(9600);
  ss.begin(GPSBaud); 
  //Serial.println(TinyGPSPlus::libraryVersion());
SendMessage("SYSTEM READY.");
  buz_beep(2);

delay(1000);
 motor_run();
//get_gps();  
}

void SendMessage(String msg1)

{
  //Serial.println("Sending sms");
  String cmd1=msg1;
  Serial.println("AT+CMGF=1");
  delay(2000);
  Serial.println("AT+CMGS=\"9701928282\"");//919959409194
  delay(2000);
  Serial.println(cmd1);
  delay(2000);
  Serial.write(0X1A);
  delay(2000);
  //Serial.println("sent sms");
  // put your main code here, to run repeatedly:

  // put your main code here, to run repeatedly:

}

void motor_run()
{
  digitalWrite(m1,1);
  digitalWrite(m2,0);
  digitalWrite(m3,1);
  digitalWrite(m4,0);
}

void motor_stop(){
  digitalWrite(m1,0);
  digitalWrite(m2,0);
  digitalWrite(m3,0);
  digitalWrite(m4,0);
}



void loop()
{

  if(digitalRead(ir_sen)==1){
      
      motor_stop();buz_beep(3);  
      get_gps();delay(5000);get_gps();
      flag1=0;
      }
                    
  else{
    if(flag1==1){motor_run();}
  }
}

void get_gps()
{
//Serial.print("LAT:");
//lcd.begin(16, 2);
lcd.setCursor(0,0);
lcd.print("LAT:");
//lcd.setCursor(7,0);
//lcd.print("LT:");
printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
lcd.setCursor(5,0);
//lcd.setCursor(10,0);
lcd.print(gps.location.lat());
lcd.print(gps.location.isValid());
//Serial.print("\r\n");
delay(1000);

/////////////////////////////////////////////////////////////////////////////////
//Serial.print("LON:");
lcd.setCursor(0,1);
lcd.print("LON:");
//lcd.setCursor(7,1);
//lcd.print("LN:");
printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
lcd.setCursor(5,1);
//lcd.setCursor(10,1);
lcd.print(gps.location.lng());
lcd.print(gps.location.isValid());

/////////////////////////////////////////////
String cmd2="RAILWAY_CRACK_DETECTED_AT_\"r\n""https://www.google.com/maps/?q=" + String(gps.location.lat()) + String(gps.location.isValid())+","
            + String(gps.location.lng())+ String(gps.location.isValid());
//Serial.println("Sending sms");
  Serial.println("AT+CMGS=\"9701928282\"");
  delay(2000);
  Serial.println(cmd2);
  delay(2000);
  Serial.write(0X1A);
  delay(5000);
 // Serial.println("sent sms");
  
/////////////////////////////////////////////

/////////////////////////////////////////////
/*
//Serial.println("Sending sms");
  Serial.println("AT+CMGS=\"9985787078\"");
  delay(2000);
  Serial.println(cmd2);
  delay(2000);
  Serial.write(0X1A);
  delay(5000);
  //Serial.println("sent sms");
  */
/////////////////////////////////////////////


  //Serial.print("\r\n");
  delay(1000);
  unsigned long distanceKmToLondon =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT, 
      LONDON_LON) / 1000;
  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT, 
      LONDON_LON);
  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);
  //Serial.println();
  
  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    //Serial.println(" ");
  }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
    {
    }
    //Serial.print(' ');
  }
  else
  {
    //Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i);
      //Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
  }
  else
  {
  }
 
  if (!t.isValid())
  {
  }
  else
  {
  }
  smartDelay(0);
} 

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
  smartDelay(0);
}

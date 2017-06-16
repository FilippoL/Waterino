#include <Time.h>
#include <TimeLib.h>

#include <LiquidCrystal.h>
#include <DS3231.h>
#include <dht.h>




// Initialize LCD with following connections
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DS3231  rtc(SDA, SCL);
dht DHT;
Time t;

#define DHT22_PIN 13

const int PIN_BUTTON1 = 7;  // digital
const int PIN_BUTTON2 = 8;  // digital
const int PUMP_1 = 6;  // digital
const int PUMP_2 = 10;
const int read = A0; //Sensor AO pin to Arduino pin A0
int value;         //Variable to store the incomming data

void ReadWaterLvl();
void SetProgram();
void SetDate();


int Day , Month ;
int Year = 2000;
int h, m, s;
int DayOfTheWeek;

int Time_Water_h ;
int Interval ;
int Water_howlong;

int Current_m;

bool set;
bool Pump_ON;
bool Prog_Run;


void setup() {


 Serial.begin(9600);
  rtc.begin();

  
  pinMode(PUMP_1, OUTPUT);
  pinMode(PUMP_2, OUTPUT);
  
  lcd.begin(16, 2);


}

void InitialiseClock()
{
 // rtc.setDOW(DayOfTheWeek);    
  rtc.setTime(h, m, s);     
  rtc.setDate(Day, Month, Year);   //
}

void loop() {
lcd.clear();
lcd.noBlink();

  if(HIGH == digitalRead(PIN_BUTTON1) && HIGH == digitalRead(PIN_BUTTON2))
    {
      delay(500);
      SetDate();

    }

  if (!Time_Water_h == NULL || !Interval == NULL || !Water_howlong == NULL )
  {
    if(Time_Water_h == hour()|| Time_Water_h + Interval == hour())
    {
      if (!(minute() >  Water_howlong))
      { digitalWrite(PUMP_1, HIGH);
        digitalWrite(PUMP_2, HIGH);
        Prog_Run = true;
      }
      else{
        digitalWrite(PUMP_1, LOW);
        digitalWrite(PUMP_2, LOW);
          Prog_Run = false;
        }
      
    }

  }

  
  if(HIGH == digitalRead(PIN_BUTTON1))
    {
      int chk = DHT.read22(DHT22_PIN);
      switch (chk)
      {
        case DHTLIB_OK:  
        Serial.println("OK,\t"); 
        break;
        case DHTLIB_ERROR_CHECKSUM: 
        Serial.println("Checksum error,\t"); 
        break;
        case DHTLIB_ERROR_TIMEOUT: 
        Serial.println("Time out error,\t"); 
        break;
        default: 
        Serial.println("Unknown error,\t"); 
        break;
      }
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Celsius: ");
      lcd.setCursor(11, 0);
      lcd.print(DHT.temperature);
    
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.setCursor(11, 1);
      lcd.print(DHT.humidity);

      delay(250);

    }

  else{  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr());

  lcd.setCursor(0, 1);
  lcd.print(rtc.getDateStr());

   if(Pump_ON == true || Prog_Run == true)
  {
     lcd.setCursor(13, 0);
     lcd.print("ON");
  }
  else if (Pump_ON == false && Prog_Run == false) {
     lcd.setCursor(13, 0);
     lcd.print("OFF");
    }
   
  }
   
/*
  ReadWaterLvl();
 
  
  if ( value>415 && Pump_ON == false && Prog_Run == false)
  {
     lcd.clear();
     while(value>415)
     {
     lcd.setCursor(0, 0);
     lcd.print("**TOO MUCH WATER!!**");
     lcd.scrollDisplayLeft();
   //  digitalWrite(ALERT_LED, HIGH);
     ReadWaterLvl();
    }
    
   }
   else{
    
   // digitalWrite(ALERT_LED, LOW);
    }
   
  if (value<=350 && Pump_ON == false && Prog_Run == false )
  {
    lcd.clear();
     while(value<=350)
     {
     
     lcd.setCursor(0, 0);
     lcd.print("**TOO LITTLE WATER!!**");
     lcd.scrollDisplayLeft();
   //  digitalWrite(ALERT_LED, HIGH);
     ReadWaterLvl();
   }
    
   }
   else{
    
  //  digitalWrite(ALERT_LED, LOW);
    }
*/

  if(HIGH == digitalRead(PIN_BUTTON2) && Pump_ON == false && Prog_Run == false)
  {digitalWrite(PUMP_1, HIGH);
  digitalWrite(PUMP_2, HIGH);
   Pump_ON = true;
  }
  else if (HIGH == digitalRead(PIN_BUTTON2) && Pump_ON == true && Prog_Run == false)
  {digitalWrite(PUMP_1, LOW); 
  digitalWrite(PUMP_2, LOW); 
   Pump_ON = false;}
  
  delay (500); ///Dont under estimate the delay, LCD needs it to correctly print 
}

void SetDate()
{
   Month = Day = 1;
   Year = 2000;
   rtc.setDate(Day, Month, Year);
   
   
    while (set == false) ///set day
  {
     
      lcd.clear();
      lcd.print("ENTER DAY: ");
      lcd.setCursor(0,1);   
      lcd.print(rtc.getDateStr(FORMAT_SHORT));
      lcd.setCursor(1,1); 
      lcd.blink();
      
      if (HIGH == digitalRead(PIN_BUTTON1))
      {
         lcd.noBlink();
        Day += 1;       
      }
          
    if (HIGH == digitalRead(PIN_BUTTON2))
      {
        set = true;       
      }

    if (Day > 31)
    { 
      Day = 1;
    }
    
    rtc.setDate(Day, Month, Year);
    delay (250);
  }


/*************************************************/
  set = false;
  while (set == false) //set month
  {
          
      lcd.clear();
      lcd.print("ENTER MONTH: ");
      lcd.setCursor(0,1);
      lcd.print(rtc.getDateStr(FORMAT_SHORT));
      lcd.setCursor(4,1);
      lcd.blink();
      
      if (HIGH == digitalRead(PIN_BUTTON1))
      {
         lcd.noBlink();
        Month += 1;       
      }
      
    if (HIGH == digitalRead(PIN_BUTTON2))
      {
        set = true;       
      }
      
 if (Month > 12)
    { 
      Month = 1;
    }
    
   rtc.setDate(Day, Month, Year);
      delay (250);
  }

   
/********************************************************/
  set = false;
  while (set == false)  //set year
  {
          
      lcd.clear();
      lcd.print("ENTER YEAR: ");
      lcd.setCursor(0,1);
      lcd.print(rtc.getDateStr(FORMAT_SHORT));

      lcd.setCursor(7,1);
      lcd.blink();

      
      if (HIGH == digitalRead(PIN_BUTTON1))
      {
         lcd.noBlink();
        Year += 1;       
      }
      
    if (HIGH == digitalRead(PIN_BUTTON2))
      {
        set = true;       
      }
      rtc.setDate(Day, Month, Year);
      delay (250);
  }
  
  /******************************************/
  set = false;
  while (set == false) //set h
  {
          
      lcd.clear();
      lcd.print("ENTER TIME: h");
      lcd.setCursor(0,1);
      lcd.print(rtc.getTimeStr());

      lcd.setCursor(1,1);
      lcd.blink();

      
      if (HIGH == digitalRead(PIN_BUTTON1))
      {
         lcd.noBlink();
        h += 1;       
      }
      
    if (HIGH == digitalRead(PIN_BUTTON2))
      {
        
        set = true;       
      }

    if (h > 23)
    { 
      h = 0;
    }
    InitialiseClock();
      delay (250);
  }

/*************************************************/
  set = false;
  while (set == false) //set m
  {
          
      lcd.clear();
      lcd.print("ENTER TIME: m");
      lcd.setCursor(0,1);
      lcd.print(rtc.getTimeStr());
      
      lcd.setCursor(4,1);
      lcd.blink();
      
      if (HIGH == digitalRead(PIN_BUTTON1))
      {
         lcd.noBlink();
        m += 1;       
      }
      
    if (HIGH == digitalRead(PIN_BUTTON2))
      {
        set = true;       
      }
      
   if (m > 59)
    { 
      m = 0;
    }
    InitialiseClock();
    delay (250);
  }
  
setTime(h, m, 0, Day, Month, Year);
SetProgram();
set = false;
}

void SetProgram()
{
  /********************************************************** SET Hour*/
  set = false;
   while(set == false)
     {
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Water starts at:");
       lcd.setCursor(0, 1);
       lcd.print(Time_Water_h);
     
      if (HIGH == digitalRead(PIN_BUTTON1))
      {
         lcd.noBlink();
         Time_Water_h += 1;       
      }
       if (HIGH == digitalRead(PIN_BUTTON2))
      {
        set = true;       
      }
        
    if (Time_Water_h > 23)
    { 
     Time_Water_h = 0;
    }
     delay (250);
    }


/********************************************************** SET Interval*/

     set = false;
   while(set == false)
     {
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("How often:");
       lcd.setCursor(0, 1);
       lcd.print("Every ");
       lcd.setCursor(6, 1);
       lcd.print(Interval);
       lcd.setCursor(8, 1);
       lcd.print("hours");
       
     
      if (HIGH == digitalRead(PIN_BUTTON1))
      {
         lcd.noBlink();
         Interval += 1;       
      }
       if (HIGH == digitalRead(PIN_BUTTON2))
      {
        set = true;       
      }
        
    if (Interval > 24)
    { 
     Time_Water_h = 0;
    }
     delay (250);
    }



/********************************************************** SET Length*/

     set = false;
   while(set == false)
     {
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("For how long:");
       lcd.setCursor(0, 1);
       lcd.print(Water_howlong);
       lcd.setCursor(3, 1);
       lcd.print("minutes");
     
      if (HIGH == digitalRead(PIN_BUTTON1))
      {
         lcd.noBlink();
         Water_howlong += 1;       
      }
       if (HIGH == digitalRead(PIN_BUTTON2))
      {
        set = true;       
      }
        
    if (Water_howlong > 30)
    { 
     Water_howlong = 0;
    }
     delay (250);
    }

    lcd.clear();

}


void ReadWaterLvl()
{
  value = analogRead(A0); //Read data from analog pin and store it to value variable
  Serial.println(value); 
 
}

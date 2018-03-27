//code written for Arduino-NANO by praveen kumar
#include <LiquidCrystal.h> 
#include <math.h>
#define BUZ  10
const int trigPin = 9;                            //9th pins of Arduino connected to trigger pin of HC-SR04(sends a pulse)
const int echoPin = 8;                            //8th pin of Arduino connected to echo pin of HC-SR04(receives pulse)           
long duration, inches, cm;                        //these are the variables used in the code
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
String str="WATER LEVEL: ";                       // creating string
String CM,STRR,STR1,IND="FLUD DETECTED\n",LVL="WATER LEVEL=";
void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);                                //initializing the display(16 columns,2 rows)
  lcd.print("FLUID DETECTION"); 
  lcd.setCursor(0,1);
  lcd.print("&ALERTING SYSTM "); 
  pinMode(trigPin, OUTPUT);                        //defining as output
  pinMode(echoPin, INPUT);                         //defining as input 
  pinMode(BUZ,     OUTPUT);                        //defining as output(which is used to controll the motor
  delay(2000);
  digitalWrite(BUZ, HIGH);
  gsm_init(); 
  lcd.setCursor(0,0);
   
}

void loop()
{//lcd.clear();
   lcd.setCursor(0,0);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);                    //this sends a 10 microseconds pulse
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);                   
  duration = pulseIn(echoPin, HIGH);              //calculates the pulse width according to triggered pulse travell
  cm = microsecondsToCentimeters(duration);       //gets the duration in terms of distance in cm
  if(cm>100)cm=100;                               //limits the cm value to 103 cm
  cm=100-cm;
  CM=(String)cm;
  
  lcd.print("WATER LEVL: ");
  lcd.setCursor(12,0);
  if(cm>=75)SendMessage();
  if((cm<75)&&(cm>=50)){
    lcd.print("HIGH");

  }
  if((cm<50)&&(cm>=25)){
    lcd.print("NORM");
  }
  if(cm<25){
    lcd.print("LOW ");
   
  }
  //Serial.println(cm);
  lcd.setCursor(0,1);
  lcd.print("WATER HIGT:");
  lcd.print(cm);
  lcd.print("cm   ");
  delay(300);
}
void SendMessage()                                    //sents a message
{ 
  digitalWrite(BUZ, LOW);                             //buzzer active
  delay(100);
  digitalWrite(BUZ, HIGH);                            //buzzer off
  lcd.clear();                                        //clears the display
  lcd.print("  FLUD DETECTED");                       //prints on the screen
  lcd.setCursor(0,1);
  lcd.print(" SENDING MESSAGE");
  delay(100);
  digitalWrite(BUZ, LOW);                             //buzzer active
  delay(100);
  digitalWrite(BUZ, HIGH);                            //buzzer off
  STR1=LVL+CM;
  STRR=IND+STR1+"cm";
  Serial.println("AT+CMGF=1");                        //command to set GSM module in text mode 
  delay(1000);                                        //wait for a second
  Serial.println("AT+CMGS=\"+91**********\"\r");      //command to send message to that number
  digitalWrite(BUZ, LOW);                             //buzzer active
  delay(500);
  Serial.println(STRR);                               //print the message contained string
  //Serial.print("cm");
  digitalWrite(BUZ, HIGH);                            //buzzer off
  delay(100);
  Serial.println((char)26);                           //ending the message(Ctrl+Z in ASCCI)
  digitalWrite(BUZ, LOW);                             //buzzer active
  delay(300);
  digitalWrite(BUZ, HIGH);                            //buzzer off
  delay(3000);
  lcd.clear();                                
 
}
void gsm_init()                                       //this function calls to initialise the GSM module
{
  lcd.clear();
  lcd.print("GSM initializing...." );
 boolean at_flag=1;                                   //flage for check network
 while(at_flag)
  {
   Serial.println("AT");
   while(Serial.available()>0)
    {
     if(Serial.find("OK"))
     at_flag=0;
    }
    delay(1000);
  }
  boolean echo_flag=1;
  while(echo_flag)
   {
    Serial.println("ATE0");                               //to check network signal
    while(Serial.available()>0) 
     {
      if(Serial.find("OK"))
      echo_flag=0;
     }
     delay(1000);
   }
   boolean net_flag=1;
   while(net_flag)
    {
     Serial.println("AT+CPIN?");                          //confirmation command
     while(Serial.available()>0)
      {
       if(Serial.find("+CPIN: READY"))net_flag=0;
      }
      delay(1000);
    }lcd.clear();
}
long microsecondsToCentimeters(long microseconds)        // The speed of sound is 340 m/s or 29 microseconds per centimeter.
{                                                        // The ping travels out and back, so to find the distance of the
  return microseconds / 29 / 2;                          // object we take half of the distance travelled.
  
}

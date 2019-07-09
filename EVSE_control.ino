#include <Wire.h>
int LED1 =22;
int LED2 =24;
int LED3 =26;
int LED4 =28;
int pushbutton =30;
int overcuroff =7;
int grnd =  8;
int amps = 0;
int numbtnpush = 0;
int avalcurrent = 0;

int led1val =0;
int led2val =0;
int led3val =0;
int led4val =0;

int led1delval =0;
int led2delval =0;
int led3delval =0;
int led4delval =0;

int difled1;
int difled2;
int difled3;
int difled4;

//code for RMS current reading  http://henrysbench.capnfatz.com/henrys-bench/arduino-current-measurements/acs712-arduino-ac-current-tutorial/
const int sensorIn = A0;
int mVperAmp = 41; // use 100 for 20A Module and 66 for 30A Module
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;


void setup() {
pinMode(LED1, INPUT);
pinMode(LED2, INPUT);
pinMode(LED3, INPUT);
pinMode(LED4, INPUT);

pinMode(grnd, OUTPUT);
pinMode(pushbutton, OUTPUT);
pinMode(overcuroff, OUTPUT);
Serial.begin(9600); 
}

void loop() {

goto READ;
INCREASE:
//Serial.println ("increase loop");
digitalWrite(pushbutton, HIGH);
digitalWrite(grnd, LOW);
delay (300);
digitalWrite(pushbutton, LOW);
numbtnpush = numbtnpush - 1;
if (numbtnpush > 0){;
goto INCREASE; 
}

READ:
//Serial.println ("read loop");
//read current provided by inverter

// 50 amp bidirectional 41mv/amp
float getVPP();
Voltage = getVPP();
VRMS = (Voltage/2.0) *0.707; 
AmpsRMS = (VRMS * 1000)/mVperAmp*2; //multiplied by 2
Serial.print("Inverter Output: ");
Serial.println (AmpsRMS);

//read current setting on device
led1val = digitalRead (LED1);
led2val = digitalRead (LED2);
led3val = digitalRead (LED3);
led4val = digitalRead (LED4);

delay (500);

led1delval = digitalRead (LED1);
led2delval = digitalRead (LED2);
led3delval = digitalRead (LED3);
led4delval = digitalRead (LED4);

//Serial.print("ILED vals");
//Serial.print(led1val);
//Serial.print(led2val);
//Serial.print(led3val);
//Serial.print(led4val);



difled1 = led1val - led1delval;
difled2 = led2val - led2delval;
difled3 = led3val - led3delval;
difled4 = led4val - led4delval;

//Serial.print("DIF-LED vals");
//Serial.print(difled1);
//Serial.print(difled2);
//Serial.print(difled3);
//Serial.print(difled4);

//determine amp setting

if (led1val == 0 && led2val == 1 && led3val == 1 && led4val == 1 && difled2 == 0){;
amps =6;
//Serial.print("amps=6");
}

if (led2val == 1 && led3val == 1 && led4val == 1 && difled1 != 0){;
amps =7;
//Serial.print("amps=7");
}

if (led1val == 1 && led2val == 0 && led3val == 1 && led4val == 1 && difled2 == 0){;
amps =8;
//Serial.print("amps=8");
}
if (led1val == 1 && led3val == 1 && led4val == 1 && difled2 != 0){;
amps =9;
//Serial.print("amps=9");
}
if (led1val == 1 && led2val == 1 && led3val == 0 && led4val == 1 && difled3 == 0){;
amps =10;
//Serial.print("amps=10");
}
if (led1val == 1 && led2val == 1 && led4val == 1 && difled3 != 0){;
amps =11;
//Serial.print("amps=11");
}
if (led1val == 1 && led2val == 1 && led3val == 1 && led4val == 0 && difled4 == 0){;
amps =12;
//Serial.print("amps=12");
}
if (led1val == 1 && led2val == 1 && led3val == 1 && difled4 != 0){;
amps =13;
//Serial.print("amps=13");
}
if (led1val == 0 && led2val == 0 && led3val == 1 && led4val == 1){;
amps =14;
//Serial.print("amps=14");
}
if (led2val == 0 && led3val == 1 && led4val == 1 && difled1 != 0){;
amps =15;
//Serial.print("amps=15");
}
if (led1val == 0 && led2val == 1 && led3val == 0 && led4val == 1){;
amps =16;
//Serial.print("amps=16");
}
Serial.print(", EVSE amps = ");
Serial.print(amps);

avalcurrent = 18-AmpsRMS;

if (avalcurrent >1 && amps!=16){;
numbtnpush = 1;

Serial.print(", avalible current: ");
Serial.print(avalcurrent);
Serial.print(", number of pushes: ");
Serial.println (numbtnpush);
goto INCREASE;
}

if (avalcurrent <1 && amps > 6){;
numbtnpush = 17-amps; //take current level to 8 amps 

Serial.print(", avalible current");
Serial.print(avalcurrent);
Serial.print(", number of pushes");
Serial.println (numbtnpush);
goto INCREASE;
}

if (avalcurrent <1 && amps == 8){;
digitalWrite (overcuroff, LOW);
delay (10000);
digitalWrite (overcuroff, HIGH);
goto READ;
}
}

float getVPP()
{
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }



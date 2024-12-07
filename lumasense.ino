
/********************************************************************************************
*/#define softVersion "AQma LED Control, version 3.1.10"/*
/  Copyright (c) 2017 Marcin Grunt
/  Wszelkie prawa do AQma LED Control są zastrzeżone przez autora: Marcina Grunta.
/  Poniższy kod programu można używać i rozpowszechniać za darmo, pod warunkiem
/  zachowania informacji o licencji i autorstwie.
/  Poniższy kod udostępniony jest bez żadnej gwarancji, używasz go na własne ryzyko.
/  Autor nie ponosi odpowiedzialności za szkody, utratę zysków, lub jakiekolwiek
/  inne straty wynikłe w konsekwencji uzywania, lub niemożności użycia poniższego kodu.
/ 
/  Pozdrawiam i życzę bezproblemowej pracy:
/                           Marcin Grunt, magu@magu.pl
*********************************************************************************************/

#include <Wire.h>
#include <Time.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include <DS1307RTC.h>
#include <SoftwareSerial.h>

////////////////////////////////////
/////////// Ustawienia  ////////////
////////////////////////////////////
#define ledPin A0                ///   
#define keyboardPin A3           ///
boolean ON=true, OFF=false;      ///
                                 ///
boolean pwm1Invert = false;      ///
boolean pwm2Invert = false;      ///
boolean pwm3Invert = false;      ///
boolean pwm4Invert = false;      ///
boolean pwm5Invert = false;      ///
boolean pwm6Invert = false;      ///
byte pwm1Pin = 3;                ///
byte pwm2Pin = 5;                ///
byte pwm3Pin = 6;                ///
byte pwm4Pin = 9;                ///
byte pwm5Pin = 10;               ///
byte pwm6Pin = 11;               ///
                                 ///
byte fan1Pin = 4;                ///
byte fan2Pin = 7;                ///
byte fan3Pin = 8;                ///
int sleepTime = 600;             ///
byte upInterval = 60;            ///
byte fanTreshold = 10;           ///
byte downInterval = 10;          ///
byte powerOnInterval = 66;       ///
                                 ///
                                 ///
int beepPin = 2;                ///
////////////////////////////////////
////////////////////////////////////

////////// Bluetooth pin(A2, A1)  /////////
                                         //
SoftwareSerial bluetooth(A2,A1);         //
                                         //
///////////////////////////////////////////


byte mode = 6;
tmElements_t tm;
int btnCounter = 0;
char cmdOutputArray[64];
boolean stopPowerUp = false;
long unsigned currentTimeSec;
byte pwm1, pwm2, pwm3, pwm4, pwm5, pwm6; 
byte getPwm1, getPwm2, getPwm3, getPwm4, getPwm5, getPwm6;
boolean beep = true, manualMode = false, keyboardBtnState = false, longBeep = false;
byte testPwm1 = 0, testPwm2 = 0, testPwm3 = 0, testPwm4 = 0, testPwm5 = 0, testPwm6 = 0;
byte testPwm1Val = 0, testPwm2Val = 0, testPwm3Val = 0, testPwm4Val = 0, testPwm5Val = 0, testPwm6Val = 0;
boolean stopPwm1Counter = false, stopPwm2Counter = false, stopPwm3Counter = false, stopPwm4Counter = false, stopPwm5Counter = false, stopPwm6Counter = false;
boolean stopPwm1CounterUp = false, stopPwm2CounterUp = false, stopPwm3CounterUp = false, stopPwm4CounterUp = false, stopPwm5CounterUp = false, stopPwm6CounterUp = false;
boolean stopAutoPwm1Counter = false, stopAutoPwm2Counter = false, stopAutoPwm3Counter = false, stopAutoPwm4Counter = false, stopAutoPwm5Counter = false, stopAutoPwm6Counter = false;


// PWM - 1
 byte pwm1Status, pwm1HOn, pwm1MOn, pwm1SOn;
 byte pwm1HOff, pwm1MOff, pwm1SOff, pwm1Min, pwm1Max;
 byte pwm1Sr, pwm1Ss, pwm1KeepLight;

// PWM - 2
 byte pwm2Status, pwm2HOn, pwm2MOn, pwm2SOn;
 byte pwm2HOff, pwm2MOff, pwm2SOff, pwm2Min, pwm2Max;
 byte pwm2Sr, pwm2Ss, pwm2KeepLight;

// PWM - 3
 byte pwm3Status, pwm3HOn, pwm3MOn, pwm3SOn;
 byte pwm3HOff, pwm3MOff, pwm3SOff, pwm3Min, pwm3Max;
 byte pwm3Sr, pwm3Ss, pwm3KeepLight;

// PWM - 4
 byte pwm4Status, pwm4HOn, pwm4MOn, pwm4SOn;
 byte pwm4HOff, pwm4MOff, pwm4SOff, pwm4Min, pwm4Max;
 byte pwm4Sr, pwm4Ss, pwm4KeepLight;

// PWM - 5
 byte pwm5Status, pwm5HOn, pwm5MOn, pwm5SOn;
 byte pwm5HOff, pwm5MOff, pwm5SOff, pwm5Min, pwm5Max;
 byte pwm5Sr, pwm5Ss, pwm5KeepLight;

 // PWM - 6
 byte pwm6Status, pwm6HOn, pwm6MOn, pwm6SOn;
 byte pwm6HOff, pwm6MOff, pwm6SOff, pwm6Min, pwm6Max;
 byte pwm6Sr, pwm6Ss, pwm6KeepLight;

// KL
 byte klState, kl1HOn, kl1MOn, kl1HOff, kl1MOff, kl2HOn, kl2MOn, kl2HOff, kl2MOff;
 boolean klEepromRead = false; 
 boolean klCurrentState = false;

 boolean silkyStart = true;
 boolean beepConnect = true;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// SETUP //////////////////////////////////////////////////////////////////////

void setup()
{
wdt_enable(WDTO_8S);
Serial.begin(9600);
bluetooth.begin(9600);
 
 
  pinMode(pwm1Pin,OUTPUT);    pinMode(pwm2Pin,OUTPUT); 
  pinMode(pwm3Pin, OUTPUT);   pinMode(pwm4Pin, OUTPUT);  
  pinMode(pwm5Pin, OUTPUT);   pinMode(pwm6Pin, OUTPUT); 
  pinMode(beepPin, OUTPUT);   pinMode(ledPin, OUTPUT);
  pinMode(fan1Pin, OUTPUT);   pinMode(fan2Pin, OUTPUT); 
  pinMode(fan3Pin, OUTPUT); 
  pinMode(keyboardPin, INPUT_PULLUP);
 
// Ustaw stany poczatkowe
  digitalWrite(pwm1Pin,pwm1Invert);    digitalWrite(pwm2Pin,pwm2Invert);
  digitalWrite(pwm3Pin,pwm3Invert);    digitalWrite(pwm4Pin,pwm4Invert);
  digitalWrite(pwm5Pin,pwm5Invert);    digitalWrite(pwm6Pin,pwm6Invert); 

  digitalWrite(ledPin,HIGH);

  eEpromRead();
  defaultPwm();
  resetStopAutoPwmCounter();

}
////////////////////////////////////////////////////////////// END SETUP //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// *************************************************************************************************************************************** ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////// LOOP ///////////////////////////////////////////////////////////////////////

void loop()
{ 
   wdt_reset();
   
   long unsigned currentMillis= millis();
   getPwm();
   rtcRead();
   apiCommand();
   
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
//////////////////////////////////////// SILNIK //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

         turnOnFan(fan1Pin, getPwm1, pwm1Invert);
         turnOnFan(fan2Pin, getPwm2, pwm2Invert); 
         turnOnFan(fan3Pin, getPwm3, pwm3Invert);

         
                 
          //silkySmoothPowerOn(currentMillis);
          if ( currentMillis < 2) { manualMode=true; mode = 6; }
          btnClick();
          
          if (!manualMode) {
              btnCounter = 0;
           /* if ( (millis()%1000) == 0) { 
              Serial.print("Pozycja 0 - keepLightCheck1: "); Serial.println(keepLighCheck( klState, kl1HOn, kl1MOn, kl1HOff, kl1MOff));
              Serial.print("Pozycja 0 - keepLightCheck2: "); Serial.println(keepLighCheck( klState, kl2HOn, kl2MOn, kl2HOff, kl2MOff));
              Serial.print("klState: "); Serial.println(klState);
              }
            */
            
              if (klState == 1) {
                
                  if ( (!keepLighCheck( klState, kl1HOn, kl1MOn, kl1HOff, kl1MOff)) && (!keepLighCheck( klState, kl2HOn, kl2MOn, kl2HOff, kl2MOff))  ) {
                 
              //      if ( (millis()%500) == 0) { Serial.println("OFF"); }
                    moonlightOff();
                    klCurrentState = false;
                    klEepromRead = false;
                }
                
                else {
                  klCurrentState = true;
                  if (!klEepromRead) {eEpromRead(); klEepromRead = true; } 
                  //if ( (millis()%555) == 0) { Serial.println("On"); }
 
                }
              }
              

                    if (testPwm1 == 1) { channelTest(pwm1Pin, testPwm1Val, pwm1Invert);  } else { pwm1 = pwm(pwm1Pin, pwm1Status, pwm1HOn, pwm1MOn, pwm1SOn, pwm1HOff, pwm1MOff, pwm1SOff, pwm1Min, pwm1Max, pwm1Sr, pwm1Ss, pwm1KeepLight, pwm1Invert); }
                    if (testPwm2 == 1) { channelTest(pwm2Pin, testPwm2Val, pwm2Invert);  } else { pwm2 = pwm(pwm2Pin, pwm2Status, pwm2HOn, pwm2MOn, pwm2SOn, pwm2HOff, pwm2MOff, pwm2SOff, pwm2Min, pwm2Max, pwm2Sr, pwm2Ss, pwm2KeepLight, pwm2Invert); }
                    if (testPwm3 == 1) { channelTest(pwm3Pin, testPwm3Val, pwm3Invert);  } else { pwm3 = pwm(pwm3Pin, pwm3Status, pwm3HOn, pwm3MOn, pwm3SOn, pwm3HOff, pwm3MOff, pwm3SOff, pwm3Min, pwm3Max, pwm3Sr, pwm3Ss, pwm3KeepLight, pwm3Invert); }
                    if (testPwm4 == 1) { channelTest(pwm4Pin, testPwm4Val, pwm4Invert);  } else { pwm4 = pwm(pwm4Pin, pwm4Status, pwm4HOn, pwm4MOn, pwm4SOn, pwm4HOff, pwm4MOff, pwm4SOff, pwm4Min, pwm4Max, pwm4Sr, pwm4Ss, pwm4KeepLight, pwm4Invert); }
                    if (testPwm5 == 1) { channelTest(pwm5Pin, testPwm5Val, pwm5Invert);  } else { pwm5 = pwm(pwm5Pin, pwm5Status, pwm5HOn, pwm5MOn, pwm5SOn, pwm5HOff, pwm5MOff, pwm5SOff, pwm5Min, pwm5Max, pwm5Sr, pwm5Ss, pwm5KeepLight, pwm5Invert); }
                    if (testPwm6 == 1) { channelTest(pwm6Pin, testPwm6Val, pwm6Invert);  } else { pwm6 = pwm(pwm6Pin, pwm6Status, pwm6HOn, pwm6MOn, pwm6SOn, pwm6HOff, pwm6MOff, pwm6SOff, pwm6Min, pwm6Max, pwm6Sr, pwm6Ss, pwm6KeepLight, pwm6Invert); }
           }

         
           if (manualMode) {
              //Off 
             if (mode == 0) {
              silkyDown(currentMillis,downInterval);
              if (secondTick(currentMillis)) { btnCounter++; }
              if ( btnCounter >= sleepTime ) {  mode=2; }
              }
            
             //On
             if (mode == 1) {
              silkyUp(currentMillis,upInterval);
              if (secondTick(currentMillis)) { btnCounter++; }
              if ( btnCounter >= sleepTime ) { mode = 2; }
              }

             //Enter to auto mode
             if (mode == 2) {
              enterToAutoMode(currentMillis);
                 
              }

             //Power On
             if (mode == 6) {
              silkyPowerUp(currentMillis, powerOnInterval);
               
              }


              
           }
                  
          if ( !beepCheck(beepPin, 120) ) { digitalWrite(ledPin, HIGH); } else { digitalWrite(ledPin, LOW); }

 
}
////////////////////////////////////////////////////////////// END LOOP////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
/////                   silkyUP                                                    /////
////////////////////////////////////////////////////////////////////////////////////////
boolean silkyUp(unsigned long currentMillis, byte upInterval){
  static unsigned long lastSilkyUpTime = currentMillis; 
  
  if ( currentMillis - lastSilkyUpTime >= upInterval ) { 
//PWM1
      if (!pwm1Invert && pwm1 < pwm1Max && !stopPwm1CounterUp) { ++pwm1; analogWrite(pwm1Pin,pwm1); }
      if (!pwm1Invert && pwm1 >=pwm1Max && !stopPwm1CounterUp) { pwm1 = pwm1Max; stopPwm1CounterUp = true; } 
    
      if (pwm1Invert && pwm1 > (255-pwm1Max) && !stopPwm1CounterUp) { --pwm1; analogWrite(pwm1Pin,pwm1); }
      if (pwm1Invert && pwm1 <= (255-pwm1Max) && !stopPwm1CounterUp) { pwm1 = (255-pwm1Max); stopPwm1CounterUp = true; } 
//PWM2
      if (!pwm2Invert && pwm2 < pwm2Max && !stopPwm2CounterUp) { ++pwm2; analogWrite(pwm2Pin,pwm2); }
      if (!pwm2Invert && pwm2 >=pwm2Max && !stopPwm2CounterUp) { pwm2 = pwm2Max; stopPwm2CounterUp = true; } 
    
      if (pwm2Invert && pwm2 > (255-pwm2Max) && !stopPwm2CounterUp) { --pwm2; analogWrite(pwm2Pin,pwm2); }
      if (pwm2Invert && pwm2 <= (255-pwm2Max) && !stopPwm2CounterUp) { pwm2 = (255-pwm2Max); stopPwm2CounterUp = true; } 
//PWM3
      if (!pwm3Invert && pwm3 < pwm3Max && !stopPwm3CounterUp) { ++pwm3; analogWrite(pwm3Pin,pwm3); }
      if (!pwm3Invert && pwm3 >=pwm3Max && !stopPwm3CounterUp) { pwm3 = pwm3Max; stopPwm3CounterUp = true; } 
    
      if (pwm3Invert && pwm3 > (255-pwm3Max) && !stopPwm3CounterUp) { --pwm3; analogWrite(pwm3Pin,pwm3); }
      if (pwm3Invert && pwm3 <= (255-pwm3Max) && !stopPwm3CounterUp) { pwm3 = (255-pwm3Max); stopPwm3CounterUp = true; } 
//PWM4
      if (!pwm4Invert && pwm4 < pwm4Max && !stopPwm4CounterUp) { ++pwm4; analogWrite(pwm4Pin,pwm4); }
      if (!pwm4Invert && pwm4 >=pwm4Max && !stopPwm4Counter) { pwm4 = pwm4Max; stopPwm4CounterUp = true; } 
    
      if (pwm4Invert && pwm4 > (255-pwm4Max) && !stopPwm4CounterUp) { --pwm4; analogWrite(pwm4Pin,pwm4); }
      if (pwm4Invert && pwm4 <= (255-pwm4Max) && !stopPwm4CounterUp) { pwm4 = (255-pwm4Max); stopPwm4CounterUp = true; } 
//PWM5
      if (!pwm5Invert && pwm5 < pwm5Max && !stopPwm5CounterUp) { ++pwm5; analogWrite(pwm5Pin,pwm5); }
      if (!pwm5Invert && pwm5 >=pwm5Max && !stopPwm5CounterUp) { pwm5 = pwm5Max; stopPwm5CounterUp = true; } 
    
      if (pwm5Invert && pwm5 > (255-pwm5Max) && !stopPwm5CounterUp) { --pwm5; analogWrite(pwm5Pin,pwm5); }
      if (pwm5Invert && pwm5 <= (255-pwm5Max) && !stopPwm5CounterUp) { pwm5 = (255-pwm5Max); stopPwm5CounterUp = true; } 
//PWM6
      if (!pwm6Invert && pwm6 < pwm6Max && !stopPwm6CounterUp) { ++pwm6; analogWrite(pwm6Pin,pwm6); }
      if (!pwm6Invert && pwm6 >=pwm6Max && !stopPwm6CounterUp) { pwm6 = pwm6Max; stopPwm6CounterUp = true; } 
    
      if (pwm6Invert && pwm6 > (255-pwm6Max) && !stopPwm6CounterUp) { --pwm6; analogWrite(pwm6Pin,pwm6); }
      if (pwm6Invert && pwm6 <= (255-pwm6Max) && !stopPwm6CounterUp) { pwm6 = (255-pwm6Max); stopPwm6CounterUp = true; } 
    
      lastSilkyUpTime = currentMillis;
  }
}


////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
/////                   resetSilkyUpCounter                                        /////
////////////////////////////////////////////////////////////////////////////////////////
void resetSilkyUpCounter() {
    stopPwm1CounterUp = false;
    stopPwm2CounterUp = false;
    stopPwm3CounterUp = false; 
    stopPwm4CounterUp = false; 
    stopPwm5CounterUp = false; 
    stopPwm6CounterUp = false;
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/////                   silkyDown                                                  /////
////////////////////////////////////////////////////////////////////////////////////////
boolean silkyDown(unsigned long currentMillis, byte downInterval) {
 static unsigned long lastSilkyDownTime = currentMillis; 
 
   if ( currentMillis - lastSilkyDownTime >= downInterval ) {
    // PWM1
      if (!pwm1Invert && pwm1 > 0 && !stopPwm1Counter) { --pwm1; analogWrite(pwm1Pin,pwm1);   }
      if (!pwm1Invert && pwm1 <=0 && !stopPwm1Counter) { pwm1 = 0; stopPwm1Counter = true; } 
       
      if (pwm1Invert && pwm1 < 255 && !stopPwm1Counter ) { ++pwm1; analogWrite(pwm1Pin,pwm1); }
      if (pwm1Invert && pwm1 >= 255 && !stopPwm1Counter) { pwm1 = 255; stopPwm1Counter = true; }
    //PWM2
      if (!pwm2Invert && pwm2 > 0 && !stopPwm2Counter) { --pwm2; analogWrite(pwm2Pin,pwm2);   }
      if (!pwm2Invert && pwm2 <=0 && !stopPwm2Counter) { pwm2 = 0; stopPwm2Counter = true; } 
       
      if (pwm2Invert && pwm2 < 255 && !stopPwm2Counter ) { ++pwm2; analogWrite(pwm2Pin,pwm2); }
      if (pwm2Invert && pwm2 >= 255 && !stopPwm2Counter) { pwm2 = 255; stopPwm2Counter = true; }
    //PWM3
      if (!pwm3Invert && pwm3 > 0 && !stopPwm3Counter) { --pwm3; analogWrite(pwm3Pin,pwm3);   }
      if (!pwm3Invert && pwm3 <=0 && !stopPwm3Counter) { pwm3 = 0; stopPwm3Counter = true; } 
       
      if (pwm3Invert && pwm3 < 255 && !stopPwm3Counter ) { ++pwm3; analogWrite(pwm3Pin,pwm3); }
      if (pwm3Invert && pwm3 >= 255 && !stopPwm3Counter) { pwm3 = 255; stopPwm3Counter = true; }
    //PWM4
      if (!pwm4Invert && pwm4 > 0 && !stopPwm4Counter) { --pwm4; analogWrite(pwm4Pin,pwm4);   }
      if (!pwm4Invert && pwm4 <=0 && !stopPwm4Counter) { pwm4 = 0; stopPwm4Counter = true; } 
       
      if (pwm4Invert && pwm4 < 255 && !stopPwm4Counter ) { ++pwm4; analogWrite(pwm4Pin,pwm4); }
      if (pwm4Invert && pwm4 >= 255 && !stopPwm4Counter) { pwm4 = 255; stopPwm4Counter = true; }
    //PWM5
      if (!pwm5Invert && pwm5 > 0 && !stopPwm5Counter) { --pwm5; analogWrite(pwm5Pin,pwm5);   }
      if (!pwm5Invert && pwm5 <=0 && !stopPwm5Counter) { pwm5 = 0; stopPwm5Counter = true; } 
       
      if (pwm5Invert && pwm5 < 255 && !stopPwm5Counter ) { ++pwm5; analogWrite(pwm5Pin,pwm5); }
      if (pwm5Invert && pwm5 >= 255 && !stopPwm5Counter) { pwm5 = 255; stopPwm5Counter = true; }
    //PWM6
      if (!pwm6Invert && pwm6 > 0 && !stopPwm6Counter) { --pwm6; analogWrite(pwm6Pin,pwm6);   }
      if (!pwm6Invert && pwm6 <=0 && !stopPwm6Counter) { pwm6 = 0; stopPwm6Counter = true; } 
       
      if (pwm6Invert && pwm6 < 255 && !stopPwm6Counter ) { ++pwm6; analogWrite(pwm6Pin,pwm6); }
      if (pwm6Invert && pwm6 >= 255 && !stopPwm6Counter) { pwm6 = 255; stopPwm6Counter = true; }  

      lastSilkyDownTime = currentMillis;
      }
  }



////////////////////////////////////////////////////////////////////////////////////////
/////                   resetSilkyDownCounter                                      /////
////////////////////////////////////////////////////////////////////////////////////////
void resetSilkyDownCounter() {
    stopPwm1Counter = false;
    stopPwm2Counter = false;
    stopPwm3Counter = false; 
    stopPwm4Counter = false; 
    stopPwm5Counter = false; 
    stopPwm6Counter = false;
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/////                   resetStopAutoPwmCounter                                        /////
////////////////////////////////////////////////////////////////////////////////////////
void resetStopAutoPwmCounter() {
    stopAutoPwm1Counter = false;
    stopAutoPwm2Counter = false;
    stopAutoPwm3Counter = false; 
    stopAutoPwm4Counter = false; 
    stopAutoPwm5Counter = false; 
    stopAutoPwm6Counter = false;
}
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
/////                   silkyPowerUp                                               /////
////////////////////////////////////////////////////////////////////////////////////////
boolean silkyPowerUp(unsigned long currentMillis, byte powerOnInterval){
  static unsigned long lastSilkyPowerUpTime = currentMillis; 
  static int powerCounter = 0;
 
 if (currentMillis - lastSilkyPowerUpTime >= powerOnInterval) {
    if (stopPowerUp) {
      exitFromManualMode(); 
      //Serial.println("Bzyyt... Wychodze, co bede z martwymi cielakami siedzial.");
    }
   
//PWM1
    if (!stopPowerUp && !pwm1Invert && (powerCounter < getPwm1) ) { pwm1 = powerCounter; analogWrite(pwm1Pin,powerCounter);}
    if (powerCounter == getPwm1) { pwm1 = powerCounter; }
  
    if (!stopPowerUp && pwm1Invert && ( (255-powerCounter) > getPwm1) ) { pwm1 = powerCounter; analogWrite(pwm1Pin,(255-powerCounter));}
    if (powerCounter == getPwm1) { pwm1 = powerCounter; }
//PWM2
    if (!stopPowerUp && !pwm2Invert && (powerCounter < getPwm2) ) { pwm2 = powerCounter; analogWrite(pwm2Pin,powerCounter);}
    if (powerCounter == getPwm2) { pwm2 = powerCounter; }
  
    if (!stopPowerUp && pwm2Invert && ( (255-powerCounter) > getPwm2) ) { pwm2 = powerCounter; analogWrite(pwm2Pin,(255-powerCounter));}
    if (powerCounter == getPwm2) { pwm2 = powerCounter; }
//PWM3
    if (!stopPowerUp && !pwm3Invert && (powerCounter < getPwm3) ) { pwm3 = powerCounter; analogWrite(pwm3Pin,powerCounter);}
    if (powerCounter == getPwm3) { pwm3 = powerCounter; }
  
    if (!stopPowerUp && pwm3Invert && ( (255-powerCounter) > getPwm3) ) { pwm3 = powerCounter; analogWrite(pwm3Pin,(255-powerCounter));}
    if (powerCounter == getPwm3) { pwm3 = powerCounter; }
//PWM4
    if (!stopPowerUp && !pwm4Invert && (powerCounter < getPwm4) ) { pwm4 = powerCounter; analogWrite(pwm4Pin,powerCounter);}
    if (powerCounter == getPwm4) { pwm4 = powerCounter; }
  
    if (!stopPowerUp && pwm4Invert && ( (255-powerCounter) > getPwm4) ) { pwm4 = powerCounter; analogWrite(pwm4Pin,(255-powerCounter));}
    if (powerCounter == getPwm4) { pwm4 = powerCounter; }
//PWM5
    if (!stopPowerUp && !pwm5Invert && (powerCounter < getPwm5) ) { pwm5 = powerCounter; analogWrite(pwm5Pin,powerCounter);}
    if (powerCounter == getPwm5) { pwm5 = powerCounter; }
  
    if (!stopPowerUp && pwm5Invert && ( (255-powerCounter) > getPwm5) ) { pwm5 = powerCounter; analogWrite(pwm5Pin,(255-powerCounter));}
    if (powerCounter == getPwm5) { pwm5 = powerCounter; }
//PWM6
    if (!stopPowerUp && !pwm6Invert && (powerCounter < getPwm6) ) { pwm6 = powerCounter; analogWrite(pwm6Pin,powerCounter);}
    if (powerCounter == getPwm6) { pwm6 = powerCounter; }
  
    if (!stopPowerUp && pwm6Invert && ( (255-powerCounter) > getPwm6) ) { pwm6 = powerCounter; analogWrite(pwm6Pin,(255-powerCounter));}
    if (powerCounter == getPwm6) { pwm6 = powerCounter; }
    
    if (powerCounter >= 255) { stopPowerUp = true; }
     
      ++powerCounter; 
      lastSilkyPowerUpTime = currentMillis;
  }

}                    


////////////////////////////////////////////////////////////////////////////////////////
/////                   enterToAutoMode                                            /////
////////////////////////////////////////////////////////////////////////////////////////
boolean enterToAutoMode(unsigned long currentMillis) {
  static unsigned long lastSilkyAutoUpTime = currentMillis; 

  if (stopAutoPwm1Counter && stopAutoPwm2Counter) { exitFromManualMode(); resetStopAutoPwmCounter();  return false; }
 
      if ( currentMillis - lastSilkyAutoUpTime >= upInterval ) {

    //PWM1
          if (pwm1 < getPwm1 && !stopAutoPwm1Counter) { ++pwm1; analogWrite(pwm1Pin,pwm1); }
          if (pwm1 > getPwm1 && !stopAutoPwm1Counter) { --pwm1; analogWrite(pwm1Pin,pwm1); }
          if (getPwm1 == pwm1) { stopAutoPwm1Counter = true; }
   
   //PWM2
          if (pwm2 < getPwm2 && !stopAutoPwm2Counter) { ++pwm2; analogWrite(pwm2Pin,pwm2); }
          if (pwm2 > getPwm2 && !stopAutoPwm2Counter) { --pwm2; analogWrite(pwm2Pin,pwm2); }
          if (getPwm2 == pwm2) { stopAutoPwm2Counter = true; }
  
   //PWM3
          if (pwm3 < getPwm3 && !stopAutoPwm3Counter) { ++pwm3; analogWrite(pwm3Pin,pwm3); }
          if (pwm3 > getPwm3 && !stopAutoPwm3Counter) { --pwm3; analogWrite(pwm3Pin,pwm3); }
          if (getPwm3 == pwm3) { stopAutoPwm3Counter = true; }   
                       
   //PWM4
          if (pwm4 < getPwm4 && !stopAutoPwm4Counter) { ++pwm4; analogWrite(pwm4Pin,pwm4); }
          if (pwm4 > getPwm4 && !stopAutoPwm4Counter) { --pwm4; analogWrite(pwm4Pin,pwm4); }
          if (getPwm4 == pwm4) { stopAutoPwm4Counter = true; }     

   //PWM5
          if (pwm5 < getPwm5 && !stopAutoPwm5Counter) { ++pwm5; analogWrite(pwm5Pin,pwm5); }
          if (pwm5 > getPwm5 && !stopAutoPwm5Counter) { --pwm5; analogWrite(pwm5Pin,pwm5); }
          if (getPwm5 == pwm5) { stopAutoPwm5Counter = true; }

   //PWM6
          if (pwm6 < getPwm6 && !stopAutoPwm6Counter) { ++pwm6; analogWrite(pwm6Pin,pwm6); }
          if (pwm6 > getPwm6 && !stopAutoPwm6Counter) { --pwm6; analogWrite(pwm6Pin,pwm6); }
          if (getPwm6 == pwm6) { stopAutoPwm6Counter = true; }          
          lastSilkyAutoUpTime = currentMillis;
     }
}


////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////
/////                         getPwm                                               /////
////////////////////////////////////////////////////////////////////////////////////////
void getPwm() {
  
  getPwm1 = checkPwmValue(pwm1Pin, pwm1Status, pwm1HOn, pwm1MOn, pwm1SOn, pwm1HOff, pwm1MOff, pwm1SOff, pwm1Min, pwm1Max, pwm1Sr, pwm1Ss, pwm1KeepLight, pwm1Invert);
  getPwm2 = checkPwmValue(pwm2Pin, pwm2Status, pwm2HOn, pwm2MOn, pwm2SOn, pwm2HOff, pwm2MOff, pwm2SOff, pwm2Min, pwm2Max, pwm2Sr, pwm2Ss, pwm2KeepLight, pwm2Invert);
  getPwm3 = checkPwmValue(pwm3Pin, pwm3Status, pwm3HOn, pwm3MOn, pwm3SOn, pwm3HOff, pwm3MOff, pwm3SOff, pwm3Min, pwm3Max, pwm3Sr, pwm3Ss, pwm3KeepLight, pwm3Invert);
  getPwm4 = checkPwmValue(pwm4Pin, pwm4Status, pwm4HOn, pwm4MOn, pwm4SOn, pwm4HOff, pwm4MOff, pwm4SOff, pwm4Min, pwm4Max, pwm4Sr, pwm4Ss, pwm4KeepLight, pwm4Invert);
  getPwm5 = checkPwmValue(pwm5Pin, pwm5Status, pwm5HOn, pwm5MOn, pwm5SOn, pwm5HOff, pwm5MOff, pwm5SOff, pwm5Min, pwm5Max, pwm5Sr, pwm5Ss, pwm5KeepLight, pwm5Invert);
  getPwm6 = checkPwmValue(pwm6Pin, pwm6Status, pwm6HOn, pwm6MOn, pwm6SOn, pwm6HOff, pwm6MOff, pwm6SOff, pwm6Min, pwm6Max, pwm6Sr, pwm6Ss, pwm6KeepLight, pwm6Invert);
}

////////////////////////////////////////////////////////////////////////////////////////
/////                   defaultPwm                                                 /////
////////////////////////////////////////////////////////////////////////////////////////
void defaultPwm() {

 
  if (pwm1Invert) { pwm1 = 255; }
  if (!pwm1Invert) { pwm1 = 0; }

  if (pwm2Invert) { pwm2 = 255; }
  if (!pwm2Invert) { pwm2 = 0; }

  if (pwm3Invert) { pwm3 = 255; }
  if (!pwm3Invert) { pwm3 = 0; }

  if (pwm4Invert) { pwm4 = 255; }
  if (!pwm4Invert) { pwm4 = 0; }

  if (pwm5Invert) { pwm5 = 255; }
  if (!pwm5Invert) { pwm5 = 0; }

  if (pwm6Invert) { pwm6 = 255; }
  if (!pwm6Invert) { pwm6 = 0; }
 
}
/////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////
/////                       rtcRead                                                 /////
/////////////////////////////////////////////////////////////////////////////////////////
void rtcRead() {
    if (RTC.read(tm))
   {
     currentTimeSec =  (long(tm.Hour)*3600) + (long(tm.Minute)*60) + long(tm.Second);
   }    
}

///////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
/////                       exitFromManualMode                                      /////
/////////////////////////////////////////////////////////////////////////////////////////
void exitFromManualMode() {
  keyboardBtnState = false; 
  btnCounter = 0; 
  beep = true; 
  mode = 3;
  manualMode = false;
}
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
/////                        apiCommand                                           /////
///////////////////////////////////////////////////////////////////////////////////////
void apiCommand(){
   char cmdChar = 0;
   String cmd = "";
   
    while (Serial.available() > 0)
     {
       cmdChar = Serial.read();
       delay(5);
       cmd.concat(cmdChar);
     }

    
    while (bluetooth.available() > 0)
     {
       cmdChar = bluetooth.read();
       delay(5);
       cmd.concat(cmdChar);
     }
  
     if (cmd != "") {
        
         cmd.toCharArray(cmdOutputArray,64);

           if (commandAnalysis(cmdOutputArray))
           {
              eEpromRead();
              exitFromManualMode();
             
           }
        
           else
           {
             Serial.print("666,Bledne dane\n");
             bluetooth.print("666,Bledne dane\n");
           }
         
     }
    
     cmd = "";
}
   
/////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
/////                  BeepCheck                                                   /////
////////////////////////////////////////////////////////////////////////////////////////
boolean beepCheck(byte pin, int beepTime)
{
  static unsigned long countBeep = 0;
  if (!beep) {  digitalWrite(pin, LOW);  return false;}

  if ( longBeep && !digitalRead(keyboardPin) ) { digitalWrite(pin,HIGH); return true;}
  if ( longBeep && digitalRead(keyboardPin) ) { beep=false; longBeep=false; return false; }
  if ( countBeep > beepTime && beepConnect) { countBeep = beepTime/3; digitalWrite(pin, HIGH); beep = true; beepConnect = false; return true;  }
  if ( countBeep > beepTime) { countBeep = 0; digitalWrite(pin, LOW); beep = false; beepConnect = false; return false; }

 int beepPart = beepTime/3;

  if ( beep &&  (countBeep <= beepPart ) )
  {

    digitalWrite(pin, HIGH);
    countBeep++;
    return true; 
  }
    if ( beep &&  (countBeep > beepPart) && (countBeep < beepPart*2) )
  {
    digitalWrite(pin, LOW);
    countBeep++;
    return true; 
  }
 
    if ( beep &&  (countBeep >= beepPart*2 ) )
  {
    digitalWrite(pin, HIGH);
    countBeep++;
    return true; 
  }
   
}

////////////////////////////////////////////////////////////////////////////
/////                     Button Click                                 /////
////////////////////////////////////////////////////////////////////////////
void btnClick()
{
  static unsigned long oldMillis = millis();
  static int btnCount = 0;
// if (btnCount != 0) { Serial.println(btnCount);}
  if(!digitalRead(keyboardPin) && millis()-oldMillis >= 1)
  {
    btnCount++;
    oldMillis = millis();
  }

// piszczenie
    if (!digitalRead(keyboardPin) && (btnCount >= 200) ) {
     beep = true;
     longBeep = true;
     return;
    }

// Powrot do automatu
    if (digitalRead(keyboardPin) && (btnCount >= 200) ) {
      mode = 2;
      manualMode = true;
      btnCount = 0;
      btnCounter = 0;
      resetSilkyDownCounter();
      resetSilkyUpCounter();
      digitalWrite(beepPin,LOW);
      delay(100);
      return;
    }
 
 // Po kliknieciu
  if (digitalRead(keyboardPin) && (btnCount > 4) && (btnCount< 200) )
  {
    resetSilkyDownCounter();
    resetSilkyUpCounter();
   
      
    if (!manualMode) { keyboardBtnState = false; } else {keyboardBtnState = !keyboardBtnState;}
     
      manualMode = true;
      
      // OFF
      if (!keyboardBtnState) { mode = 0;}
      
      // ON
      if (keyboardBtnState) { mode = 1;}

      btnCount = 0;
      btnCounter = 0;
      digitalWrite(beepPin,HIGH);
      delay(150);
      digitalWrite(beepPin,LOW); 
  }


 
}

////////////////////////////////////////////////////////////////////////////
/////                     Second Tick                                  /////
////////////////////////////////////////////////////////////////////////////
boolean secondTick(unsigned long currentMillis)
{
  static unsigned long counterLastMillis = 0;

  if (currentMillis - counterLastMillis >= 1000)
   {
    counterLastMillis = currentMillis;
    return true;
   }
return false;
}

////////////////////////////////////////////////////////////////////////////////////////
/////                               PWM Function                                   /////
////////////////////////////////////////////////////////////////////////////////////////
byte pwm(byte pwmPin, byte pwmStatus, byte pwmHOn, byte pwmMOn, byte pwmSOn, byte pwmHOff, byte pwmMOff, byte pwmSOff, byte pwmMin, byte pwmMax, byte pwmSr, byte pwmSs, byte pwmKeepLight, boolean pwmInvert)
{
  byte low=0, high=255;

  if (pwmInvert) { low = 255; high = 0;}
  if (!pwmInvert) { low = 0; high = 255;}
 
  if ( pwmStatus == 0 ) { analogWrite(pwmPin,map(0,0,255,low,high)); return map(0,0,255,low,high); }
 
 
  long pwmOn = (long(pwmHOn)*3600) + (long(pwmMOn)*60) + long(pwmSOn);
  long pwmOff  = (long(pwmHOff)*3600) + (long(pwmMOff)*60) + long(pwmSOff);
 
  boolean state = false;
   
  if (pwmOn < pwmOff) {         // Normalnie 
      if (currentTimeSec >= pwmOn && currentTimeSec < pwmOff) { state = true; } 
  } 
 
  if (pwmOn > pwmOff) {         // Zasuwaj dookola
      if (currentTimeSec >= pwmOn && currentTimeSec <= 86400) { state = true; }
      if (currentTimeSec >= 0 && currentTimeSec < pwmOff) { state = true; }
  }

  if (!state && !pwmKeepLight ) { analogWrite(pwmPin,map(0,0,255,low,high)); return map(0,0,255,low,high); }
  if (!state && pwmKeepLight )  {   if (pwmMin > 1 && pwmMin <3) {pwmMin = 1;} analogWrite(pwmPin,map(pwmMin,0,255,low,high)); return map(pwmMin,0,255,low,high); }
 
////////////////////////////////////
////  Swity
///////////////////////////////////
   float jump;
   float sunriseLenght = pwmSr*60;
   long elapsed;
   long pwmSunriseStop = pwmOn + sunriseLenght;
   long pwm;
   if (sunriseLenght == 0 ) { sunriseLenght = 1;}
   jump = ((pwmMax-pwmMin)/sunriseLenght);
 
 if ( pwmSunriseStop < 86399 && currentTimeSec >= pwmOn &&  currentTimeSec <= pwmSunriseStop ) {        
   
   elapsed = sunriseLenght-(sunriseLenght-(currentTimeSec-pwmOn));
   pwm = pwmMin+(elapsed*jump);
   analogWrite(pwmPin,map(pwm,0,255,low,high));
   return map(pwm,0,255,low,high);          
  }
 
   if (( pwmSunriseStop > 86399 && currentTimeSec >= pwmOn &&  currentTimeSec <= 86399 ) // przez zero
      || pwmSunriseStop > 86399 && currentTimeSec >= 0 && (currentTimeSec < (pwmSunriseStop - 86399)))
   {
    if (currentTimeSec>=pwmOn && currentTimeSec<=86399)
     {
       elapsed = (sunriseLenght-(sunriseLenght-(currentTimeSec-pwmOn)));
     }
    else
     {
      elapsed = sunriseLenght-(sunriseLenght -(86399-pwmOn+currentTimeSec));
     }
  
   pwm = pwmMin+(elapsed*jump);
   analogWrite(pwmPin,map(pwm,0,255,low,high));
   return map(pwm,0,255,low,high);
  }
 
 float sunsetLenght = pwmSs*60;
 float sunsetStart;
 
 
 if ( (pwmOff - sunsetLenght) >= 0 )
 {
  sunsetStart = pwmOff - sunsetLenght; 
 }
 else
 {
  sunsetStart = 86400 + (pwmOff - sunsetLenght);
 }
 

   if (sunsetStart < pwmOff && sunsetStart >= 0 && currentTimeSec >= sunsetStart && currentTimeSec <= pwmOff)
     { // Normalnie
       elapsed = (currentTimeSec - sunsetStart);
       if (sunsetLenght == 0 )  { sunsetLenght = 1; }
       jump = (pwmMax - pwmMin) / sunsetLenght;
       pwm = pwmMax - ( elapsed * jump );
       analogWrite(pwmPin,map(pwm,0,255,low,high));
       return map(pwm,0,255,low,high);
     }
    
 
    if (sunsetStart > pwmOff && currentTimeSec >= sunsetStart && currentTimeSec <= 86400)
    {
       elapsed = (currentTimeSec - sunsetStart);
       if (sunsetLenght == 0 ) { sunsetLenght = 1;}
       jump = (pwmMax - pwmMin) / sunsetLenght;
       pwm = pwmMax - ( elapsed * jump );
       analogWrite(pwmPin,map(pwm,0,255,low,high));
       return map(pwm,0,255,low,high);
    }

    if (sunsetStart > pwmOff && currentTimeSec >= 0 && currentTimeSec < pwmOff)
   {
       elapsed = (86400-sunsetStart)+currentTimeSec;
       if (sunsetLenght == 0 ) { sunsetLenght = 1;}
       jump = ( pwmMax - pwmMin) / sunsetLenght;
       pwm = pwmMax-(elapsed * jump);
       analogWrite(pwmPin,map(pwm,0,255,low,high));
       return map(pwm,0,255,low,high);
   }
  
 
 //// Jak sie nic nie zlapalo, to wypal oczy
   analogWrite(pwmPin,map(pwmMax,0,255,low,high));
   return map(pwmMax,0,255,low,high);
}
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
/////                               checkPwmValue                                  /////
////////////////////////////////////////////////////////////////////////////////////////
byte checkPwmValue(byte pwmPin, byte pwmStatus, byte pwmHOn, byte pwmMOn, byte pwmSOn, byte pwmHOff, byte pwmMOff, byte pwmSOff, byte pwmMin, byte pwmMax, byte pwmSr, byte pwmSs, byte pwmKeepLight, boolean pwmInvert)
{
  byte low=0, high=255;

  if (pwmInvert) { low = 255; high = 0;}
  if (!pwmInvert) { low = 0; high = 255;}
 
  if ( pwmStatus == 0 ) { return map(0,0,255,low,high); }
 
 
  long pwmOn = (long(pwmHOn)*3600) + (long(pwmMOn)*60) + long(pwmSOn);
  long pwmOff  = (long(pwmHOff)*3600) + (long(pwmMOff)*60) + long(pwmSOff);
 
  boolean state = false;
   
  if (pwmOn < pwmOff) {         // Normalnie 
      if (currentTimeSec >= pwmOn && currentTimeSec < pwmOff) { state = true; } 
  } 
 
  if (pwmOn > pwmOff) {         // Zasuwaj dookola
      if (currentTimeSec >= pwmOn && currentTimeSec <= 86400) { state = true; }
      if (currentTimeSec >= 0 && currentTimeSec < pwmOff) { state = true; }
  }

  if (!state && !pwmKeepLight ) { return map(0,0,255,low,high); }
  if (!state && pwmKeepLight )  {   if (pwmMin > 1 && pwmMin <3) {pwmMin = 1;} return map(pwmMin,0,255,low,high); }

   float jump;
   float sunriseLenght = pwmSr*60;
   long elapsed;
   long pwmSunriseStop = pwmOn + sunriseLenght;
   long pwm;
   if (sunriseLenght == 0 ) { sunriseLenght = 1;}
   jump = ((pwmMax-pwmMin)/sunriseLenght);
 
 if ( pwmSunriseStop < 86399 && currentTimeSec >= pwmOn &&  currentTimeSec <= pwmSunriseStop ) {        
   
   elapsed = sunriseLenght-(sunriseLenght-(currentTimeSec-pwmOn));
   pwm = pwmMin+(elapsed*jump);
   return map(pwm,0,255,low,high);          
  }
 
   if (( pwmSunriseStop > 86399 && currentTimeSec >= pwmOn &&  currentTimeSec <= 86399 ) // przez zero
      || pwmSunriseStop > 86399 && currentTimeSec >= 0 && (currentTimeSec < (pwmSunriseStop - 86399)))
   {
    if (currentTimeSec>=pwmOn && currentTimeSec<=86399)
     {
       elapsed = (sunriseLenght-(sunriseLenght-(currentTimeSec-pwmOn)));
     }
    else
     {
      elapsed = sunriseLenght-(sunriseLenght -(86399-pwmOn+currentTimeSec));
     }
  
   pwm = pwmMin+(elapsed*jump);
   return map(pwm,0,255,low,high);
  }
 
 float sunsetLenght = pwmSs*60;
 float sunsetStart;
 
 
 if ( (pwmOff - sunsetLenght) >= 0 )
 {
  sunsetStart = pwmOff - sunsetLenght; 
 }
 else
 {
  sunsetStart = 86400 + (pwmOff - sunsetLenght);
 }
    if (sunsetStart < pwmOff && sunsetStart >= 0 && currentTimeSec >= sunsetStart && currentTimeSec <= pwmOff)
     { 
       elapsed = (currentTimeSec - sunsetStart);
       if (sunsetLenght == 0 )  { sunsetLenght = 1; }
       jump = (pwmMax - pwmMin) / sunsetLenght;
       pwm = pwmMax - ( elapsed * jump );
       return map(pwm,0,255,low,high);
     }
    
 
    if (sunsetStart > pwmOff && currentTimeSec >= sunsetStart && currentTimeSec <= 86400)
    {
       elapsed = (currentTimeSec - sunsetStart);
       if (sunsetLenght == 0 ) { sunsetLenght = 1;}
       jump = (pwmMax - pwmMin) / sunsetLenght;
       pwm = pwmMax - ( elapsed * jump );
       return map(pwm,0,255,low,high);
    }

    if (sunsetStart > pwmOff && currentTimeSec >= 0 && currentTimeSec < pwmOff)
   {
       elapsed = (86400-sunsetStart)+currentTimeSec;
       if (sunsetLenght == 0 ) { sunsetLenght = 1;}
       jump = ( pwmMax - pwmMin) / sunsetLenght;
       pwm = pwmMax-(elapsed * jump);
       return map(pwm,0,255,low,high);
   }
   return map(pwmMax,0,255,low,high);
}
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
/////                      keepLighCheck Function                                 /////
///////////////////////////////////////////////////////////////////////////////////////

boolean keepLighCheck( byte klStatus, byte hOn, byte mOn, byte hOff, byte mOff) 
{
  if ( klStatus == 0 ) { return false; }
  
  boolean state = false;
  long klOn = ((long(hOn)*3600)+(long(mOn)*60));  
  long klOff = ((long(hOff)*3600)+(long(mOff)*60));
  
  
  if (klOn < klOff) {           
      if (currentTimeSec >= klOn && currentTimeSec < klOff) { state = true; }  
  }  
  
  if (klOn > klOff) {         
      if (currentTimeSec >= klOn && currentTimeSec <= 86399) { state = true; }
      if (currentTimeSec >= 0 && currentTimeSec < klOff) { state = true; }
  }
  
  if (state) { return true; } else { return false; }
}


///////////////////////////////////////////////////////////////////////////////////////
/////                      moonlightOff Function                                  /////
///////////////////////////////////////////////////////////////////////////////////////

boolean moonlightOff() 
{
  pwm1KeepLight = 0;
  pwm2KeepLight = 0;
  pwm3KeepLight = 0;
  pwm4KeepLight = 0;
  pwm5KeepLight = 0;
  pwm6KeepLight = 0;
  return true;
}


///////////////////////////////////////////////////////////////////////////////////////
/////                      channelTest Function                                   /////
///////////////////////////////////////////////////////////////////////////////////////

void channelTest(byte pwmPin, byte pwmValue, boolean pwmInvert) 
{
  byte low=0, high=255;
    //if (millis()%1000 == 0) { Serial.print("pwmValue: "); Serial.println(pwmValue); }
    pwmValue = map(pwmValue,0,100,0,255);
  
  //if (millis()%1000 == 0) { Serial.println(pwmValue); }
  
  if (pwmInvert) { low = 255; high = 0;}
  if (!pwmInvert) { low = 0; high = 255;}
 
  analogWrite( pwmPin,map(pwmValue,0,255,low,high) ); 
  //analogWrite(pwmPin,pwmValue);
    
}

///////////////////////////////////////////////////////////////////////////////////////
/////                      turnOnFan                                              /////
///////////////////////////////////////////////////////////////////////////////////////

boolean turnOnFan(byte fanPin, byte pwmValue, boolean invertChannel) 
{
  //if ( millis()%2000 == 0) { Serial.println(pwmValue); }
  if ( (!invertChannel) && (pwmValue >= fanTreshold) ) { digitalWrite(fanPin,ON); }
  if ( (!invertChannel) && (pwmValue < fanTreshold) ) { digitalWrite(fanPin,OFF); }

  if ( (invertChannel) && (pwmValue <= (255-fanTreshold) ) ) { digitalWrite(fanPin,ON); }
  if ( (invertChannel) && (pwmValue > (255-fanTreshold) ) ) { digitalWrite(fanPin,OFF); }
}
///////////////////////////////////////////////////////////////////////////
/////                       eEpromRead                                /////
///////////////////////////////////////////////////////////////////////////

void eEpromRead() {
               // Serial.println("666, eEpromRead!");
             // PWM - 1
                pwm1Status = EEPROM.read(282);
                pwm1HOn = EEPROM.read(283); pwm1MOn = EEPROM.read(284);
                pwm1SOn = EEPROM.read(285); pwm1HOff = EEPROM.read(286);
                pwm1MOff = EEPROM.read(287); pwm1SOff = EEPROM.read(288);
                pwm1Min = map(EEPROM.read(289),0,100,0,255); pwm1Max = map(EEPROM.read(290),0,100,0,255);
                pwm1Sr = EEPROM.read(291); pwm1Ss = EEPROM.read(292);     
                pwm1KeepLight = EEPROM.read(293);

              // PWM - 2
                pwm2Status = EEPROM.read(302);
                pwm2HOn = EEPROM.read(303); pwm2MOn = EEPROM.read(304);
                pwm2SOn = EEPROM.read(305); pwm2HOff = EEPROM.read(306);
                pwm2MOff = EEPROM.read(307); pwm2SOff = EEPROM.read(308);
                pwm2Min = map(EEPROM.read(309),0,100,0,255); pwm2Max = map(EEPROM.read(310),0,100,0,255);
                pwm2Sr = EEPROM.read(311); pwm2Ss = EEPROM.read(312);     
                pwm2KeepLight = EEPROM.read(313);
               
              // PWM - 3
                pwm3Status = EEPROM.read(332);
                pwm3HOn = EEPROM.read(333); pwm3MOn = EEPROM.read(334);
                pwm3SOn = EEPROM.read(335); pwm3HOff = EEPROM.read(336);
                pwm3MOff = EEPROM.read(337); pwm3SOff = EEPROM.read(338);
                pwm3Min = map(EEPROM.read(339),0,100,0,255); pwm3Max = map(EEPROM.read(340),0,100,0,255);
                pwm3Sr = EEPROM.read(341); pwm3Ss = EEPROM.read(342);     
                pwm3KeepLight = EEPROM.read(343);

              // PWM - 4
                pwm4Status = EEPROM.read(372);
                pwm4HOn = EEPROM.read(373); pwm4MOn = EEPROM.read(374);
                pwm4SOn = EEPROM.read(375); pwm4HOff = EEPROM.read(376);
                pwm4MOff = EEPROM.read(377); pwm4SOff = EEPROM.read(378);
                pwm4Min = map(EEPROM.read(379),0,100,0,255); pwm4Max = map(EEPROM.read(380),0,100,0,255);
                pwm4Sr = EEPROM.read(381); pwm4Ss = EEPROM.read(382);     
                pwm4KeepLight = EEPROM.read(383);

              // PWM - 5
                pwm5Status = EEPROM.read(392);
                pwm5HOn = EEPROM.read(393); pwm5MOn = EEPROM.read(394);
                pwm5SOn = EEPROM.read(395); pwm5HOff = EEPROM.read(396);
                pwm5MOff = EEPROM.read(397); pwm5SOff = EEPROM.read(398);
                pwm5Min = map(EEPROM.read(399),0,100,0,255); pwm5Max = map(EEPROM.read(400),0,100,0,255);
                pwm5Sr = EEPROM.read(401); pwm5Ss = EEPROM.read(402);     
                pwm5KeepLight = EEPROM.read(403);
        
              // PWM - 6
                pwm6Status = EEPROM.read(412);
                pwm6HOn = EEPROM.read(413); pwm6MOn = EEPROM.read(414);
                pwm6SOn = EEPROM.read(415); pwm6HOff = EEPROM.read(416);
                pwm6MOff = EEPROM.read(417); pwm6SOff = EEPROM.read(418);
                pwm6Min = map(EEPROM.read(419),0,100,0,255); pwm6Max = map(EEPROM.read(420),0,100,0,255);
                pwm6Sr = EEPROM.read(421); pwm6Ss = EEPROM.read(422);     
                pwm6KeepLight = EEPROM.read(423);

              // KL
                klState = EEPROM.read(432); kl1HOn = EEPROM.read(433); kl1MOn = EEPROM.read(434); kl1HOff = EEPROM.read(435);
                kl1MOff = EEPROM.read(436); kl2HOn = EEPROM.read(437); kl2MOn = EEPROM.read(438); kl2HOff = EEPROM.read(439);
                kl2MOff = EEPROM.read(440); 
               /* Serial.println(klState);
                Serial.println(kl1HOn);
                Serial.println(kl1MOn);
                Serial.println(kl1HOff);
                Serial.println(kl1MOff);
                Serial.println(kl2HOn);
                Serial.println(kl2MOn);
                Serial.println(kl2HOff);
                Serial.println(kl2MOff);
                */
                

                
}



////////////////////////////////////////////////////////////////////////////////////////
/////                             Analysis                                         /////
////////////////////////////////////////////////////////////////////////////////////////

 boolean commandAnalysis(char cmdOutputArray[64]){

  unsigned int val[64];
  char *cmdVal;
  int i = 0;
 
  cmdVal = strtok(cmdOutputArray, ",");
 
      while (cmdVal)
      {
       val[i++] = atoi(cmdVal);
       cmdVal = strtok(NULL, ",");
      }
 
  beep = true;
  manualMode = false;
  eEpromRead(); 

 ///////////////////////////////////////////////////////////////////////////////////////
 /////                        PWM - 1, EPROM                                       /////
 ///////////////////////////////////////////////////////////////////////////////////////
  if (val[0] == 31)
  { //      1          2         3            4        5          6          7         8        9          10        11       12         13
    byte ePwm1Pin, ePwm1Status, ePwm1HOn, ePwm1MOn, ePwm1SOn, ePwm1HOff, ePwm1MOff, ePwm1SOff, ePwm1Min, ePwm1Max, ePwm1Sr, ePwm1Ss, ePwm1KeepLight;
        
      if (val[1] >=0 && val[1] <= 99) { ePwm1Pin = val[1]; } else { return false;}             // ePwm1Pin -       1
      if (val[2] >=0 && val[2] <= 1)  { ePwm1Status = val[2]; } else { return false;}          // ePwm1Status -    2
      if (val[3] >=0 && val[3] <= 23) { ePwm1HOn = val[3]; } else { return false;}             // ePwm1HOn -       3
      if (val[4] >=0 && val[4] <= 59) { ePwm1MOn = val[4]; } else { return false;}             // ePwm1MOn -       4
      if (val[5] >=0 && val[5] <= 59) { ePwm1SOn = val[5]; } else { return false;}             // ePwm1SOn -       5
      if (val[6] >=0 && val[6] <= 23) { ePwm1HOff = val[6]; } else { return false;}            // ePwm1HOff -      6
      if (val[7] >=0 && val[7] <= 59) { ePwm1MOff = val[7]; } else { return false;}            // ePwm1MOff -      7
      if (val[8] >=0 && val[8] <= 59) { ePwm1SOff = val[8]; } else { return false;}            // ePwm1SOff -      8
      if (val[9] >=0 && val[9] <= 255) { ePwm1Min = val[9]; } else { return false;}            // ePwm1Min -       9
      if (val[10] >=0 && val[10] <= 255) { ePwm1Max = val[10]; } else { return false;}         // ePwm1Max -       10
      if (val[11] >=0 && val[11] <= 255) { ePwm1Sr = val[11]; } else { return false;}          // ePwm1Sr -        11
      if (val[12] >=0 && val[12] <= 255) { ePwm1Ss = val[12]; } else { return false;}          // ePwm1Ss -        12
      if (val[13] >=0 && val[13] <= 1)   { ePwm1KeepLight = val[13]; } else { return false;}   // ePwm1KeepLight - 13
     
     EEPROM.update(281,ePwm1Pin); EEPROM.update(282,ePwm1Status); EEPROM.update(283,ePwm1HOn); EEPROM.update(284,ePwm1MOn);
     EEPROM.update(285,ePwm1SOn); EEPROM.update(286,ePwm1HOff); EEPROM.update(287,ePwm1MOff); EEPROM.update(288,ePwm1SOff);
     EEPROM.update(289,ePwm1Min); EEPROM.update(290,ePwm1Max); EEPROM.update(291,ePwm1Sr); EEPROM.update(292,ePwm1Ss);     
     EEPROM.update(293,ePwm1KeepLight);

  } 


 ///////////////////////////////////////////////////////////////////////////////////////
 /////                        PWM - 2, EPROM                                       /////
 ///////////////////////////////////////////////////////////////////////////////////////
  if (val[0] == 32)
  { //      1          2         3            4        5          6          7         8        9          10        11       12         13
    byte ePwm2Pin, ePwm2Status, ePwm2HOn, ePwm2MOn, ePwm2SOn, ePwm2HOff, ePwm2MOff, ePwm2SOff, ePwm2Min, ePwm2Max, ePwm2Sr, ePwm2Ss, ePwm2KeepLight;
     
      if (val[1] >=0 && val[1] <= 99) { ePwm2Pin = val[1]; } else { return false;}             // ePwm2Pin -       1
      if (val[2] >=0 && val[2] <= 1)  { ePwm2Status = val[2]; } else { return false;}          // ePwm2Status -    2
      if (val[3] >=0 && val[3] <= 23) { ePwm2HOn = val[3]; } else { return false;}             // ePwm2HOn -       3
      if (val[4] >=0 && val[4] <= 59) { ePwm2MOn = val[4]; } else { return false;}             // ePwm2MOn -       4
      if (val[5] >=0 && val[5] <= 59) { ePwm2SOn = val[5]; } else { return false;}             // ePwm2SOn -       5
      if (val[6] >=0 && val[6] <= 23) { ePwm2HOff = val[6]; } else { return false;}            // ePwm2HOff -      6
      if (val[7] >=0 && val[7] <= 59) { ePwm2MOff = val[7]; } else { return false;}            // ePwm2MOff -      7
      if (val[8] >=0 && val[8] <= 59) { ePwm2SOff = val[8]; } else { return false;}            // ePwm2SOff -      8
      if (val[9] >=0 && val[9] <= 255) { ePwm2Min = val[9]; } else { return false;}            // ePwm2Min -       9
      if (val[10] >=0 && val[10] <= 255) { ePwm2Max = val[10]; } else { return false;}         // ePwm2Max -       10
      if (val[11] >=0 && val[11] <= 255) { ePwm2Sr = val[11]; } else { return false;}          // ePwm2Sr -        11
      if (val[12] >=0 && val[12] <= 255) { ePwm2Ss = val[12]; } else { return false;}          // ePwm2Ss -        12
      if (val[13] >=0 && val[13] <= 1)   { ePwm2KeepLight = val[13]; } else { return false;}   // ePwm2KeepLight - 13
     
     EEPROM.update(301,ePwm2Pin); EEPROM.update(302,ePwm2Status); EEPROM.update(303,ePwm2HOn); EEPROM.update(304,ePwm2MOn);
     EEPROM.update(305,ePwm2SOn); EEPROM.update(306,ePwm2HOff); EEPROM.update(307,ePwm2MOff); EEPROM.update(308,ePwm2SOff);
     EEPROM.update(309,ePwm2Min); EEPROM.update(310,ePwm2Max); EEPROM.update(311,ePwm2Sr); EEPROM.update(312,ePwm2Ss);     
     EEPROM.update(313,ePwm2KeepLight);
  } 
 
 ///////////////////////////////////////////////////////////////////////////////////////
 /////                        PWM - 3, EPROM                                       /////
 ///////////////////////////////////////////////////////////////////////////////////////
  if (val[0] == 33)
  { //      1          2         3            4        5          6          7         8        9          10        11       12         13
    byte ePwm3Pin, ePwm3Status, ePwm3HOn, ePwm3MOn, ePwm3SOn, ePwm3HOff, ePwm3MOff, ePwm3SOff, ePwm3Min, ePwm3Max, ePwm3Sr, ePwm3Ss, ePwm3KeepLight;
     
      if (val[1] >=0 && val[1] <= 99) { ePwm3Pin = val[1]; } else { return false;}             // ePwm3Pin -       1
      if (val[2] >=0 && val[2] <= 1)  { ePwm3Status = val[2]; } else { return false;}          // ePwm3Status -    2
      if (val[3] >=0 && val[3] <= 23) { ePwm3HOn = val[3]; } else { return false;}             // ePwm3HOn -       3
      if (val[4] >=0 && val[4] <= 59) { ePwm3MOn = val[4]; } else { return false;}             // ePwm3MOn -       4
      if (val[5] >=0 && val[5] <= 59) { ePwm3SOn = val[5]; } else { return false;}             // ePwm3SOn -       5
      if (val[6] >=0 && val[6] <= 23) { ePwm3HOff = val[6]; } else { return false;}            // ePwm3HOff -      6
      if (val[7] >=0 && val[7] <= 59) { ePwm3MOff = val[7]; } else { return false;}            // ePwm3MOff -      7
      if (val[8] >=0 && val[8] <= 59) { ePwm3SOff = val[8]; } else { return false;}            // ePwm3SOff -      8
      if (val[9] >=0 && val[9] <= 255) { ePwm3Min = val[9]; } else { return false;}            // ePwm3Min -       9
      if (val[10] >=0 && val[10] <= 255) { ePwm3Max = val[10]; } else { return false;}         // ePwm3Max -       10
      if (val[11] >=0 && val[11] <= 255) { ePwm3Sr = val[11]; } else { return false;}          // ePwm3Sr -        11
      if (val[12] >=0 && val[12] <= 255) { ePwm3Ss = val[12]; } else { return false;}          // ePwm3Ss -        12
      if (val[13] >=0 && val[13] <= 1)   { ePwm3KeepLight = val[13]; } else { return false;}   // ePwm3KeepLight - 13

     
     EEPROM.update(331,ePwm3Pin); EEPROM.update(332,ePwm3Status); EEPROM.update(333,ePwm3HOn); EEPROM.update(334,ePwm3MOn);
     EEPROM.update(335,ePwm3SOn); EEPROM.update(336,ePwm3HOff); EEPROM.update(337,ePwm3MOff); EEPROM.update(338,ePwm3SOff);
     EEPROM.update(339,ePwm3Min); EEPROM.update(340,ePwm3Max); EEPROM.update(341,ePwm3Sr); EEPROM.update(342,ePwm3Ss);     
     EEPROM.update(343,ePwm3KeepLight);
  } 

  ///////////////////////////////////////////////////////////////////////////////////////
 /////                        PWM - 4, EPROM                                       /////
 ///////////////////////////////////////////////////////////////////////////////////////
  if (val[0] == 90)
  { //      1          2         3            4        5          6          7         8        9          10        11       12         13
    byte ePwm4Pin, ePwm4Status, ePwm4HOn, ePwm4MOn, ePwm4SOn, ePwm4HOff, ePwm4MOff, ePwm4SOff, ePwm4Min, ePwm4Max, ePwm4Sr, ePwm4Ss, ePwm4KeepLight;
     
      if (val[1] >=0 && val[1] <= 99) { ePwm4Pin = val[1]; } else { return false;}             // ePwm4Pin -       1
      if (val[2] >=0 && val[2] <= 1)  { ePwm4Status = val[2]; } else { return false;}          // ePwm4Status -    2
      if (val[3] >=0 && val[3] <= 23) { ePwm4HOn = val[3]; } else { return false;}             // ePwm4HOn -       3
      if (val[4] >=0 && val[4] <= 59) { ePwm4MOn = val[4]; } else { return false;}             // ePwm4MOn -       4
      if (val[5] >=0 && val[5] <= 59) { ePwm4SOn = val[5]; } else { return false;}             // ePwm4SOn -       5
      if (val[6] >=0 && val[6] <= 23) { ePwm4HOff = val[6]; } else { return false;}            // ePwm4HOff -      6
      if (val[7] >=0 && val[7] <= 59) { ePwm4MOff = val[7]; } else { return false;}            // ePwm4MOff -      7
      if (val[8] >=0 && val[8] <= 59) { ePwm4SOff = val[8]; } else { return false;}            // ePwm4SOff -      8
      if (val[9] >=0 && val[9] <= 255) { ePwm4Min = val[9]; } else { return false;}            // ePwm4Min -       9
      if (val[10] >=0 && val[10] <= 255) { ePwm4Max = val[10]; } else { return false;}         // ePwm4Max -       10
      if (val[11] >=0 && val[11] <= 255) { ePwm4Sr = val[11]; } else { return false;}          // ePwm4Sr -        11
      if (val[12] >=0 && val[12] <= 255) { ePwm4Ss = val[12]; } else { return false;}          // ePwm4Ss -        12
      if (val[13] >=0 && val[13] <= 1)   { ePwm4KeepLight = val[13]; } else { return false;}   // ePwm4KeepLight - 13
     
     EEPROM.update(371,ePwm4Pin); EEPROM.update(372,ePwm4Status); EEPROM.update(373,ePwm4HOn); EEPROM.update(374,ePwm4MOn);
     EEPROM.update(375,ePwm4SOn); EEPROM.update(376,ePwm4HOff); EEPROM.update(377,ePwm4MOff); EEPROM.update(378,ePwm4SOff);
     EEPROM.update(379,ePwm4Min); EEPROM.update(380,ePwm4Max); EEPROM.update(381,ePwm4Sr); EEPROM.update(382,ePwm4Ss);     
     EEPROM.update(383,ePwm4KeepLight);
  } 

  ///////////////////////////////////////////////////////////////////////////////////////
 /////                        PWM - 5, EPROM                                       /////
 ///////////////////////////////////////////////////////////////////////////////////////
  if (val[0] == 91)
  { //      1          2         3            4        5          6          7         8        9          10        11       12         13
    byte ePwm5Pin, ePwm5Status, ePwm5HOn, ePwm5MOn, ePwm5SOn, ePwm5HOff, ePwm5MOff, ePwm5SOff, ePwm5Min, ePwm5Max, ePwm5Sr, ePwm5Ss, ePwm5KeepLight;
     
      if (val[1] >=0 && val[1] <= 99) { ePwm5Pin = val[1]; } else { return false;}             // ePwm5Pin -       1
      if (val[2] >=0 && val[2] <= 1)  { ePwm5Status = val[2]; } else { return false;}          // ePwm5Status -    2
      if (val[3] >=0 && val[3] <= 23) { ePwm5HOn = val[3]; } else { return false;}             // ePwm5HOn -       3
      if (val[4] >=0 && val[4] <= 59) { ePwm5MOn = val[4]; } else { return false;}             // ePwm5MOn -       4
      if (val[5] >=0 && val[5] <= 59) { ePwm5SOn = val[5]; } else { return false;}             // ePwm5SOn -       5
      if (val[6] >=0 && val[6] <= 23) { ePwm5HOff = val[6]; } else { return false;}            // ePwm5HOff -      6
      if (val[7] >=0 && val[7] <= 59) { ePwm5MOff = val[7]; } else { return false;}            // ePwm5MOff -      7
      if (val[8] >=0 && val[8] <= 59) { ePwm5SOff = val[8]; } else { return false;}            // ePwm5SOff -      8
      if (val[9] >=0 && val[9] <= 255) { ePwm5Min = val[9]; } else { return false;}            // ePwm5Min -       9
      if (val[10] >=0 && val[10] <= 255) { ePwm5Max = val[10]; } else { return false;}         // ePwm5Max -       10
      if (val[11] >=0 && val[11] <= 255) { ePwm5Sr = val[11]; } else { return false;}          // ePwm5Sr -        11
      if (val[12] >=0 && val[12] <= 255) { ePwm5Ss = val[12]; } else { return false;}          // ePwm5Ss -        12
      if (val[13] >=0 && val[13] <= 1)   { ePwm5KeepLight = val[13]; } else { return false;}   // ePwm5KeepLight - 13
     
     EEPROM.update(391,ePwm5Pin); EEPROM.update(392,ePwm5Status); EEPROM.update(393,ePwm5HOn); EEPROM.update(394,ePwm5MOn);
     EEPROM.update(395,ePwm5SOn); EEPROM.update(396,ePwm5HOff); EEPROM.update(397,ePwm5MOff); EEPROM.update(398,ePwm5SOff);
     EEPROM.update(399,ePwm5Min); EEPROM.update(400,ePwm5Max); EEPROM.update(401,ePwm5Sr); EEPROM.update(402,ePwm5Ss);     
     EEPROM.update(403,ePwm5KeepLight);
  }


///////////////////////////////////////////////////////////////////////////////////////
 /////                        PWM - 6, EPROM                                       /////
 ///////////////////////////////////////////////////////////////////////////////////////
  if (val[0] == 92)
  {  //      1          2         3            4        5          6          7         8        9          10        11       12         13
    byte ePwm6Pin, ePwm6Status, ePwm6HOn, ePwm6MOn, ePwm6SOn, ePwm6HOff, ePwm6MOff, ePwm6SOff, ePwm6Min, ePwm6Max, ePwm6Sr, ePwm6Ss, ePwm6KeepLight;
        
      if (val[1] >=0 && val[1] <= 99) { ePwm6Pin = val[1]; } else { return false;}             // ePwm6Pin -       1
      if (val[2] >=0 && val[2] <= 1)  { ePwm6Status = val[2]; } else { return false;}          // ePwm6Status -    2
      if (val[3] >=0 && val[3] <= 23) { ePwm6HOn = val[3]; } else { return false;}             // ePwm6HOn -       3
      if (val[4] >=0 && val[4] <= 59) { ePwm6MOn = val[4]; } else { return false;}             // ePwm6MOn -       4
      if (val[5] >=0 && val[5] <= 59) { ePwm6SOn = val[5]; } else { return false;}             // ePwm6SOn -       5
      if (val[6] >=0 && val[6] <= 23) { ePwm6HOff = val[6]; } else { return false;}            // ePwm6HOff -      6
      if (val[7] >=0 && val[7] <= 59) { ePwm6MOff = val[7]; } else { return false;}            // ePwm6MOff -      7
      if (val[8] >=0 && val[8] <= 59) { ePwm6SOff = val[8]; } else { return false;}            // ePwm6SOff -      8
      if (val[9] >=0 && val[9] <= 255) { ePwm6Min = val[9]; } else { return false;}            // ePwm6Min -       9
      if (val[10] >=0 && val[10] <= 255) { ePwm6Max = val[10]; } else { return false;}         // ePwm6Max -       10
      if (val[11] >=0 && val[11] <= 255) { ePwm6Sr = val[11]; } else { return false;}          // ePwm6Sr -        11
      if (val[12] >=0 && val[12] <= 255) { ePwm6Ss = val[12]; } else { return false;}          // ePwm6Ss -        12
      if (val[13] >=0 && val[13] <= 1)   { ePwm6KeepLight = val[13]; } else { return false;}   // ePwm6KeepLight - 13

     EEPROM.update(411,ePwm6Pin); EEPROM.update(412,ePwm6Status); EEPROM.update(413,ePwm6HOn); EEPROM.update(414,ePwm6MOn);
     EEPROM.update(415,ePwm6SOn); EEPROM.update(416,ePwm6HOff); EEPROM.update(417,ePwm6MOff); EEPROM.update(418,ePwm6SOff);
     EEPROM.update(419,ePwm6Min); EEPROM.update(420,ePwm6Max); EEPROM.update(421,ePwm6Sr); EEPROM.update(422,ePwm6Ss);     
     EEPROM.update(423,ePwm6KeepLight);

  }
 // Keep Light - Config
  if (val[0] == 96)
  {  //      1        2        3        4          5        6        7        8        9
    byte eKlState, eKl1HOn, eKl1MOn, eKl1HOff, eKl1MOff, eKl2HOn, eKl2MOn, eKl2HOff, eKl2MOff;
        
      if (val[1] >=0 && val[1] <= 1)  { eKlState = val[1]; } else { return false;}         
      if (val[2] >=0 && val[2] <= 23)  { eKl1HOn = val[2]; } else { return false;}          
      if (val[3] >=0 && val[3] <= 59) { eKl1MOn = val[3]; } else { return false;}          
      if (val[4] >=0 && val[4] <= 23) { eKl1HOff = val[4]; } else { return false;}         
      if (val[5] >=0 && val[5] <= 59) { eKl1MOff = val[5]; } else { return false;}        
      if (val[6] >=0 && val[6] <= 23) { eKl2HOn = val[6]; } else { return false;}         
      if (val[7] >=0 && val[7] <= 59) { eKl2MOn = val[7]; } else { return false;}          
      if (val[8] >=0 && val[8] <= 59) { eKl2HOff = val[8]; } else { return false;}         
      if (val[9] >=0 && val[9] <= 59) { eKl2MOff = val[9]; } else { return false;}         

     EEPROM.update(432,eKlState); EEPROM.update(433,eKl1HOn); EEPROM.update(434,eKl1MOn); EEPROM.update(435,eKl1HOff);
     EEPROM.update(436,eKl1MOff); EEPROM.update(437,eKl2HOn); EEPROM.update(438,eKl2MOn); EEPROM.update(439,eKl2HOff);
     EEPROM.update(440,eKl2MOff); 
  }


    if (val[0] == 101)
  {    
      if (val[1] >=0 && val[1] <= 1)  { testPwm1 = val[1]; } else { return false;}
      if (val[2] >=0 && val[2] <= 100)  { testPwm1Val = val[2]; } else { return false;} 
  }

 
    if (val[0] == 102)
  {    
      if (val[1] >=0 && val[1] <= 1)  { testPwm2 = val[1]; } else { return false;}
      if (val[2] >=0 && val[2] <= 100)  { testPwm2Val = val[2]; } else { return false;} 
      
  }

  
    if (val[0] == 103)
  {    
      if (val[1] >=0 && val[1] <= 1)  { testPwm3 = val[1]; } else { return false;}
      if (val[2] >=0 && val[2] <= 100)  { testPwm3Val = val[2]; } else { return false;} 
  }


    if (val[0] == 104)
  {    
      if (val[1] >=0 && val[1] <= 1)  { testPwm4 = val[1]; } else { return false;}
      if (val[2] >=0 && val[2] <= 100)  { testPwm4Val = val[2]; } else { return false;} 
  }

    if (val[0] == 105)
  {    
      if (val[1] >=0 && val[1] <= 1)  { testPwm5 = val[1]; } else { return false;}
      if (val[2] >=0 && val[2] <= 100)  { testPwm5Val = val[2]; } else { return false;} 
  }
  
    if (val[0] == 106)
  {    
      if (val[1] >=0 && val[1] <= 1)  { testPwm6 = val[1]; } else { return false;}
      if (val[2] >=0 && val[2] <= 100)  { testPwm6Val = val[2]; } else { return false;} 
  }
  
 /////////////////////////////////////////////////////////////////////////////////////////////
 /////                        Set Date                                                   /////
 /////////////////////////////////////////////////////////////////////////////////////////////
 byte setHour, setMinute, setSecond, setYear, setMonth, setDay;
 
 if (val[0] == 40) {
   if (val[1] >=0 && val[1] <= 23) { setHour = val[1]; } else { return false;}           // setHour -   1
   if (val[2] >=0 && val[2] <= 59) { setMinute = val[2]; } else { return false;}         // setMinute - 2
   if (val[3] >=0 && val[3] <= 59) { setSecond = val[3]; } else { return false;}         // setSecond - 3
   if (val[4] >=15 && val[4] <=99 ) { setYear = val[4]+2000; } else { return false;}     // setYear -   4
   if (val[5] >=1 && val[5] <=99 ) { setMonth = val[5]; } else { return false;}          // setMonth -  5
   if (val[6] >=1 && val[6] <=31 ) { setDay = val[6]; } else { return false;}            // setDay -    5
    
   setTime(setHour,setMinute,setSecond,setDay,setMonth,setYear); // godz, min, sek, dzien, miesiac, rok

   RTC.set(now());
   return true;
 }


 ///////////////////////////////////////////////////////////////////////////////////////////
 /////                       Get Settings                                              /////
 ///////////////////////////////////////////////////////////////////////////////////////////
 if (val[0] == 66 )
 {
   int eAddress = 0;
   Serial.write("*66,");
   bluetooth.write("*66,");
  
   while (eAddress <= 512)
    {
        Serial.print(EEPROM.read(eAddress));
        bluetooth.print(EEPROM.read(eAddress));
        Serial.write(",");
        bluetooth.write(",");
        eAddress++;
    }
  Serial.write("#\n");
  bluetooth.write("#\n");
 }

 ///////////////////////////////////////////////////////////////////////////////////////////
 /////                       Get Date                                                  /////
 ///////////////////////////////////////////////////////////////////////////////////////////
 if (val[0] == 83 )
 {
   Serial.print("*83");
   Serial.write(",");
   Serial.print(tm.Hour);
   Serial.write(",");
   Serial.print(tm.Minute);
   Serial.write(",");
   Serial.print(tm.Second);
   Serial.write(",");
   Serial.print(tmYearToCalendar(tm.Year));
   Serial.write(",");
   Serial.print(tm.Month);
   Serial.write(",");
   Serial.print(tm.Day);
   Serial.println();

   bluetooth.print("*83");
   bluetooth.write(",");
   bluetooth.print(tm.Hour);
   bluetooth.write(",");
   bluetooth.print(tm.Minute);
   bluetooth.write(",");
   bluetooth.print(tm.Second);
   bluetooth.write(",");
   bluetooth.print(tmYearToCalendar(tm.Year));
   bluetooth.write(",");
   bluetooth.print(tm.Month);
   bluetooth.write(",");
   bluetooth.print(tm.Day);
   bluetooth.println();
   
 }

if (val[0] == 80)
{
  beepConnect = true; manualMode = false;
  Serial.println(softVersion);
  bluetooth.println(softVersion);

}

return true; 
}

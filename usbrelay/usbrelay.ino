#include <stdlib.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

#include "TimerOne.h"
#include "D:\Development\Arduino\USB Relay\usbrelay\constants.h"

#include <i2ckeypad.h>

#define ROWS 4
#define COLS 4

// With A0, A1 and A2 of PCF8574 to ground I2C address is 0x20
#define PCF8574_ADDR 0x20


i2ckeypad kpd = i2ckeypad(PCF8574_ADDR, ROWS, COLS);

char incomingData[DATA_PROTOCOL_LENGHT];
int watchDogCnt = 0;
boolean watchdogTimerState = false;

LiquidCrystal_I2C lcd(0x3F,16,2); // LCD ekrano adresas ir matmenys   

/*****************************************************************************************/
// set device ON
/*****************************************************************************************/
void setDeviceOn(int devNr){
  digitalWrite(DEVICES_LST[devNr][0], LOW);  
  digitalWrite(DEVICES_LST[devNr][1], LOW);  
}

/*****************************************************************************************/
// set device Off
/*****************************************************************************************/
void setDeviceOff(int devNr){
  digitalWrite(DEVICES_LST[devNr][0], HIGH);  
  digitalWrite(DEVICES_LST[devNr][1], HIGH);  
}

/*****************************************************************************************/
// set all devices Off
/*****************************************************************************************/
void setAllDevicesOff(){
  for (byte i = 0; i <= MAX_SUPPORTED_DEVICES; i++){
    setDeviceOff(i);
  }
}


/*****************************************************************************************/
// return system state: pins and wdg timer
/*****************************************************************************************/
void getSystemState() {
  char sysState[6] = "";
  sysState[0] = digitalRead(PIN_REL_1)+48;
  sysState[1] = digitalRead(PIN_REL_2)+48;
  sysState[2] = digitalRead(PIN_REL_3)+48;
  sysState[3] = digitalRead(PIN_REL_4)+48;
  sysState[4] = watchdogTimerState+48;
  
  lcd.clear();
  lcd.setCursor(0,0); 
  //Spausdinamos žinutės 
  lcd.print("SystemState: "); 
  lcd.setCursor(0,1); 
  lcd.print(sysState); 
  
  //Serial.print("SystemState: ");
  //Serial.println(sysState);
  
}


/*****************************************************************************************/
// IN/OUT pins setup and first state init
/*****************************************************************************************/
void setupPins() {
  pinMode(PIN_REL_1, OUTPUT);
  pinMode(PIN_REL_2, OUTPUT);
  pinMode(PIN_REL_3, OUTPUT);
  pinMode(PIN_REL_4, OUTPUT);
  pinMode(PIN_REL_5, OUTPUT);
  pinMode(PIN_REL_6, OUTPUT);
  pinMode(PIN_REL_7, OUTPUT);
  pinMode(PIN_REL_8, OUTPUT);
  
  digitalWrite(PIN_REL_1, HIGH);
  digitalWrite(PIN_REL_2, HIGH);
  digitalWrite(PIN_REL_3, HIGH);
  digitalWrite(PIN_REL_4, HIGH);  
  digitalWrite(PIN_REL_5, HIGH);
  digitalWrite(PIN_REL_6, HIGH);
  digitalWrite(PIN_REL_7, HIGH);
  digitalWrite(PIN_REL_8, HIGH);  
}


/*****************************************************************************************/
// Enable watchdog Timer
/*****************************************************************************************/
void watchdogTimerEnable() {
  Timer1.start();
  watchDogCnt = 0;
  watchdogTimerState = true;
}


/*****************************************************************************************/
// Disable watchdog Timer
/*****************************************************************************************/
void watchdogTimerDisable() {
  Timer1.stop();
  watchDogCnt = 0;
  watchdogTimerState = false;
}


/*****************************************************************************************/
// Watchdog timer init
/*****************************************************************************************/
void setupWachdogTimer(void (*callbackProc)(), long timerPeriod, boolean startAfterInit) {
  Timer1.initialize(timerPeriod);
  Timer1.attachInterrupt(callbackProc);
  if (startAfterInit == true) {
    watchdogTimerEnable();
  } else {
    watchdogTimerDisable();
  } 
}

/*****************************************************************************************/
// watchdogTimerCallbackProc
/*****************************************************************************************/
void watchdogTimerCallbackProc(){
  ++watchDogCnt;
  if (watchDogCnt >= WATCH_DOG_TIME){
    watchDogCnt = 0;
    setAllDevicesOff();
    watchdogTimerDisable();
  }
}


/*****************************************************************************************/
// action Set Device to On
/*****************************************************************************************/
void actionSetDeviceOn(String devNr){
  lcd.clear();
  lcd.setCursor(0,0); 
  //Spausdinamos žinutės 
  lcd.print("SetDeviceOn:"); 
  lcd.setCursor(0,1); 
  lcd.print(devNr);
  /*
  Serial.print("actionSetDeviceOn: ");
  Serial.println(devNr);
  */
  setDeviceOn(atoi(devNr.c_str()));
}

/*****************************************************************************************/
// action Set Device to Off
/*****************************************************************************************/
void actionSetDeviceOff(String devNr){
  lcd.clear();
  lcd.setCursor(0,0); 
  //Spausdinamos žinutės 
  lcd.print("SetDeviceOff:"); 
  lcd.setCursor(0,1); 
  lcd.print(devNr);
  /*
  Serial.print("actionSetDeviceOn: ");
  Serial.println(devNr);
  */
  setDeviceOff(atoi(devNr.c_str()));
}



/*****************************************************************************************/
// do watchdog timer action
/*****************************************************************************************/
void actionSetWatchdogState(char action){
  lcd.clear();
  lcd.setCursor(0,0); 
  //Spausdinamos žinutės 
  lcd.print("SetWatchdogState"); 
  lcd.setCursor(0,1); 
  lcd.print(action);
  /*
  Serial.print("actionSetDeviceOn: ");
  Serial.println(devNr);
  */
  
  /*
  Serial.print("actionSetWatchdogState: ");
  Serial.println(action);
  */
  if (action == SET_WATCHDOG_ENABLE) {
    watchdogTimerEnable();
  } else {
    watchdogTimerDisable();
  }
}


/*****************************************************************************************/
// do get system state action
/*****************************************************************************************/
void actionGetSystemState(){
  //Serial.println("actionGetSystemState");
  getSystemState();
}


/*****************************************************************************************/
// do an action depending on received char
/*****************************************************************************************/
void doAction(char action[]){
  Serial.println("actionGetSystemState");
  Serial.print(action[ACTION_CODE_INDEX]);
  Serial.println(action[ACTION_ARGS_INDEX]);
  
  switch (action[ACTION_CODE_INDEX]) {
    case ACTION_SET_DEVICE_ON: 
      actionSetDeviceOn((String)action[ACTION_ARGS_INDEX]); 
      break;
    case ACTION_SET_DEVICE_OFF: 
      actionSetDeviceOff((String)action[ACTION_ARGS_INDEX]); 
      break;      
    case ACTION_SET_WATCHDOG_STATE:
      actionSetWatchdogState(action[ACTION_ARGS_INDEX]);
      break;
    case ACTION_GET_SYSTEM_STATE:
      actionGetSystemState();
      break;  
  }
}


/*****************************************************************************************/
// Setup
/*****************************************************************************************/
void setup() {
  Serial.begin(9600); 
  
  setupPins();
  setupWachdogTimer(
    watchdogTimerCallbackProc, 
    TIMER_ONE_SEC_TIME,
    false);
    
    
  // Paleidžiamas LCD ekranas 
  lcd.init();   
  //Įjungiamas apšvietimas 40
  lcd.backlight();  
  
  //Perkeliamas kursorius 
  lcd.setCursor(0,0); 
  //Spausdinamos žinutės 
  lcd.print("Welcome"); 
  //lcd.blink();
  
  
  kpd.init();
}



/*****************************************************************************************/
// mani loop
/*****************************************************************************************/
void loop() {  
  if (Serial.available() > 0) {    
    watchDogCnt = 0;    
    Serial.readBytesUntil(LF, incomingData, DATA_PROTOCOL_LENGHT);
    doAction(incomingData);
  }
  
  char key = kpd.get_key();
  if(key != '\0') {
        //Serial.print(key);
    lcd.clear();
    lcd.setCursor(0,0); 
    //Spausdinamos žinutės 
    lcd.print("Key:"); 
    lcd.setCursor(0,1); 
    lcd.print(key);
  }
}

#include <stdlib.h>

#include "TimerOne.h"
#include "D:\Development\Arduino\USB Relay\usbrelay\constants.h"


char incomingData[DATA_PROTOCOL_LENGHT];
int watchDogCnt = 0;
boolean watchdogTimerState = false;


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
  sysState[0] = digitalRead(PIN_REL_1)+48;8
  sysState[1] = digitalRead(PIN_REL_2)+48;
  sysState[2] = digitalRead(PIN_REL_3)+48;
  sysState[3] = digitalRead(PIN_REL_4)+48;
  sysState[4] = watchdogTimerState+48;
  
  Serial.print("SystemState: ");
  Serial.println(sysState);
  
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
  }
}


/*****************************************************************************************/
// action Set Device to On
/*****************************************************************************************/
void actionSetDeviceOn(String devNr){
  Serial.print("actionSetDeviceOn: ");
  Serial.println(devNr);
  setDeviceOn(atoi(devNr.c_str()));
}

/*****************************************************************************************/
// action Set Device to Off
/*****************************************************************************************/
void actionSetDeviceOff(String devNr){
  Serial.print("actionSetDeviceOff: ");
  Serial.println(devNr);
  setDeviceOff(atoi(devNr.c_str()));
}



/*****************************************************************************************/
// do watchdog timer action
/*****************************************************************************************/
void actionSetWatchdogState(char action){
  Serial.print("actionSetWatchdogState: ");
  Serial.println(action);
  
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
  Serial.println("actionGetSystemState");
  getSystemState();
}


/*****************************************************************************************/
// do an action depending on received char
/*****************************************************************************************/
void doAction(char action[]){
  
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
}

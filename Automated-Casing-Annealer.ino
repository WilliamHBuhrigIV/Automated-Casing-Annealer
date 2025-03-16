#include "display_main.h"
#include "input_main.h"
#include "state.h"
//#include <SD.h> //TODO for Backup of Settings over Powercycle

////////////////////////////////////////////////////////////////////////////////
/// Initial State Setup
////////////////////////////////////////////////////////////////////////////////

int IR_Temperature = 700, Water_Temperature = 60, Session_Total_Count = 0;
double Heating_Period = 1.0, Cooling_Period = 1.0;

state active_state = {
  REQUESTS::START,
  REQUESTS::START,
  {
    FLAGS::WAITING,
    0,
    { 1.0, 20, millis() },
    { 1.0, 20, millis() }
  },
  {
    true,
    14,
    { "Initializing...", -1, 5, 0 },
    { 10, 0, millis() }
  }
};

////////////////////////////////////////////////////////////////////////////////
/// Global Keypad Setup
////////////////////////////////////////////////////////////////////////////////


const byte numRows = 4;
const byte numCols = 4;
const byte row[numRows] = {23,25,27,29};
const byte col[numCols] = {22,24,26,28};
char hexaKeys[numRows][numCols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
Keypad* customKeypad = new Keypad(makeKeymap(hexaKeys),row,col,numRows,numCols);
InputPad* keypad = new InputPad(customKeypad);
char key = keypad->GetKeyState();

////////////////////////////////////////////////////////////////////////////////
/// Global Liquid Crystal Display Setup
////////////////////////////////////////////////////////////////////////////////

uint8_t pin_d0 = 4;
uint8_t pin_d1 = 5;
uint8_t pin_d2 = 6;
uint8_t pin_d3 = 7;
uint8_t pin_rs = 8;
uint8_t pin_en = 9;
Display* display = new Display(pin_rs, pin_en, pin_d0, pin_d1, pin_d2, pin_d3);

////////////////////////////////////////////////////////////////////////////////
/// Run Time Code
////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200,SERIAL_8E1);
  //..
}

void loop() {
  switch(active_state.request) {
    case REQUESTS::START:
      // ..
    case REQUESTS::PAUSED:
      active_state.display.active.name = "PAUSED........";
      active_state.request = REQUESTS::PAUSED;
      active_state.opperation.flag = FLAGS::WAITING;
      break;
    case REQUESTS::COMMAND:
      active_state.display.active.name = "COMMAND........";
      if (active_state.opperation.flag == FLAGS::WAITING) {
        keypad->CommandSequence(active_state);
        if (active_state.request == REQUESTS::COMMAND) {
          active_state.request = active_state.previous_request;
        }
        break;
      }
    case REQUESTS::OPERATE:
      switch(active_state.opperation.flag) {
        case FLAGS::HEATING:
          active_state.display.active.name = "HEATING........";
          break;
        case FLAGS::COOLING:
          active_state.display.active.name = "COOLING........";
          break;
        case FLAGS::WAITING:
          active_state.display.active.name = "WAITING........";
          break;
      }
      break;
    case REQUESTS::ERROR:
      active_state.display.active.name = "ERRORING........";
      active_state.opperation.flag = FLAGS::ERRORING;
      display->Error();
      while(true) {};
  }
  
  if(active_state.display.swap){
    display->DisplayMainScreen(
      active_state.display,
      "TC: ", active_state.opperation.heating.temperature_ir,
      "CP: ", active_state.opperation.cooling.period,
      "WT: ", active_state.opperation.cooling.temperature_water
    );
  }else{
    display->DisplayMainScreen(
      active_state.display,
      "TC: ", active_state.opperation.heating.temperature_ir,
      "HP: ", active_state.opperation.heating.period,
      "#C: ", active_state.opperation.count
    );
  }

  if(active_state.opperation.heating.temperature_ir > 1200){
    active_state.request = REQUESTS::ERROR;
  }
  if(active_state.opperation.cooling.temperature_water > 140){
    active_state.request = REQUESTS::ERROR;
  }
  
  key = keypad->GetKeyState();
  if (key == '#'){
    active_state.previous_request = active_state.request;
    active_state.request = REQUESTS::COMMAND;
  }
}

/*
void loop() {  
  // Turns on Heater if (HEATER IS OFF) AND (ELAPSED TIME (ms) is GREATER than COOLING PERIOD (ms))
  currentMillis = millis();
  if(!HEAT_PAUSE_STATE && !COMMAND_REQUEST && !Heat_Active && (currentMillis-endHeatMillis) > Cooling_Period*1000){
    startHeatMillis = millis();
    Active_State = String("HEATING");
    Heat_Active = true;
  }

  // REAL IR SENSOR LATER TEST HERE
  currentMillis = millis();
  if(Heat_Active){
    IR_Temperature = ((currentMillis-startHeatMillis)/(Heating_Period*1000))*1120+72;
  }else{
    IR_Temperature = 72;
  }

  // Turns off Heater if (HEATER IS ON) AND (ELAPSED TIME (ms) is GREATER than HEATING PERIOD (ms))
  currentMillis = millis();
  if((Heat_Active && (currentMillis-startHeatMillis) > Heating_Period*1000) || ERROR_STATE){
    endHeatMillis = millis();
    Active_State = String("COOLING");
    Heat_Active = false;
    Session_Total_Count += 1;
    if(COMMAND_REQUEST){
      commandSequence();
      COMMAND_REQUEST = false;
    }
  }
}
*/
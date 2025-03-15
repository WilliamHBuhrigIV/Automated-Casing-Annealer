#include "display_main.h"

/*
#include <Keypad.h>
#include <LiquidCrystal.h>
//#include <SD.h> //TODO for Backup of Settings over Powercycle

////////////////////////////////////////////////////////////////////////////////
/// Global Keypad Setup
////////////////////////////////////////////////////////////////////////////////

const byte ROWS = 4;
const byte COLS = 4;
const char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
const byte rowPins[ROWS] = {22,24,26,28};
const byte colPins[COLS] = {23,25,27,29};
const Keypad keypad = Keypad(makeKeymap(hexaKeys),rowPins,colPins,ROWS,COLS);
*/

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
/// Global Variable Setup
////////////////////////////////////////////////////////////////////////////////

/*
unsigned long startDisplayMillis;
unsigned long startDisplayInfoSwapMillis;
unsigned long startHeatMillis, endHeatMillis;
unsigned long currentMillis;
const int Display_Refresh_Period = 300;
const int Display_Info_Swap_Period = 4*1000;
*/

////////////////////////////////////////////////////////////////////////////////
/// Global Test Variable Setup
////////////////////////////////////////////////////////////////////////////////

/*
int Session_Total_Count = 0, Water_Temperature = 72, IR_Temperature = 72, Banner_Offset = 0;
double Heating_Period = 1, Cooling_Period = 1;
bool Heat_Active = false, Display_Info_Swap = false;
bool HEAT_PAUSE_STATE = true, COMMAND_REQUEST = false, ERROR_STATE = false;
String Active_State = String("PAUSED........");
*/

////////////////////////////////////////////////////////////////////////////////
/// Global Function Setup
////////////////////////////////////////////////////////////////////////////////

/*
void lcdCommandScreen(String Command_String){
  lcd.clear();
  lcd.print("COMMAND ENTRY:");
  lcd.setCursor(0, 1);
  lcd.print(Command_String);
  Serial.print("\033[0H\033[0J");
  Serial.println("COMMAND ENTRY:");
  Serial.println(Command_String);
}

void commandSequence(){
  String Command_String = String("");
  lcdCommandScreen(Command_String);
  while(true){
    char key = keypad.getKey();
    if(key){
      if (key == '#'){
        break;
      }
      if((key == '*') && (Command_String.lastIndexOf('*') == Command_String.length()-1)){
        Command_String.remove(Command_String.length()-2,2);
      }else{
        Command_String = Command_String+String(key);
      }
      lcdCommandScreen(Command_String);
    }
    
  }
  String Command_String_Prefix = String();
  String Command_String_Whole = String();
  String Command_String_Decimal = String();
  if(Command_String.indexOf('*') == -1){
    Command_String_Prefix = Command_String.substring(0,Command_String.length());
  }else{
    Command_String_Prefix = Command_String.substring(0,Command_String.indexOf('*'));
  }
  if(!(Command_String.indexOf('*') == -1)){
    if(Command_String.indexOf('*') == Command_String.lastIndexOf('*')){
      Command_String_Whole = Command_String.substring(Command_String.indexOf('*')+1,Command_String.length());
    }else{
      Command_String_Whole = Command_String.substring(Command_String.indexOf('*')+1,Command_String.lastIndexOf('*'));
      Command_String_Decimal = Command_String.substring(Command_String.lastIndexOf('*')+1,Command_String.length());
    }
  }
  char Command_String_Prefix_Char_Array[Command_String_Prefix.length()+1];
  Command_String_Prefix.toCharArray(Command_String_Prefix_Char_Array, Command_String_Prefix.length()+1);
  int Command_String_Prefix_Value = 0;
  for(int i=0;i<sizeof(Command_String_Prefix_Char_Array)+1;i++){
    Command_String_Prefix_Value += Command_String_Prefix_Char_Array[i];
  }
  Serial.println(Command_String_Prefix_Value);
  switch(Command_String_Prefix_Value){
    case 'A':
      HEAT_PAUSE_STATE = !HEAT_PAUSE_STATE;
      break;
    case 'A'+'A':
      ERROR_STATE = true;
      break;
    case 'D'+'D':
      if(Command_String_Whole.toInt()){
        Water_Temperature = Command_String_Whole.toInt();
      }
      break;
    case 'B':
      if(Command_String_Whole.toInt()){
        IR_Temperature = Command_String_Whole.toInt();
      }
      break;
    case 'C':
      if(Command_String_Decimal.toDouble() || Command_String_Whole.toInt()){
        Heating_Period = Command_String_Whole.toInt()+Command_String_Decimal.toDouble()/pow(10,Command_String_Decimal.length());
      }else{
        HEAT_PAUSE_STATE = !HEAT_PAUSE_STATE;
      }
      break;
    case 'C'+'C':
      if(Command_String_Decimal.toDouble() || Command_String_Whole.toInt()){
        Cooling_Period = Command_String_Whole.toInt()+Command_String_Decimal.toDouble()/pow(10,Command_String_Decimal.length());
      }else{
        HEAT_PAUSE_STATE = !HEAT_PAUSE_STATE;
      }
    case 'D':
      if(Command_String_Whole.toInt()){
        Session_Total_Count = Command_String_Whole.toInt();
      }else{
        Session_Total_Count = 0;
      }
      break;
  }
}
*/

////////////////////////////////////////////////////////////////////////////////
/// Run Time Code
////////////////////////////////////////////////////////////////////////////////

#include "state.h"
state active_state = {
  "Initializing...", 
  {
    true,
    14,
    { "TRYING........", -1, 6, 0 },
    { 30, 0, millis() }
  }
};

void setup() {
  Serial.begin(115200,SERIAL_8E1);
  //..
}

void loop() {
  int IR_Temperature = 700, Water_Temperature = 60, Session_Total_Count = 0;
  double Heating_Period = 1.0, Cooling_Period = 1.0;

  if(active_state.display.swap){
    display->DisplayMainScreen(
      active_state.display,
      "TC: ", IR_Temperature,
      "CP: ", Cooling_Period,
      "WT: ", Water_Temperature
    );
  }else{
    display->DisplayMainScreen(
      active_state.display,
      "TC: ", IR_Temperature,
      "HP: ", Heating_Period,
      "#C: ", Session_Total_Count);
  }

  // display->Error();
  // while(true) {}
  //..
}

/*
void loop() {
  // Gets USER input to Keypad
  char key = keypad.getKey();
  if (key == '#'){
    COMMAND_REQUEST = true;
  }
  if(COMMAND_REQUEST && !Heat_Active){
    commandSequence();
    COMMAND_REQUEST = false;
  }
  if(HEAT_PAUSE_STATE){
    Active_State = String("PAUSED........");
  }
  
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
  
  // ERROR STATE TEST
  if(IR_Temperature > 1200){
    ERROR_STATE = true;
  }
  if(Water_Temperature > 140){
    ERROR_STATE = true;
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

  // Updates Display if ELAPSED TIME (ms) is GREATER than DISPLAY REFRESH PERIOD (ms)
  currentMillis = millis();
  if((currentMillis-startDisplayMillis) > Display_Refresh_Period || ERROR_STATE){
    startDisplayMillis = millis();
    if((currentMillis-startDisplayInfoSwapMillis) > Display_Info_Swap_Period){
      Display_Info_Swap = !Display_Info_Swap;
      startDisplayInfoSwapMillis = millis();
    }
    if(Display_Info_Swap){
      lcdMainScreen(Active_State,"TC: ", IR_Temperature,"CP: ", Cooling_Period,"WT: ", Water_Temperature);
    }else{
      lcdMainScreen(Active_State,"TC: ", IR_Temperature,"HP: ", Heating_Period,"#C: ", Session_Total_Count);
    }
  }

  if(ERROR_STATE){
    lcd.clear();
    lcd.print("ERROR STATE");
    lcd.setCursor(0, 1);
    lcd.print("ACTIVE SHUTDOWN");
    Serial.print("\033[0H\033[0J");
    Serial.println("ERROR STATE");
    Serial.println("ACTIVE SHUTDOWN");
    while(true) {}
  }
}
*/
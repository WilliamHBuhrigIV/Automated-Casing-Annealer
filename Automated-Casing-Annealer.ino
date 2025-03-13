#include <Keypad.h>
#include <LiquidCrystal.h>
//#include <SD.h> //TODO for Backup of Settings over Powercycle

////////////////////////////////////////////////////////////////////////////////
/// Keypad Instantiation
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

////////////////////////////////////////////////////////////////////////////////
/// Liquid Crystal Display Instatiation
////////////////////////////////////////////////////////////////////////////////

const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_RS = 8;
const int pin_EN = 9;
//const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

////////////////////////////////////////////////////////////////////////////////
/// Common Variable Instatiation
////////////////////////////////////////////////////////////////////////////////

unsigned long startDisplayMillis;
unsigned long startDisplayInfoSwapMillis;
unsigned long startHeatMillis, endHeatMillis;
unsigned long currentMillis;
const int Display_Refresh_Period = 300;
const int Display_Info_Swap_Period = 4*1000;

////////////////////////////////////////////////////////////////////////////////
/// Test Variable Instatiation
////////////////////////////////////////////////////////////////////////////////

int Session_Total_Count = 0, Water_Temperature = 72, IR_Temperature = 72, Banner_Offset = 0;
double Heating_Period = 1, Cooling_Period = 1;
bool Heat_Active = false, Display_Info_Swap = false;
bool HEAT_PAUSE_STATE = true, COMMAND_REQUEST = false, ERROR_STATE = false;
String Active_State = String("PAUSED........");

////////////////////////////////////////////////////////////////////////////////
/// Run Time Code
////////////////////////////////////////////////////////////////////////////////

void lcdSetup(){
  lcd.begin(16,2);
  lcd.noAutoscroll();
  lcd.noBlink();
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initializing...");
  startDisplayMillis = millis();
}

void setup() {
  lcdSetup();
  Serial.begin(115200,SERIAL_8E1);
}

void lcdMainScreen(String INFO_1, String tag_INFO_2, int INFO_2, String tag_INFO_3, double INFO_3, String tag_INFO_4, int INFO_4){
  lcd.clear();
  if(INFO_1.length() > 7){
    Banner_Offset += 1;
  }else{
    Banner_Offset = 0;
  }
  String string_INFO_1 = String();
  for(int i=0;i<7;i++){
    string_INFO_1 += INFO_1.charAt((i+Banner_Offset) % INFO_1.length());
  }
  
  String string_INFO_2 = String();
  if(INFO_2 <= 9){
    string_INFO_2 = "   "+String(INFO_2);
  }else if(INFO_2 <= 99){
    string_INFO_2 = "  "+String(INFO_2);
  }else if(INFO_2 <= 999){
    string_INFO_2 = " "+String(INFO_2);
  }else if(INFO_2 <= 9999){
    string_INFO_2 = String(INFO_2);
  }else{
    string_INFO_2 = "ERRR";
  }
  String string_INFO_3 = String();
  if(INFO_3 <= 9.9){
    string_INFO_3 = String(INFO_3,1);
  }else if(INFO_3 <= 99){
    string_INFO_3 = " "+String(INFO_3,0);
  }else if(INFO_3 <= 999){
    string_INFO_3 = String(INFO_3,0);
  }else{
    string_INFO_3 = "ERR";
  }
  String string_INFO_4 = String();
  if(INFO_4 <= 9){
    string_INFO_4 = "   "+String(INFO_4);
  }else if(INFO_4 <= 99){
    string_INFO_4 = "  "+String(INFO_4);
  }else if(INFO_4 <= 999){
    string_INFO_4 = " "+String(INFO_4);
  }else if(INFO_4 <= 9999){
    string_INFO_4 = String(INFO_4);
  }else{
    string_INFO_4 = "ERRR";
  }
  lcd.print(string_INFO_1+" "+tag_INFO_2+string_INFO_2);
  lcd.setCursor(0, 1);
  lcd.print(tag_INFO_3+string_INFO_3+" "+tag_INFO_4+string_INFO_4);
  Serial.print("\033[0H\033[0J");
  Serial.println(string_INFO_1+" "+tag_INFO_2+string_INFO_2);
  Serial.println(tag_INFO_3+string_INFO_3+" "+tag_INFO_4+string_INFO_4);
}

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

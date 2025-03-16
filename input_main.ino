#include <Keypad.h>
#include "state.h"

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////

InputPad::InputPad(
    char *keyMap, 
    byte *row, 
    byte *col, 
    byte numRows,
    byte numCols
) {
    keypad = new Keypad(keyMap,row,col,numRows,numCols);
    return;
}
char InputPad::GetKeyState() {
    return keypad->getKey();
}
void InputPad::CommandSequence(state& active_state) {
    String Command_String = String("");
    display->DisplayCommandScreen(Command_String);
    while(true){
        char key = keypad->getKey();
        if(key){
        if (key == '#'){
            break;
        }
        if((key == '*') && (
            Command_String.lastIndexOf('*') == Command_String.length()-1
        )){
            Command_String.remove(Command_String.length()-2,2);
        }else{
            Command_String = Command_String+String(key);
        }
        display->DisplayCommandScreen(Command_String);
        }
        
    }
    String Command_String_Prefix = String();
    String Command_String_Whole = String();
    String Command_String_Decimal = String();
    if(Command_String.indexOf('*') == -1){
        Command_String_Prefix =
            Command_String.substring(0,Command_String.length());
    }else{
        Command_String_Prefix =
            Command_String.substring(0,Command_String.indexOf('*'));
    }
    if(!(Command_String.indexOf('*') == -1)){
        if(Command_String.indexOf('*') == Command_String.lastIndexOf('*')){
            Command_String_Whole = Command_String.substring(
                    Command_String.indexOf('*')+1,Command_String.length()
                );
        }else{
            Command_String_Whole = Command_String.substring(
                Command_String.indexOf('*')+1,Command_String.lastIndexOf('*')
            );
            Command_String_Decimal = Command_String.substring(
                Command_String.lastIndexOf('*')+1,Command_String.length()
            );
        }
    }
    char Command_String_Prefix_Char_Array[Command_String_Prefix.length()+1];
    Command_String_Prefix.toCharArray(
        Command_String_Prefix_Char_Array, Command_String_Prefix.length()+1
    );
    int Command_String_Prefix_Value = 0;
    for(int i=0;i<sizeof(Command_String_Prefix_Char_Array)+1;i++){
        Command_String_Prefix_Value += Command_String_Prefix_Char_Array[i];
    }
    Serial.println(Command_String_Prefix_Value);
    switch(Command_String_Prefix_Value){
        case 'A':
            // 
            break;
        case 'A'+'A':
            // ERROR_STATE = true;
            break;
        case 'D'+'D':
            // if(Command_String_Whole.toInt()){
            //     Water_Temperature = Command_String_Whole.toInt();
            // }
            break;
        case 'B':
            // if(Command_String_Whole.toInt()){
            //     IR_Temperature = Command_String_Whole.toInt();
            // }
            break;
        case 'C':
            // if(
            //     Command_String_Decimal.toDouble() || 
            //     Command_String_Whole.toInt()
            // ){
            //     Heating_Period = Command_String_Whole.toInt() + (
            //         Command_String_Decimal.toDouble() /
            //         pow(10,Command_String_Decimal.length())
            //     );
            // }else{
            //     HEAT_PAUSE_STATE = !HEAT_PAUSE_STATE;
            // }
            break;
        case 'C'+'C':
            // if(
            //     Command_String_Decimal.toDouble() ||
            //     Command_String_Whole.toInt()
            // ){
            //     Cooling_Period = Command_String_Whole.toInt() + (
            //         Command_String_Decimal.toDouble() /
            //         pow(10,Command_String_Decimal.length())
            //     );
            // }else{
            //     HEAT_PAUSE_STATE = !HEAT_PAUSE_STATE;
            // }
        case 'D':
            // if(Command_String_Whole.toInt()){
            //     Session_Total_Count = Command_String_Whole.toInt();
            // }else{
            //     Session_Total_Count = 0;
            // }
            break;
    }
}
#include <Keypad.h>
#include "state.h"

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////

InputPad::InputPad( Keypad* keypad ) {
    this->keypad = keypad;
    return;
}
char InputPad::GetKeyState() {
    char key = keypad->getKey();
    Serial.print(key);
    return key;
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
    switch(Command_String_Prefix_Value){
        case 'A':
            switch(active_state.previous_request) {
                case REQUESTS::OPERATE:
                    active_state.previous_request = REQUESTS::OPERATE;
                    active_state.request = REQUESTS::PAUSED;
                    break;
                case REQUESTS::PAUSED:
                    active_state.previous_request = REQUESTS::PAUSED;
                    active_state.request = REQUESTS::OPERATE;
                    break;
            }
            break;
        case 'A'+'A':
            active_state.request = REQUESTS::ERROR;
            break;
        case 'D'+'D':
            if(Command_String_Whole.toInt()){
                active_state.opperation.cooling.temperature_water =
                    Command_String_Whole.toInt();
            }
            break;
        case 'B':
            if(Command_String_Whole.toInt()){
                active_state.opperation.heating.temperature_ir =
                    Command_String_Whole.toInt();
            }
            break;
        case 'C':
            if(
                Command_String_Decimal.toDouble() || 
                Command_String_Whole.toInt()
            ){
                active_state.opperation.heating.period =
                    Command_String_Whole.toInt() + (
                        Command_String_Decimal.toDouble() /
                        pow(10,Command_String_Decimal.length())
                    );
            }else{
                switch(active_state.previous_request) {
                    case REQUESTS::OPERATE:
                        active_state.previous_request = REQUESTS::OPERATE;
                        active_state.request = REQUESTS::PAUSED;
                        break;
                    case REQUESTS::PAUSED:
                        active_state.previous_request = REQUESTS::PAUSED;
                        active_state.request = REQUESTS::OPERATE;
                        break;
                }
            }
            break;
        case 'C'+'C':
            if(
                Command_String_Decimal.toDouble() ||
                Command_String_Whole.toInt()
            ){
                active_state.opperation.cooling.period =
                    Command_String_Whole.toInt() + (
                        Command_String_Decimal.toDouble() /
                        pow(10,Command_String_Decimal.length())
                    );
            }else{
                switch(active_state.previous_request) {
                    case REQUESTS::OPERATE:
                        active_state.previous_request = REQUESTS::OPERATE;
                        active_state.request = REQUESTS::PAUSED;
                        break;
                    case REQUESTS::PAUSED:
                        active_state.previous_request = REQUESTS::PAUSED;
                        active_state.request = REQUESTS::OPERATE;
                        break;
                }
            }
        case 'D':
            if(Command_String_Whole.toInt()){
                active_state.opperation.count = Command_String_Whole.toInt();
            }else{
                active_state.opperation.count = 0;
            }
            break;
    }
}
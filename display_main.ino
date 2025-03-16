#include <LiquidCrystal.h>
#include "state.h"

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////

Display::Display(
    uint8_t pin_rs,
    uint8_t pin_en,
    uint8_t pin_d0,
    uint8_t pin_d1,
    uint8_t pin_d2,
    uint8_t pin_d3
) {
    display = new LiquidCrystal(pin_rs, pin_en, pin_d0, pin_d1, pin_d2, pin_d3);
    display->begin(16,2);
    display->noAutoscroll();
    display->noBlink();
    display->noCursor();
    display->clear();
    display->setCursor(0,0);
    display->print("Initializing...");
    return;
}
LiquidCrystal* Display::UnsafeExport() {
    return display;
}
void Display::Error() {
    display->clear();
    display->print("ERROR STATE");
    display->setCursor(0, 1);
    display->print("ACTIVE SHUTDOWN");
    Serial.print("\033[0H\033[0J");
    Serial.println("ERROR STATE");
    Serial.println("ACTIVE SHUTDOWN");
    return;
}
void Display::DisplayMainScreen(
    state::state_display& state_display,
    String tag_INFO_2, int INFO_2,
    String tag_INFO_3, double INFO_3,
    String tag_INFO_4, int INFO_4
){
    unsigned long current_time = millis();
    if (
        (current_time-state_display.refresh.time_last) < 
        1000.0/state_display.refresh.hz
    ) {
        return;
    }
    state_display.refresh.frame_current += 1;
    if (state_display.active.name.length() == state_display.active.offset) {
        state_display.swap = !active_state.display.swap;
        state_display.active.offset = 0;
    }
    state_display.refresh.time_last = current_time;
    display->clear();
    if (
        ( state_display.active.name.length() > 7 ) && (
            (state_display.refresh.frame_current-state_display.active.frame_last) >
            state_display.active.frame_period
        )
    ) {
        state_display.active.offset += 1;
        state_display.active.frame_last = state_display.refresh.frame_current;
    }else{
        state_display.active.offset += 0;
    }
    String string_state_display = String();
    for(int i=0;i<7;i++){
        string_state_display += state_display.active.name.charAt(
            (i+state_display.active.offset) % state_display.active.name.length()
        );
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
    display->print(string_state_display+" "+tag_INFO_2+string_INFO_2);
    display->setCursor(0, 1);
    display->print(tag_INFO_3+string_INFO_3+" "+tag_INFO_4+string_INFO_4);
    Serial.print("\033[0H\033[0J");
    Serial.println(string_state_display+" "+tag_INFO_2+string_INFO_2);
    Serial.println(tag_INFO_3+string_INFO_3+" "+tag_INFO_4+string_INFO_4);
    return;
}
void Display::DisplayCommandScreen(String Command_String){
    display->clear();
    display->print("COMMAND ENTRY:");
    display->setCursor(0, 1);
    display->print(Command_String);
    Serial.print("\033[0H\033[0J");
    Serial.println("COMMAND ENTRY:");
    Serial.println(Command_String);
    return;
}
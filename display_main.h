#ifndef DISPLAY_H
#define DISPLAY_H
#include <LiquidCrystal.h>
#include "state.h"

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////

class Display {
    // LiquidCrystal display;
    public:
        Display(
            uint8_t pin_rs,
            uint8_t pin_en,
            uint8_t pin_d0,
            uint8_t pin_d1,
            uint8_t pin_d2,
            uint8_t pin_d3
        );
        LiquidCrystal* UnsafeExport();
        void Error();
        void DisplayMainScreen(
            state::state_display& active_state,
            String tag_INFO_2, int INFO_2,
            String tag_INFO_3, double INFO_3,
            String tag_INFO_4, int INFO_4
        );
        void DisplayCommandScreen(String Command_String);
        // ..
    private:
        LiquidCrystal* display;
        // ..
};
#endif
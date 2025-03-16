#ifndef INPUT_H
#define INPUT_H
#include <Keypad.h>
#include "state.h"

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////

class InputPad {
    public:
        InputPad(
            char *keyMap, 
            byte *row, 
            byte *col, 
            byte numRows,
            byte numCols
        );
        char GetKeyState();
        // ..
    private:
        Keypad* keypad;
        // ..
};
#endif
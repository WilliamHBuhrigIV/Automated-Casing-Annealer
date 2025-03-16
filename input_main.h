#ifndef INPUT_H
#define INPUT_H
#include <Keypad.h>
#include "state.h"

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////

class InputPad {
    public:
        InputPad( Keypad* keypad );
        char GetKeyState();
        void CommandSequence(state& active_state);
        // ..
    private:
        Keypad* keypad;
        // ..
};
#endif
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
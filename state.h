#ifndef STATE_H
#define STATE_H

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////

struct state {
    String name;
    struct state_display {
        // bool swap;
        // int swap_offsets;
        struct state_display_swap {
            bool value;
            int frame_period;
            operator bool() const { return value; }
            state_display_swap& operator=(bool v) { 
                value = v; 
                return *this;
            }
        } swap;
        struct state_display_active {
            String name;
            int offset;
            float frame_period;
            unsigned long frame_last;
            operator String() const { return name; }
            state_display_active& operator=(const String& v) { 
                name = v; 
                return *this;
            }
        } active;
        struct state_display_refresh {
            int hz;
            unsigned long frame_current;
            unsigned long time_last;
        } refresh;
    } display;
};
#endif
#ifndef STATE_H
#define STATE_H

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////

enum REQUESTS {
    START,
    PAUSED,
    COMMAND,
    OPERATE,
    ERROR,
};

enum FLAGS {
    HEATING,
    COOLING,
    WAITING,
    ERRORING
};

struct state {
    REQUESTS request;
    struct state_opperation {
        FLAGS flag;
    } opperation;
    struct state_values {
        double heating_period; // TODO Storage
        double cooling_period; // TODO Storage
        int temperature_ir;
        int temperature_water;
        int count; // TODO Storage
    } values;
    struct state_display {
        struct state_display_swap {
            bool value;
            int frame_period; // TODO Storage, Modification
            operator bool() const { return value; }
            state_display_swap& operator=(bool v) { 
                value = v; 
                return *this;
            }
        } swap;
        struct state_display_active {
            String name;
            int offset;
            float frame_period; // TODO Storage, Modification
            unsigned long frame_last;
            operator String() const { return name; }
            state_display_active& operator=(const String& v) { 
                name = v; 
                return *this;
            }
        } active;
        struct state_display_refresh {
            int hz; // TODO Storage, Modification
            unsigned long frame_current;
            unsigned long time_last;
        } refresh;
    } display;
};
#endif
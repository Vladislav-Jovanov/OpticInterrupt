#ifndef TURNCOUNTER_H
#define TURNCOUNTER_H
#include "OpticInterrupt.h"
#include "HardwareSerial.h"
#include "RPM.h"

class TurnCounter
{
    public:
        TurnCounter(OpticInterrupt * chopper, unsigned int openings=1, unsigned int turns=10);//if the first one is default asigned all following should be too
        virtual ~TurnCounter();
        
        
        bool get_status() { return counter_running; };
        void display_on(){display=true;};
        void display_off(){display=false;};
        //unsigned int GetTotCounter() { return totCount; };
        //unsigned int GetCurentCounter() { return currentCount; };
        void SetCounter(unsigned int val);
        //void SetCurrentCounter(unsigned int val) {currentCount=val;};
        void start_counter();
        void stop_counter(bool multi_use_check=false);
        void main_counter(HardwareSerial * Serial);
        int process_command(String * input_command, bool mutli_use_check=false);
        void IRS_CHANGE();
        void setup(void (*end_func)()=NULL);

    protected:

    private:
        bool State;
        unsigned int Openings;
        bool display=false;
        void (*counter_end_func)();
        bool (RPM::*counter_stop_check)();
        OpticInterrupt * Chopper;
        unsigned int totTurns;
        unsigned int currentTurn;
        unsigned int Counter;
        bool Edge=false;
        bool counter_running=false;//freq and counter class
        bool init=true;
        static void pass(){};
        
};

#endif // TURNCOUNTER_H

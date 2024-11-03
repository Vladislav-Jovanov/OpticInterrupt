#ifndef TURNCOUNTER_H
#define TURNCOUNTER_H
#include "OpticInterrupt.h"
#include "HardwareSerial.h"
#include "RPM.h"

#define TCCOMMANDS "on - start turn counter\noff - stop turn counter\ndon - display results\ndoff - stop displaying results\nt<num> - set number of turns"

class TurnCounter
{
    public:
        TurnCounter(Optic_Interrupt * chopper, unsigned int openings=1);//if the first one is default asigned all following should be too
        virtual ~TurnCounter();
        
        bool get_status() { return counter_running; };
        void enable_serial(HardwareSerial * Serial_in);
        void display_on();
        void display_off();
        void start();
        void stop();//this also stops absolute positioning
        void main();
        int process_command(String * input_command);
        void IRS_change();
        void setup_finish(void (*end_func)()=NULL);
        void set_turns(unsigned int turns);


    protected:

    private:
        bool connected;
        bool display=false;
        void display_serial();
        unsigned int Openings;
        bool serial_enabled=false;
        void (*counter_end_func)()=NULL;
        Optic_Interrupt * Chopper;
        HardwareSerial * Serial=NULL;
        int totTurns=0;
        int counter;
        int currentTurn;
        bool Edge=false;
        bool counter_running=false;
};

#endif // TURNCOUNTER_H

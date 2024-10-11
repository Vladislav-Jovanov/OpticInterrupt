#ifndef OPTICINTERRUPT_H
#define OPTICINTERRUPT_H

#include <SoftwareInterrupt.h>

//what if this one would be tracking position?
//why not tracking position should be done when LED is on
class Optic_Interrupt
{
    public:
        Optic_Interrupt(int LED, int Ointp);
        virtual ~Optic_Interrupt();
        void setup(int mode);
        void setup_interrupt(void (*irs)(),int State, int delay=NULL);
        void start();
        void stop();
        void led_start();
        void led_stop();
        int connect_TC();
        int connect_RPM();
        bool is_led_on(){return LED_state;};
        int get_state(){return digitalRead(PIN_Ointp);};
        void main();
        void IRS_rising();
        void IRS_falling();
        void IRS_change();

    protected:
        
        
    private:
        bool rising_finished=false;
        bool falling_finished=false;
        bool change_finished=false;
        bool setup_finished;
        int disconnect_RPM();//do I need this?
        int disconnect_TC();//do I need this?
        bool RPM_measurement=false;
        bool Turn_counting=false;
        Software_Interrupt INTER_falling=NULL;
        Software_Interrupt INTER_rising=NULL;
        Software_Interrupt INTER_change=NULL;        
        bool LED_state;//on off
        unsigned long start_time;
        int PIN_LED;
        int PIN_Ointp;
        bool init;

};

#endif // OPTICINTERRUPT_H

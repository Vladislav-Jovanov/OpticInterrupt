#ifndef OPTICINTERRUPT_H
#define OPTICINTERRUPT_H

#include <SoftwareInterrupt.h>

struct LED_state{
    bool ledrpm;
    bool ledtc;
    bool ledcustom;
};

enum oitype{
    oicustom=0,
    oirpm=1,
    oitc=2,
};

//what if this one would be tracking position?
//why not tracking position should be done when LED is on
class Optic_Interrupt
{
    public:
        Optic_Interrupt(int LED, int Ointp);
        virtual ~Optic_Interrupt();
        void setup(int mode);
        void setup_interrupt(void (*irs)(),int State, int delay=NULL);
        void start(oitype val);
        void stop(oitype val);
        void led_start(oitype val=oicustom);
        void led_stop(oitype val=oicustom);
        int connect(oitype val);
        int disconnect(oitype val);
        bool is_led_on(){return (LED_on.ledrpm || LED_on.ledtc || LED_on.ledcustom);};
        int get_state(){return digitalRead(PIN_Ointp);};
        void main();
        void IRS_rising();
        void IRS_falling();
        void IRS_change();

    protected:
        
        
    private:
        void update_state(oitype val, bool state);
        bool rising_finished=false;
        bool falling_finished=false;
        bool change_finished=false;
        bool setup_finished;
        bool RPM_measurement=false;
        bool Turn_counting=false;
        Software_Interrupt INTER_falling=NULL;
        Software_Interrupt INTER_rising=NULL;
        Software_Interrupt INTER_change=NULL;
        LED_state LED_on;//on off
        unsigned long start_time;
        int PIN_LED;
        int PIN_Ointp;
        bool init;

};

#endif // OPTICINTERRUPT_H

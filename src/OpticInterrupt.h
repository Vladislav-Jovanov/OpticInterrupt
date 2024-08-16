#ifndef OPTICINTERRUPT_H
#define OPTICINTERRUPT_H


class Optic_Interrupt
{
    public:
        Optic_Interrupt(int LED, int Ointp);
        virtual ~Optic_Interrupt();
        void setup();
        void start();
        void stop();
        bool status(){return LED_state;};
        int get_change();

    protected:
        
        
    private:
        bool LED_state;//on off
        int PIN_LED;
        int PIN_OIntp;

};

#endif // OPTICINTERRUPT_H

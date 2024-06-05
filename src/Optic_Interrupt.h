#ifndef OPTIC_INTERRUPT_H
#define OPTIC_INTERRUPT_H


class Optic_Interrupt
{
    public:
        Optic_Interrupt(int LED, int Ointp);
        virtual ~Optic_Interrupt();
        void setup_OI();
        void start_OI();
        void stop_OI();
        bool get_state(){return LED_state;};
        int get_change();

    protected:
        
        
    private:
        bool LED_state;//this is a pointer to an indicator if the led is on or not we need it since optic interrupt can be used by counter or frequency measurement class
        int PIN_LED;
        int PIN_OIntp;

};

#endif // OPTIC_INTERRUPT_H

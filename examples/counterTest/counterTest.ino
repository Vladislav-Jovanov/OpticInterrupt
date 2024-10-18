#include <OpticInterrupt.h>
#include <TurnCounter.h>

#if defined (__AVR__)
    #define PIN_OIntp 2
    #define PIN_LED 4
#endif
#if defined (ESP32)
    #define PIN_OIntp 14
    #define PIN_LED 12
#endif



Optic_Interrupt my_interrupt(PIN_LED, PIN_OIntp);
TurnCounter my_counter(&my_interrupt, 1);

void IRS_change(){
    my_counter.IRS_change();
}

void end_function(){
    Serial.println("Counter has finished");
}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    my_interrupt.setup(INPUT);
    my_interrupt.setup_interrupt(&IRS_change, CHANGE,10);
    my_counter.enable_serial(&Serial);
    my_counter.display_on();
    my_counter.set_turns(20);
    my_counter.setup_finish(&end_function);
    my_counter.start();
}



void loop() {
    my_interrupt.main();
    my_counter.main();
    if (!my_counter.get_status()){
        my_counter.start();    
    }
}

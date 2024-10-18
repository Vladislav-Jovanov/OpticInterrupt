#include <OpticInterrupt.h>

#if defined (__AVR__)
    #define PIN_OIntp 2
    #define PIN_LED 4
#endif
#if defined (ESP32)
    #define PIN_OIntp 14
    #define PIN_LED 12
#endif



Optic_Interrupt my_interrupt(PIN_LED, PIN_OIntp);

void IRS_rising(){
    Serial.println("RISING");
}

void IRS_falling(){
    Serial.println("FALLING");
}

void IRS_change(){
    Serial.println("CHANGE");
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    my_interrupt.setup(INPUT);
    my_interrupt.setup_interrupt(&IRS_rising, RISING,10);
    my_interrupt.setup_interrupt(&IRS_falling, FALLING,10);
    my_interrupt.setup_interrupt(&IRS_change, CHANGE,10);
    my_interrupt.led_start();
}



void loop() {
    my_interrupt.main();
}

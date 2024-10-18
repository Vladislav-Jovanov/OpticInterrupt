#include <OpticInterrupt.h>
#include <RPM.h>

#if defined (__AVR__)
    #define PIN_OIntp 2
    #define PIN_LED 4
#endif
#if defined (ESP32)
    #define PIN_OIntp 14
    #define PIN_LED 12
#endif



Optic_Interrupt my_interrupt(PIN_LED, PIN_OIntp);
RPM my_rpm(&my_interrupt, 1);

void IRS_rising(){
    my_rpm.IRS_rising();
}

void IRS_falling(){
    my_rpm.IRS_falling();
}


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    my_interrupt.setup(INPUT);
    my_interrupt.setup_interrupt(&IRS_rising, RISING,10);
    my_interrupt.setup_interrupt(&IRS_falling, FALLING,10);
    my_rpm.enable_serial(&Serial);
    my_rpm.display_on();
    my_rpm.start();
}



void loop() {
    my_interrupt.main();
    my_rpm.main();
}

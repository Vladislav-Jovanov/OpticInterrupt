#include "OpticInterrupt.h"
#include "Arduino.h"

Optic_Interrupt::Optic_Interrupt(int LED, int Ointp){
    PIN_LED=LED;
    PIN_Ointp=Ointp;
}


Optic_Interrupt::~Optic_Interrupt(){
    //dtor
}

void Optic_Interrupt::update_state(oitype val, bool state){
    switch (val){
        case oirpm:
            LED_on.ledrpm=state;
            break;
        case oitc:
            LED_on.ledtc=state;
            break;
        case oicustom:
            LED_on.ledcustom=state;
            break;
    }
}

void Optic_Interrupt::start(oitype val){
    if(RPM_measurement||Turn_counting){
        if (is_led_on()){
            update_state(val,true);
        }else{
            led_start(val);
        }
    }
}

void Optic_Interrupt::led_start(oitype val){
    if (!is_led_on() && setup_finished){
        digitalWrite(PIN_LED, HIGH);
        start_time=micros();
        init=true;
        update_state(val,true);
        if (falling_finished){
            INTER_falling.set_init(true); //init first state
        }
        if (rising_finished){
            INTER_rising.set_init(true);
        }
        if (change_finished){
            INTER_change.set_init(true);
        }
    }
}


void Optic_Interrupt::stop(oitype val){
    if(RPM_measurement||Turn_counting){
        led_stop(val);
    }
}


void Optic_Interrupt::led_stop(oitype val){
    update_state(val,false);
    if (!is_led_on()){
        digitalWrite(PIN_LED, LOW);
    }
}

int Optic_Interrupt::connect(oitype val){
    if(val==oirpm && !RPM_measurement){
        RPM_measurement=true;
        return 0;
    }else if (val==oitc && !Turn_counting){
        Turn_counting=true;
        return 0;
    }else{
        return 1;
    }
}

int Optic_Interrupt::disconnect(oitype val){
    if(val==oirpm && RPM_measurement){
        RPM_measurement=false;
        led_stop(val);
        return 0;
    }else if (val==oitc && Turn_counting){
        Turn_counting=false;
        led_stop(val);
        return 0;
    }else{
        return 1;
    }
}

void Optic_Interrupt::setup(int mode){
    pinMode(PIN_LED, OUTPUT);//to turn on the led
    if (mode==(INPUT || INPUT_PULLUP)){
        pinMode(PIN_Ointp, mode);
        setup_finished=true;
        if (falling_finished){
            INTER_falling.set_setup(true);
            INTER_falling.set_init(true);
        }
        if (rising_finished){
            INTER_rising.set_setup(true);
            INTER_rising.set_init(true);
        }
        if (change_finished){
            INTER_change.set_setup(true);
            INTER_change.set_init(true);
        }
    }

//we have external pullup resistor connected to PIN_LED. PIN_LED-> HIGH  PIN_OIntp LOW->LOW when sensor illuminated
//                                                                       PIN_OIntp LOW->High when sensor covered
//we will have fake trigger in second case so we wont react to it but we will react to first next change from HIGH to LOW
//only if you measure time can you distinguish when sensor was illuminated and then got covered
}

void Optic_Interrupt::setup_interrupt(void (*irs)(),int State,int delay_value){
    switch (State){
        case RISING:
            INTER_rising=Software_Interrupt(PIN_Ointp);
            INTER_rising.attachInterrupt(irs,State,delay_value);
            if(setup_finished){
                INTER_rising.set_setup(true);
                INTER_rising.set_init(true);
            }
            rising_finished=true;
            break;

        case FALLING:
            INTER_falling=Software_Interrupt(PIN_Ointp);
            INTER_falling.attachInterrupt(irs,State,delay_value);
            if(setup_finished){
                INTER_falling.set_setup(true);
                INTER_falling.set_init(true);
            }
            falling_finished=true;
            break;

        case CHANGE:
            INTER_change=Software_Interrupt(PIN_Ointp);
            INTER_change.attachInterrupt(irs,State,delay_value);
            if(setup_finished){
                INTER_change.set_setup(true);
                INTER_change.set_init(true);
            }
            change_finished=true;
            break;
    }
}

//here you put delay
void Optic_Interrupt::main(){
    if (is_led_on()){
        //this part is to avoid fake edge when LED is starting
        if (init){
            if (micros()-start_time>1000){
                init=false;
            }
        }else{
            if (falling_finished){
                INTER_falling.main();
            }
            if (rising_finished){
                INTER_rising.main();
            }
            if (change_finished){
                INTER_change.main();
            }
        }
    }
}

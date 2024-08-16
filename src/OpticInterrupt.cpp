#include "OpticInterrupt.h"
#include "Arduino.h"

Optic_Interrupt::Optic_Interrupt(int LED, int Ointp){
    PIN_LED=LED;
    PIN_OIntp=Ointp;
}


Optic_Interrupt::~Optic_Interrupt(){
    //dtor
}

void Optic_Interrupt::start(){
  digitalWrite(PIN_LED, HIGH);
  LED_state=true;
}
void Optic_Interrupt::stop(){
  digitalWrite(PIN_LED, LOW);
  LED_state=false;
}

void Optic_Interrupt::setup(){
    pinMode(PIN_LED, OUTPUT);//to turn on the led
    pinMode(PIN_OIntp, INPUT);
//we have external pullup resistor connected to PIN_LED. PIN_LED-> HIGH  PIN_OIntp LOW->LOW when sensor illuminated 
//                                                                       PIN_OIntp LOW->High when sensor covered
//we will have fake trigger in second case so we wont react to it but we will react to first next change from HIGH to LOW
//only if you measure time can you distinguish when sensor was illuminated and then got covered                                 
}

int Optic_Interrupt::get_change(){
  return digitalRead(PIN_OIntp);
}

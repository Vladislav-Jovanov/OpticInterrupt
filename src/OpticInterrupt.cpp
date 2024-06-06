#include "OpticInterrupt.h"
#include "Arduino.h"

Optic_Interrupt::Optic_Interrupt(int LED, int Ointp){
    PIN_LED=LED;
    PIN_OIntp=Ointp;
}


Optic_Interrupt::~Optic_Interrupt(){
    //dtor
}

void Optic_Interrupt::start_OI(){
  digitalWrite(PIN_LED, HIGH);
  LED_state=true;
}
void Optic_Interrupt::stop_OI(){
  digitalWrite(PIN_LED, LOW);
  LED_state=false;
}

void Optic_Interrupt::setup_OI(){
    pinMode(PIN_LED, OUTPUT);//setup OI
    pinMode(PIN_OIntp, INPUT);//setup OI
}

int Optic_Interrupt::get_change(){
  return digitalRead(PIN_OIntp);
}

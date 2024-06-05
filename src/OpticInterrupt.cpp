#include "OpticInterrupt.h"
#include "Arduino.h"

OpticInterrupt::OpticInterrupt(int LED, int Ointp){
    PIN_LED=LED;
    PIN_OIntp=Ointp;
}


OpticInterrupt::~OpticInterrupt(){
    //dtor
}

void OpticInterrupt::start_OI(){
  digitalWrite(PIN_LED, HIGH);
  LED_state=true;
}
void OpticInterrupt::stop_OI(){
  digitalWrite(PIN_LED, LOW);
  LED_state=false;
}

void OpticInterrupt::setup_OI(){
    pinMode(PIN_LED, OUTPUT);//setup OI
    pinMode(PIN_OIntp, INPUT);//setup OI
}

int OpticInterrupt::get_change(){
  return digitalRead(PIN_OIntp);
}

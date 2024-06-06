#include <OpticInterrupt.h>

#define PIN_OIntp 2
#define PIN_LED 4

Optic_Interrupt my_interrupt(PIN_LED, PIN_OIntp);

void Interrupt_Action(){
  Serial.println(my_interrupt.get_change());
 }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  my_interrupt.setup_OI();
  attachInterrupt(digitalPinToInterrupt(PIN_OIntp), Interrupt_Action, CHANGE);
  my_interrupt.start_OI();
}



void loop() {
  // put your main code here, to run repeatedly:

}

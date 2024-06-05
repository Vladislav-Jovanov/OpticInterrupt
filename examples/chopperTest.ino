#define PIN_OIntp 2
#define PIN_LED 4

void Interrupt_Action(){
  int state= digitalRead(PIN_OIntp);
  Serial.println(state);
 }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_OIntp, INPUT);
  Interrupt_Action();
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED,HIGH);
  Interrupt_Action();
  attachInterrupt(digitalPinToInterrupt(PIN_OIntp), Interrupt_Action, CHANGE);
}



void loop() {
  // put your main code here, to run repeatedly:

}

#include "TurnCounter.h"
#include "OpticInterrupt.h"
#include "Arduino.h"
#include "RPM.h"

TurnCounter::TurnCounter(Optic_Interrupt * chopper, unsigned int openings, unsigned int turns)
{
    Chopper=chopper;
    totTurns=turns;
    Openings=openings;
}

TurnCounter::~TurnCounter()
{
    //dtor
}


void TurnCounter::start_counter(){
  if (!Chopper->status()){
    Chopper->start_OI();
  }
  init=true;
  counter_running=true;
  Counter=0;
  currentTurn=0;
}

void TurnCounter::stop_counter(bool multi_use_check){
  if (!multi_use_check){ 
    Chopper->stop_OI();
  }
  counter_running=false;
}


void TurnCounter::setup(void (*end_func)()){
  if (end_func==NULL){
    counter_end_func=&pass;
  }else{
    counter_end_func=end_func;
  }
}

void TurnCounter::main_counter(HardwareSerial * Serial){
    if (Edge){
        Counter++;
        if (!(Counter % Openings)& Counter!=0){
          currentTurn++;
          Counter=0;
          if (display){
            Serial->print("#Round:");
            Serial->print(currentTurn);
            Serial->print("/");
            Serial->println(totTurns);
          }
        }
        if (currentTurn>=totTurns){
          counter_end_func();
        }
        Edge=false;
    }

}

void TurnCounter::IRS_CHANGE(){
    if (counter_running){
      if (init){
        State=!Chopper->get_change();//turn will always start when the blade passes and sensor gets illuminated
        init=false; 
      }else if (Chopper->get_change()==State){
        Edge=true;
      }  
    }
}

void TurnCounter::SetCounter(unsigned int val){
  if (!counter_running){
    totTurns=val;
  }
}


int TurnCounter::process_command(String *input_command, bool multi_use_check){
  if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+3)=="on"){
      start_counter();
      return 0;
  }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+4)=="off"){
      stop_counter(multi_use_check);
      return 0;
  }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+4)=="don"){
      display_on();
      return 0;
  }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+5)=="doff"){
      display_off();
      return 0;
  }else if ((input_command->substring(input_command->indexOf("_")+1).toInt()<=0) & (input_command->substring(input_command->indexOf("_")+1)!="0")){
      //unallowed command
      return 1;
  }else{
      SetCounter(input_command->substring(input_command->indexOf("_")+1).toInt());
      return 0;
  }

}

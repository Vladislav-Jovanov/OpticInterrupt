#include "TurnCounter.h"
#include "OpticInterrupt.h"
#include "Arduino.h"
#include "RPM.h"

TurnCounter::TurnCounter(Optic_Interrupt * chopper, unsigned int openings)
{
    int answer=chopper->connect(oitc);
    if (!answer){
        Chopper=chopper;
        Openings=openings;
        connected=true;
    }
}

TurnCounter::~TurnCounter()
{
    //dtor
}

void TurnCounter::enable_serial(HardwareSerial * Serial_in){
    Serial=Serial_in;
    serial_enabled=true;
}

void TurnCounter::start(){
    if (connected){
        Chopper->start(oitc);
        counter_running=true;
        counter=0;
        currentTurn=0;
    }
}

void TurnCounter::stop(){
    if (connected){
        Chopper->stop(oitc);
        counter_running=false;
    }
}

void TurnCounter::IRS_change(){
    if(counter_running){
        Edge=true;
    }
}

void TurnCounter::set_turns(unsigned int turns){
    if (!counter_running){
        totTurns=turns;
    }
}


void TurnCounter::setup_finish(void (*end_func)()){
    if (!counter_running){
        counter_end_func=end_func;
    }
}

void TurnCounter::display_on(){
    if (serial_enabled){
        display=true;
    }
}

void TurnCounter::display_off(){
    display=false;
}

void TurnCounter::display_serial(){
    if (serial_enabled && display){
        Serial->print("#Round:");
        Serial->print(currentTurn);
        Serial->print("/");
        Serial->println(totTurns);
    }
}

void TurnCounter::main(){
    if (Edge){
        if (!(counter % 2*Openings)& counter>=1){
            currentTurn++;
            counter=0;
            display_serial();

        }
        if (currentTurn>=totTurns){
            if (counter_end_func!=NULL){
                counter_end_func();
            }
            stop();
        }
        counter++;
        Edge=false;
    }
}


int TurnCounter::process_command(String *input_command){
    if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+3)=="on"){
        start();
        return 0;
    }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+4)=="off"){
        stop();
        return 0;
    }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+4)=="don"){
        display_on();
        return 0;
    }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+5)=="doff"){
        display_off();
        return 0;
    }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+2)=="t"){
            if (input_command->substring(input_command->indexOf("t")+1).toInt()<=0){
                return 1;
            }else{
                set_turns(input_command->substring(input_command->indexOf("t")+1).toInt());
                return 0;
            }
    }else{
        return 1;
    }
}

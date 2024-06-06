#include "RPM.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include "OpticInterrupt.h"

RPM::RPM(Optic_Interrupt * chopper, int openings, int average)
{
    Chopper=chopper;
    Openings=openings;
    Average=average;
}

RPM::~RPM()
{
    //dtor
}


void RPM::start_measurement(){
  if (!Chopper->status()){
    Chopper->start_OI();
  }
  init=true;
  frontSum=0;
  backSum=0;
  frontCounter=0;
  backCounter=0;
  measurement_status=true;
  frontTimenew=0;
  frontTimeold=0;
  backTimenew=0;
  backTimeold=0;
}
void RPM::stop_measurement(bool multi_use_check){
  if (!multi_use_check){ 
    Chopper->stop_OI();
  }
  measurement_status=false;
}

void RPM::main_process(bool &edge, unsigned long &oldtime, unsigned long &newtime, double &sum, unsigned int &counter, String Direction, HardwareSerial * Serial){
    if (edge){
      if (oldtime==0){
        oldtime=micros();
      }else{
        counter++;
        //Serial->println(counter);
      }
      //this will be for the gyroscope
      if (Average==0 & counter==1){
        newtime=micros();
        //add time period in seconds
        sum+=(double)(newtime-oldtime)/(1000.*1000.);
        oldtime=newtime;
        //ovaj samo meri menja ovo
        Serial->print("Time (us):");
        Serial->print(oldtime);
        Serial->print(Direction);
        Serial->print("freq (Hz):");
        //provide frequency
        Serial->println(1/((double)Openings*sum),6);
        counter=0;
        sum=0;
      //this is for motors, counter!=0 is to skip the very first edge
      }else if (Average!=0 & !(counter % (Average*Openings)) & counter!=0){
        newtime=micros();
        sum+=(double)(newtime-oldtime)/(1000.*1000.);
        oldtime=newtime;
        //Serial->print("Time (us):");
        //Serial->print(oldtime);
        Serial->println(Direction);
        Serial->print("freq (Hz):");
        //since average is in turns
        Serial->println((double)Average/sum,6);
        sum=0;
        counter=0;
      }
      edge=false;
  }
}

void RPM::main(HardwareSerial * Serial){
  main_process(front_edge, frontTimeold, frontTimenew, frontSum, frontCounter, " Front ", Serial);
  main_process(back_edge, backTimeold, backTimenew, backSum, backCounter, " Back ", Serial);
}

//front and back edge are only set if measurement_status is true, no reason to check measurement_status in main
void RPM::IRS_CHANGE(){
    if (measurement_status){
        if (init){
         State=Chopper->get_change();
         init=false;
        }else{
          if (Chopper->get_change()==State){
             front_edge=true;
          }else{
             back_edge=true;
          } 
        }
    }

}

int RPM::process_command(String *input_command, bool multi_use_check){
  if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+3)=="on"){
      start_measurement();
      return 0;
  }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+4)=="off"){
      stop_measurement(multi_use_check);
      return 0;
  }else if ((input_command->substring(input_command->indexOf("_")+1).toInt()<=0) & (input_command->substring(input_command->indexOf("_")+1)!="0")){
      //unallowed command
      return 1;
  }else{
      set_average(input_command->substring(input_command->indexOf("_")+1).toInt());
      return 0;
  }

}

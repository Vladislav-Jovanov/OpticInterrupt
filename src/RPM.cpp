#include "RPM.h"
#include "HardwareSerial.h"
#include "Arduino.h"
#include "OpticInterrupt.h"

RPM::RPM(Optic_Interrupt * chopper, int openings, int average)
{
    int answer=chopper->connect_RPM();
    if (!answer){
        Chopper=chopper;
        Openings=openings;
        Average=average;
        connected=true;
    }
}

RPM::~RPM()
{
    //dtor
}

void RPM::enable_serial(HardwareSerial * Serial_in){
    Serial=Serial_in;
    serial_enabled=true;
}


void RPM::IRS_falling(){
    if (measurement_status){
        FallingEdge=true;
    }
}

void RPM::IRS_rising(){
    if (measurement_status){
        if (init){
            init=false;//skips the first change from low to high
        }else{        
            RisingEdge=true;
        }
    }
}


/*
//this can be used for hardware and software interrupt
void RPM::IRS_CHANGE(){
    if (measurement_status){
        if (init){
         State=!Chopper->get_change(); //to react to the first next edge so in our setup that will always be from HIGH to LOW
         init=false;
        }else{
          if (Chopper->get_change()==State){
             RisingEdge=true;
          }else{
             FallingEdge=true;
          } 
        }
    }
}
*/

void RPM::start(){
    if (connected){
        Chopper->start();
        init=true;
        RisingSum=0;
        FallingSum=0;
        RisingCounter=0;
        FallingCounter=0;
        DataOut.RisingTime=0;
        DataOut.FallingTime=0;
        measurement_status=true;
    }
}
void RPM::stop(){
    if (connected){
        Chopper->stop();
    }
    measurement_status=false;
}

void RPM::process_time(unsigned long &current_time, double &sum){
    unsigned long newtime=micros();
    //add time period in seconds
    sum+=(double)(newtime-current_time)/(1000.*1000.);
    current_time=newtime;
}

void RPM::display_data(unsigned long &current_time, float &freq, String Direction){
    if (serial_enabled){
        Serial->print("Time (us):");
        Serial->print(current_time);
        Serial->print(Direction);
        Serial->print("freq (Hz):");
        Serial->println(freq,6);
    }
}

void RPM::main_process(bool &edge, bool &data_measured, unsigned long &current_time, double &sum, float &freq, unsigned int &counter, String Direction){
    if (edge){
        if (current_time==0){
            //record the first time upon measurement start
            current_time=micros();
            return;//skips the rest
        }else{
            counter++;
        }
        //without averging and with averaging
        if ((Average==0 && counter==1)||(Average!=0 && !(counter % (Average*Openings)) && counter>=1)){
            process_time(current_time, sum);
            freq=1/((double)Openings*sum);
            display_data(current_time, freq, Direction);
            data_measured=true;
            counter=0;
            sum=0;
        }
        edge=false;
    }
}

void RPM::main(){
    main_process(RisingEdge, data_measured_rising, DataOut.RisingTime, RisingSum, DataOut.RisingFreq, RisingCounter, " Front ");
    main_process(FallingEdge, data_measured_falling, DataOut.FallingTime, FallingSum, DataOut.FallingFreq, FallingCounter, " Back ");
    if (data_measured_rising && data_measured_falling){
        data_read=false;
        data_measured_rising=false;
        data_measured_falling=false;
    }
}

RPMdataout RPM::get_data(){
    if (!data_read){
        data_read=true;
        return DataOut;   
    }
}

int RPM::process_command(String *input_command){
    if (serial_enabled){
        if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+3)=="on"){
            start();
            return 0;
        }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+4)=="off"){
            stop();
            return 0;
        }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+5)=="avg?"){
            Serial->println(get_average());
            return 0;
        }else if (input_command->substring(input_command->indexOf("_")+1,input_command->indexOf("_")+4)=="avg"){
            if (input_command->substring(input_command->indexOf("g")+1).toInt()<=0){         
                return 1;
            }else{           
                set_average(input_command->substring(input_command->indexOf("g")+1).toInt());
                return 0;
            }
        }else{
            return 1;
        }
    }
}

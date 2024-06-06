#ifndef RPM_H
#define RPM_H

#include "HardwareSerial.h"
#include "OpticInterrupt.h"

//type of measurement is always continuous
class RPM
{
    public:
        RPM(Optic_Interrupt * chopper, int openings=1, int average=0);
        virtual ~RPM();

        bool get_status() { return measurement_status; }
        void main(HardwareSerial * Serial);
        void start_measurement();
        void stop_measurement(bool multi_use_check=false);
        void set_average(int val){Average=val;};
        int get_average(){return Average;};
        int process_command(String * input_command, bool multi_use_check=false);
        void IRS_CHANGE();
        static bool pass(){};
        //struct out_data{unsigned long time; unsigned long freq;};

    protected:

    private:
        void main_process(bool &edge, unsigned long &oldtime, unsigned long &newtime, double &sum, unsigned int &counter, String Direction, HardwareSerial * Serial);
        Optic_Interrupt * Chopper;
        int Openings;
        int Average;
        unsigned int frontCounter;
        unsigned int backCounter;
        bool front_edge=false;//freq and counter class
        bool back_edge=false;//freq and counter class
        bool measurement_status=false;//freq and counter class
        unsigned long frontTimenew;//freq and counter class
        unsigned long frontTimeold;//freq and counter class
        unsigned long backTimenew;//freq and counter class
        unsigned long backTimeold;//freq and counter class
        double frontSum;
        double backSum;
        bool State;
        bool init=true;
        //unsigned long frontFreq;
        //unsigned long backFreq;
};

#endif // RPM_H

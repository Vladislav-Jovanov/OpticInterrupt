#ifndef RPM_H
#define RPM_H

#include "HardwareSerial.h"
#include "OpticInterrupt.h"
#include "SoftwareInterrupt.h"

#define RPMCOMMANDS "on - start rpm measurement\noff - stop rpm measurement\ndon - display results\ndoff - stop displaying results\navg<num> - set avergaging\navg? - display avergage value\n"

//rising from low to high when
struct RPMdataout{
    unsigned long FallingTime;
    float FallingFreq;
    unsigned long RisingTime;
    float RisingFreq;
};

//in this setup we can't know  if first RISING edge is due to the blocked start or if the chopper moved and blocked the light
class RPM
{
    public:
        RPM(Optic_Interrupt * chopper, int openings=1, int average=0);
        virtual ~RPM();

        bool get_status() {return measurement_status;};
        bool is_data_read(){return data_read;};
        RPMdataout get_data();
        void enable_serial(HardwareSerial * Serial_in);
        void display_on();
        void display_off();
        void main();
        void start();
        void stop();
        void set_average(int val){Average=val;};
        int get_average(){return Average;};
        int process_command(String * input_command);
        void IRS_falling();
        void IRS_rising();

    protected:

    private:
        bool data_measured_rising;
        bool data_measured_falling;
        bool data_read=true;
        void process_time(unsigned long &current_time, double &sum);
        void display_serial(unsigned long &current_time, float &freq, String Direction);
        bool display=false;
        bool connected=false;
        bool serial_enabled=false;
        void main_process(bool &edge, bool &data_measured, unsigned long &current_time, double &sum, float &freq, unsigned int &counter, String Direction);
        Optic_Interrupt * Chopper=NULL;
        HardwareSerial * Serial=NULL;
        int Openings;
        int Average;
        RPMdataout DataOut;
        unsigned int RisingCounter;
        unsigned int FallingCounter;
        bool RisingEdge=false;
        bool FallingEdge=false;
        bool measurement_status=false;
        double RisingSum;
        double FallingSum;
};

#endif // RPM_H


#include <zmq.hpp>
#include <iostream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <time.h>
#include <iomanip>
#include <typeinfo>


#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class pidController{
  public:
    pidController(float, float, float, float, float,float);
    ~pidController();
    float pidController_Update(float setpoint, float measurement);

private:
    // float Kp=0;
    // float Ki=0;
    // float Kd=0;

    float K1=0;
    float K2=0;
    float K3=0;

    float error[3] = {0.0,0.0,0.0};
    float outpid[2] ={0.0,0.0};

    float limMinInt=0;
    float limMaxInt=0;

    float limMin = 0;
    float limMax = 0;

    float tau=10;
    float tm;

    float integrator = 0;
    float differentiator = 0;
    float prevError = 0;
    float prevMeasurement = 0;


};

#endif

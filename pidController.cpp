
#include "pidController.h"

pidController::pidController(float Kp, float Ki, float Kd, float sTime, float MinInt, float MaxInt){
  // Kp = Kpro;
  // Ki = Kinte;
  // Kd = Kder;
  // tm = sampleTime;
  K1 = Kp+Ki*sTime+(Kd/sTime);
  K2 = -2*(Kd/sTime)-Kp;
  K3 = Kd/sTime;

  limMin = MinInt;
  limMax = MaxInt;
}

pidController::~pidController(){
  std::cout <<"The control it's finished"<< std::endl;
}

float pidController::pidController_Update(float setpoint, float measurement){
  // Error signal
  error[0] = setpoint - measurement;

  // PID
  outpid[0] = K1*error[0]+K2*error[1]+K3*error[2]+outpid[1];


  error[2] = error[1];
  error[1] = error[0];
  outpid[1] = outpid[0];

  if(outpid[0] > limMax) {
    outpid[0] = limMax;
  }
  else if (outpid[0] < limMin) {
    outpid[0] = limMin;
  }

  return outpid[0];

}

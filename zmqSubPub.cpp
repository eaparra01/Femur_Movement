#include <zmq.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> //para usar setprecision
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <typeinfo>
#include "Methods.h"
#include "pidController.h"
#include <cmath>
#include <unistd.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>


#define sleep(n)    Sleep(n)
#endif

static const float pi = 3.141592;

int main()
{

    zmq::context_t contextSub (1);
    zmq::socket_t subscriber (contextSub, ZMQ_SUB);
    subscriber.connect ("tcp://localhost:6000");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    zmq::context_t contextPub (1);
    zmq::socket_t publisher (contextPub, ZMQ_PUB);
    publisher.bind ("tcp://*:1234");

    std::string msg;
    std::string type;

    bool flagDir = true; // Clockwise
    bool flagMiddle = false;
    float numForce[6] = {0.00,0.00,0.00,0.00,0.00,0.00};  // Change 5

    float angleDesired = 0;
    float angleBefore = 0;
    float anglePresent = 0;

    float angVelDesired = 0;
    float angVelPresente = 0;

    float kp = 250;
    float ki = 1;
    float kd = 19;
    float t = 0.0;

    pidController pid(kp,ki,kd,0.001,-500.3,500.3);
    // std::ofstream myfile ("data_withoutinitialforce_negative_angle.txt");
    // myfile.is_open();
    // myfile<<"time "<<"force "<<"angle\n";

    while (1)
    {

      zmq::message_t reply;
      subscriber.recv (&reply);
	    msg = std::string( (char*)reply.data(), reply.size() );

	    int vec_size = msg_size(msg, type)+2; // int:#1  int:#2 +2

      float varPosQ[vec_size];
      float varPosR[2];

      convert_msg_qt(msg, vec_size, varPosQ);
      convert_qt_angles(vec_size, varPosQ, varPosR);


      anglePresent = varPosR[0];
      angleDesired = 1.0*cos(12.5*t); //15.5195

      numForce[4] = pid.pidController_Update(angleDesired,anglePresent)*(1/0.73);
      // myfile<< std::to_string(t)<<" "<<std::to_string(numForce[4])<<" "<<std::to_string(anglePresent)<<"\n";
      std::cout<< t <<" "<<numForce[4]<< " " <<anglePresent<<" "<<std::endl;
      t = t + 0.001;

      std::string mesg;
      create_msg_zqm("/forcefield ", "matrix int:6 int:1 ", numForce, 6, mesg);
      zmq::message_t reply_pub (mesg.size());
      memcpy (reply_pub.data (), mesg.c_str(), mesg.size());
      publisher.send (reply_pub);
    }
    // myfile.close();
    return 0;
}

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip> //para usar setprecision
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <typeinfo>
#include "Methods.h"
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
    float numForce[6] = {0.0,0.0,0.0,0.0,1.1,0.0};  // Change 5

    float angleDesired = 0;
    float angleBefore = 0;
    float anglePresent = 0;

    float angVelDesired = 0;
    float angVelPresente = 0;

    float kp = 0.4;
    float kd = 1.9;
    float ff = 1.5;
    float t = 0.0;

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
      angleDesired = (pi/6)*cos(15.5195*t);

      //numForce[4] =(ff-anglePresent/2)+ kp*(angleDesired-anglePresent);// + kd*(angVelDesired-angVelPresente);
      numForce[4] = kp*(angleDesired-anglePresent);

      angleBefore = anglePresent;

      std::cout << varPosR[0] << " "<<numForce[4]<< " " <<angleDesired<<std::endl;
      //numForce[4] = 0.9;
      t = t + 0.002;

      std::string mesg;
      create_msg_zqm("/forcefield ", "matrix int:6 int:1 ", numForce, 6, mesg);
      //std::cout << mesg << std::endl;
      zmq::message_t reply_pub (mesg.size());
      memcpy (reply_pub.data (), mesg.c_str(), mesg.size());
      publisher.send (reply_pub);
    }
    return 0;
}

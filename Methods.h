#pragma once

#ifndef _METHODS_
#define _METHODS_

#include <string>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <complex>
#include <time.h>
#include <iomanip>
#include <typeinfo>


int msg_size(const std::string& message, std::string& subjet); // Find the size of the message and the subject

void convert_msg_qt(const std::string& message, int vec_size, float *vector); // Take the values of the message and give the values in float

void convert_qt_angles(int vec_size, float *vector, float *vectorRm); // Convert the qt to angles x-z plane and y-z plane

void create_msg_zqm(const std::string& subjet, const std::string& type, float *vector, int vec_size, std::string& msgTosend);


#endif /* _METHODS_H */

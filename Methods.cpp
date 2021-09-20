
#include "Methods.h"
#include <cmath>
#include <complex>

static const float pi = 3.141592;

/////////////////////////7
int msg_size(const std::string& message, std::string& subjet)
{
    //std::string msg = message;
    std::string delimiter_1 = " ";
    std::string delimiter_2 = ":";

    std::string vectString[4];
    int vectValues[2];

    int i = 0;
    int j = 0;
    int start = 0;
    int end = message.find(delimiter_1);
    int end_ = 0;

    while (i < 4 ) {
        vectString[i] = message.substr(start, end - start);
    	if(i==0){
    	    subjet = vectString[0];
    	}
    	else if(i>1){
	        end_ = vectString[i].find(delimiter_2, 0)+delimiter_2.size();
          vectValues[j] = std::stof(vectString[i].substr(end_, end_ + vectString[i].size()));
	        //std::cout << vectValues[j] <<std::endl;
	        j++;
	    }
	    start = end + delimiter_1.size();
      end = message.find(delimiter_1, start);
      i++;
    }
    return vectValues[0]*vectValues[1];
  }
////////////////////
void convert_msg_qt(const std::string& message, int vec_size, float *vector){

  //std::string msg = message;
  std::string delimiter_1 = " ";
  std::string delimiter_2 = ":";

  std::string vectString[vec_size+2];
  //int vectValues[vec_size];

  int i = 0;
  int j = 0;
  int start = 0;
  int end = message.find(delimiter_1);
  int end_ = 0;

  while (i < vec_size + 2) {
    vectString[i] = message.substr(start, end - start);
    if(i>1){
        end_ = vectString[i].find(delimiter_2, 0)+delimiter_2.size();
        vector[j] = std::stof(vectString[i].substr(end_, end_ + vectString[i].size()));
        //std::cout << vectValues[j] <<std::endl;
        j++;
    }
    start = end + delimiter_1.size();
    end = message.find(delimiter_1, start);
    i++;
  }
}

void convert_qt_angles(int vec_size, float *vector, float *vectorRm){
  // psi x - theta y - pfi z quaternion
  // # - # - Px - Py - Pz - Qz - Qy - Qx - Qe - Px - Py - Pz - Qz - Qy - Qx - Qe
  // 0   1   2    3    4    5    6    7    8    9    10   11   12   13   14    15

  float femurLenght = 0.2645;

  float disPosx = vector[9] - vector[2];
  float disPosy = vector[10] - vector[3];
  float disPosz = vector[11] - vector[4];

  float thetaHip  = asin(disPosx/femurLenght);
   /*           theta from x-z plane     */
   if( disPosz>=0 && disPosx>=0)
   {
      thetaHip = -thetaHip + pi;

   }
   else if(disPosz>=0 && disPosx<=0)
   {
      thetaHip = -thetaHip - pi;
   }

   float pfiHip = asin(disPosy/femurLenght);
   if( disPosz>=0 && disPosy>=0)
   {
      pfiHip = -pfiHip + pi;
   }
   else if(disPosz>=0 && disPosy<=0)
   {
      pfiHip = -pfiHip - pi;
   }

    vectorRm[0] = -thetaHip;
    vectorRm[1] = -pfiHip;
}


void create_msg_zqm(const std::string& subjet, const std::string& type, float *vector, int vec_size, std::string& msgTosend){

  //      std::string mesg = "/forcefield ";
  //      mesg += "matrix int:6 int:1 ";
  msgTosend = subjet;
  msgTosend += type;
  int i=0;
  //msgTosend+="float:"+ss1.str()+" " + "float:"+ss2.str()+" " + "float:"+ss3.str()+" " + "float:"+ss4.str()+" " + "float:"+ss5.str()+" " + "float:"+ss6.str()+" ";
  while(i<=vec_size-1){
    msgTosend += "float:" + std::to_string((float)vector[i]) + " ";
    i++;
  }
}

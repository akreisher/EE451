#include "lib/matlib.h"
#include "lib/ikin.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
# define PI 3.14159265

using namespace matlib;

Vector FKIN(const Vector& Q){
  Vector s(2);
  s[0] = 1;
  s[1] = 0;
  return rot(Q[0])*s + rot(Q[1])*(rot(Q[0])*s);
}

int main(){
  srand(time(NULL));
  int n = 2;
  int m = 3;

  Vector goal = randPoint(sqrt(2));
  std::cout << "Goal: " << goal << std::endl; 
  goal[0]=.5;
  goal[1]=0;
  Vector Q0(2);
  Q0[1] = PI/2;
  
  Vector Qf = serialJacobian(&FKIN, Q0, goal);
  std::cout << "Qf: " << Qf<<std::endl;
  std::cout << "Xf: " <<FKIN(Qf)<<std::endl;
  //std::cout<<"Q"<<std::endl<<Q<<std::endl<<"X"<<std::endl<<FKIN(Q)<<std::endl;
}
  

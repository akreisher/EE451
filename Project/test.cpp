#include "lib/matlib.h"
#include <iostream>

using namespace matlib;

int main(){
  Matrix A(2,3,"Hello");

  
  A[0][0] = 3;
  A[1][0] = 5;
  

  Matrix B(3,2,"b");
  B[0][0] = 2;
  
  
  B[0][1] = 7;
  
  std::cout << A<<std::endl<<B<<std::endl;

  B = eyes(5,5);

  std::cout<<B;


  
}
  

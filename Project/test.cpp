#include "lib/matlib.h"
#include <iostream>
#include <cstdlib>

using namespace matlib;

int main(){

  int n = 4;
  int m = 6;
  
  
  Matrix A(n,m,"A");
  for (int i = 0; i<n; i++){
    for (int j = 0; j<m; j++){
      A[i][j] = rand()%100;
    }
  }
  Matrix B = MPInvert(A);
 
  
  std::cout<< A << B;
  
  Matrix res = A*B;
  res.setName("A *B");

  std::cout<<res;
  
}
  

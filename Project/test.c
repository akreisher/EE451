#include "lib/matlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

mat* FKin(mat* Q){
  double t1 = (double)Q->dat[0][0];
  double t2 = (double)Q->dat[0][1];
  return NULL;
}

int main(){
  
  printf("Test Output:\n");

  

  /*
  printf("Enter a value: ");
  char s[10];
  fgets(s,10,stdin);
  int n = atoi(s);
  printf("%i\n",n);
  int m = n;
  */
  int n = 2;
  int m = 4;
  
  mat* A = matAlloc(n,m);

  for (int i = 0;i<n;i++){
    for (int j = 0;j<m;j++){
      A->dat[i][j] = rand()%100;
    }
  }

  

  /*
  A->dat[0][0] = 1;
  A->dat[0][1] = 1;
  A->dat[0][2] = 1;
  A->dat[0][3] = 7;
  
  A->dat[1][0] = 2;
  A->dat[1][1] = 2;
  A->dat[1][2] = 5;
  A->dat[1][3] = 1;
  
  A->dat[2][0] = 4;
  A->dat[2][1] = 6;
  A->dat[2][2] = 8;
  A->dat[2][3] = -73;
  
  A->dat[3][0] = 2;
  A->dat[3][1] = -9;
  A->dat[3][2] = 2;
  A->dat[3][3] = -3;
  */
  
  
  
  mat* prod = matAlloc(n,n);
  matName(prod,"prod");




  mat* Ainv = matAlloc(m,n);
  matName(Ainv,"Ainverse:");
  matName(A,"A");

  matMPInverse(A,Ainv);
  
  matPrint(A);
  matPrint(Ainv);
  
  matMult(A,Ainv,prod);
  matPrint(prod);


  

  matFree(prod);

  
  

  matFree(Ainv);
  matFree(A);

 
  
}

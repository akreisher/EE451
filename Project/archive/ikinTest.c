#include "matlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265

struct mat* FKin(mat* Q){
  int n = 2;
  int m = Q->m;
  mat R [m];
  double t [m];
  mat* out = matAlloc(1,n);
  mat* norm = matAlloc(1,n);
  mat* temp = matAlloc(1,n);
  norm->dat[0][0] = 1;
  
  for (int i = 0; i < m; ++i) {
    R[i] = matAlloc(n,n);
    t[i] = Q->dat[0][i];

    //define rot matrix
    R[i].dat[0][0] = cos(t[i]);
    R[i].dat[0][1] = -sin(t[i]);
    R[i].dat[1][0] = sin(t[i]);
    R[i].dat[1][1] = cos(t[i]);
  }

  matMult(&R[0],temp,out);
  for (int i = 1; i < m; ++i) {
    matMult(&R[i],out,norm);
    matCopy(out,temp);
    matPlus(temp,norm,out);
  }

  matFree(temp);
  matFree(norm);
  for (int i = 0; i < m; ++i) {
    matFree(&R[i]);
  }

  return out;
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

  mat* Q = matAlloc(1,m);
  Q->dat[0][0] = 0;
  Q->dat[0][1] = 0;
  Q->dat[0][2] = 0;
  Q->dat[0][3] = 0;
  matName(Q,"Joint State");
  matPrint(Q);
  
  mat* X = FKin(Q);
  matName(X,"State");

  mat* G = matAlloc(1,n);
  matName(G,"Goal");
  G->dat[0][0] = 1.2;
  G->dat[0][1] = .3;

  mat* E = matAlloc(1,n);
  matName(E,"Error");
  matMinus(G,X,E);
  
  mat* J = matAlloc(n,m);
  matName(J,"Jacobian:");
  mat* Jinv = matAlloc(n,m);
  mat* dQ = matAlloc(Q->n,Q->m);
  mat* temp = matAlloc(Q->n,Q->m);
  while (vecNorm(E)> 0.004){

    //estimate jacobian
    for (int i = 0; i<Q->m;i++){
      mat* dX = matAlloc(1,G->m);
      float delta = 0.01;
      Q->dat[0][i] = Q->dat[0][i] + delta;
      mat* X2 = FKin(Q);
      Q->dat[0][i] = Q->dat[0][i] - delta;

      matMinus(X2,X,dX); 
      matScale(dX,1/delta);
      
      for (int j = 0; j<G->m;j++){
        J->dat[j][i] = dX->dat[0][j];
      }
      matFree(dX);
      matFree(X2);
    }
   
    matScale(E,0.1);
    matInvert(J,Jinv);
    matMultTranspose(Jinv,E,dQ);
    matScale(dQ,-1);
    
    matCopy(Q,temp);
    matMinus(temp,dQ,Q);
    matFree(X);
    X = FKin(Q);
    matMinus(G,X,E);

    
  }
  matFree(Jinv);
  matFree(dQ);
  matFree(temp);
  matName(Q,"Joints");
  matPrint(Q);
  matName(X,"FinalPos");
  matPrint(X);

  matPrint(G);
  matPrint(E);
  matFree(Q);
  matFree(X);
  matFree(G);
  matFree(E);
  matFree(J);
}

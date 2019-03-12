#include "matlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

float vecNorm(mat* A){
  if (A->n>1){
    printf("Cannot get norm of a matrix!");
    return -1;
  }
  
  float res = 0;
  for (int i = 0;i<A->m;i++){
    res += (A->dat[0][i])*(A->dat[0][i]);
  }
  return sqrt(res);
}

bool matEqualDim(mat* A, mat* B){
  return (A->n == B->n && A->m == B->m);
}

int  matMinus(mat* A, mat* B,mat* C){
  if (!matEqualDim(A,B)||!matEqualDim(A,C)){
    printf("Matricis to be subtracted must have equal dimensions!");
    return -1;    
  }
  for (int i = 0;i<A->n;i++){
    for (int j = 0;j<A->m;j++){
      C->dat[i][j] = A->dat[i][j]-B->dat[i][j];
    }
  }
  return 0;
}

int matPlus(mat* A,mat* B, mat* C){
   if (!matEqualDim(A,B)||!matEqualDim(A,C)){
    printf("Matricis to be subtracted must have equal dimensions!");
    return -1;    
  }
  for (int i = 0;i<A->n;i++){
    for (int j = 0;j<A->m;j++){
      C->dat[i][j] = A->dat[i][j]+B->dat[i][j];
    }
  }
  return 0;
}

void matScale(mat* A, float c){
    for (int i = 0;i<A->n;i++){
      for (int j = 0;j<A->m;j++){
	A->dat[i][j]=c*A->dat[i][j];
      }
    }
}

mat* matAlloc(int n, int m){
  mat* A = malloc(sizeof(*A));
  A->n = n;
  A->m = m;
  A->dat = calloc(n, sizeof(float*));
  for (int i = 0;i<n;i++){
    A->dat[i] = calloc(m, sizeof(float));
  }
  return A;
}



void matFree(mat* A){
  for (int i = 0;i<A->n;i++){
    free(A->dat[i]);
  }
  free(A->dat);
  free(A);
}

void matName(mat* A, char* name){
  strcpy(A->name, name);
}

int matTranspose(mat* A, mat* B){
  if (A->n!=B->m || A->m!=B->n) {
    printf("Incompatible matrix dimensions for transpose\n");
    return -1;
  }
  for (int i = 0;i<A->n;i++){
      for (int j = 0;j<A->m;j++){
	B->dat[j][i] = A->dat[i][j];
      }
  }
  return 0;
}

int matMult(mat* A, mat* B, mat* C){
  if (A->m!=B->n || A->n != C->n || B->m != C->m){
    printf("Incompatible matrix dimensions for multiplication\n");
    return -1;
  }
  for (int i = 0;i<A->n;i++){
    for (int j = 0;j<B->m;j++){
      C->dat[i][j] = 0;
      for (int k = 0;k<A->m;k++){
	C->dat[i][j] += A->dat[i][k]*B->dat[k][j];
      }
    }
  }
  return 0;
}

void matIdentity(mat* A){
  for (int i = 0; i<A->n;i++){
    for (int j = 0;j<A->m;j++){
      if (i==j) A->dat[i][j] = 1;
      else      A->dat[i][j] = 0;
    }
  }
}

int matCopy(mat* A, mat* B){
  if (A->n != B->n || A->m != B->m){
    printf("Invalid dimensions for copy");
  }
  for (int i = 0;i<A->n;i++){
    for (int j = 0;j<A->m;j++){
      B->dat[i][j]=A->dat[i][j];
    }
  }
}

void matLUP(mat* A, mat* L, mat* U, mat* P){
  matIdentity(L);
  matIdentity(P);
  matCopy(A,U);
  int m = A->m;
  //for each column
  for (int k = 0; k<m-1;k++){
    //get row with max element
    int maxInd = k;
    for (int i = k+1;i<m;i++){
      if (fabs(U->dat[i][k])>fabs(U->dat[maxInd][k])) maxInd = i;
    }

    //swap max row with kth row in U
    for (int i = k; i<m;i++){
      float temp = U->dat[k][i];
      U->dat[k][i] = U->dat[maxInd][i];
      U->dat[maxInd][i] = temp;
    }
    //swap rows in L
    for (int i = 0; i<k;i++){
      float temp = L->dat[k][i];
      L->dat[k][i] = L->dat[maxInd][i];
      L->dat[maxInd][i] = temp;
    }
    //swap rows in P
    float* temp = P->dat[k];
    P->dat[k]=P->dat[maxInd];
    P->dat[maxInd]=temp;

    //Facotorize remaining rows
    for (int j = k+1;j<m;j++){
      L->dat[j][k] = U->dat[j][k]/U->dat[k][k];
      for (int i = k; i<m;i++){
	U->dat[j][i] = U->dat[j][i] - L->dat[j][k]*U->dat[k][i];
      }
    }
  }    
}


int matLUInverse(mat* A,mat* B){
  int n = A->n;
  if (A->m != n || B->n !=n || B->m != n){
    printf("Incompatible dimensions for LU Inverse!");
    return -1;
  }
  mat* L = matAlloc(n,n);
  mat* U = matAlloc(n,n);
  mat* P = matAlloc(n,n);
  mat* I = matAlloc(n,n);
  mat* X = matAlloc(n,n);
  mat* Y = matAlloc(n,n);
  
  matIdentity(I);

  //LUP Decomp -> PA=LU
  matLUP(A,L,U,P);
  for (int i = 0;i<n;i++){
    if (L->dat[i][i] == 0 || U->dat[i][i] == 0){
      printf("L and U cannot contain 0 in diags");
      return -1;
    }
  }

  
  //Forward substitution -> L(X) = I, X = U(A^-1*P^-1)
  for (int i = 0; i<n;i++){
    for (int j = 0;j<n;j++){
      X->dat[i][j]= I->dat[i][j];
      for (int k = 0;k<i;k++){
	X->dat[i][j] -= L->dat[i][k]*X->dat[k][j];
      }
      X->dat[i][j] /= L->dat[i][i];
    }
  }
  
  //backward substitution -> U(Y) = X
  for (int i = n-1;i>=0;i--){
    for (int j = 0; j<n;j++){
      Y->dat[i][j] = X->dat[i][j];
      for (int k = i+1;k<n;k++){
	Y->dat[i][j] -= U->dat[i][k]*Y->dat[k][j];
      }
      Y->dat[i][j] /= U->dat[i][i];
    }
  }

  //results in B
  matMult(Y,P,B);
  
  matFree(Y);
  matFree(X);
  matFree(I);
  matFree(L);
  matFree(U);
  matFree(P);
}



int matInvert(mat* A, mat* B){
  if (A->n != B->n || A->n != B->m | A->m != A->n){

    printf("Incompatible matrix dimensions for transpose\n");
     return -1;
  }
  float det;
  switch (A->m){
  case 2 :
    det = (A->dat[0][0] * A->dat[1][1]) - (A->dat[0][1] *A->dat[1][0]);
    if (fabs(det)<(10^(-9))){
      printf("Matrix is near singular");
      return -1;
    }
   
    B->dat[0][0] = A->dat[1][1]/det;
    B->dat[1][1] = A->dat[0][0]/det;

    B->dat[0][1] = -A->dat[0][1]/det;
    B->dat[1][0] = -A->dat[1][0]/det;
    break;
  
  }
  return 0;
}

int matMultTranspose(mat* A, mat* B, mat* C){
  mat* BT = matAlloc(B->m,B->n);
  mat* CT = matAlloc(C->m,C->n);
  matTranspose(B,BT);
  int res = matMult(A,BT,CT);
  matTranspose(CT,C);
  matFree(BT);
  matFree(CT);
  return res;
}

int matMPInverse(mat* A, mat* B){
  if (A->n != B->m || A->m != B->n){
    printf("Incompatible dimensions for MP Inverse!");
    return -1;
  }
  mat* AT = matAlloc(A->m,A->n);
  matTranspose(A,AT);
  int res = 0;
  if (A->m>A->n){
    mat* AAT = matAlloc(A->n,AT->m);
    matMult(A,AT,AAT);
    
    mat* inv = matAlloc(A->n,AT->m);
    matLUInverse(AAT,inv);

    //res
    res = matMult(AT,inv,B);

    matFree(AAT);
    matFree(inv);
  }
  else if (A->m<A->n){
    mat* ATA = matAlloc(AT->n,A->m);
    matMult(AT,A,ATA);
    
    mat* inv = matAlloc(AT->n,A->m);
    matLUInverse(ATA,inv);

    //res
    
    res = matMult(inv,AT,B);
    
    matFree(inv);
    matFree(ATA);
  }
  else res = matLUInverse(A,B);

  matFree(AT);

  return res;
}


void matPrint(mat* A){
  printf("%s: \n",A->name);
  for (int i = 0; i<A->n;i++){
    for (int j = 0; j<A->m;j++){
      printf("%0.3f      ",A->dat[i][j]);
    }
    printf("\n");
  }
}

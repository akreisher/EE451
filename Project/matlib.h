

typedef struct mat {
  char name[20];
  int n,m;//rows,columns
  float** dat;
}mat;

//Allocate nxm matrix with name

mat* matAlloc(int n, int m);

int matInit(mat A);

void matName(mat* A, char* name);

//Frees memory from alloc
void matFree(mat* A);

//Transposes matrix A of size nxm to matrix b of size mxn
int matTranspose(mat* A, mat* B);

//Matrix Multiplication C(nxp)=A(nxm)*B(mxp)
int matMult(mat* A, mat* B, mat* C);

//creates an identity matrix in A
void matIdentity(mat* A);

int matCopy(mat* A, mat* B);

//Print matrix to stdout
void matPrint(mat* A);

void matLUP(mat* A, mat* L, mat* U, mat* P);

//Inverts nxn matrix B = A^-1 usigng LUP Decomposition
int matLUInverse(mat* A,mat* B);

int matInvert(mat* A, mat* B);

int matMPInverse(mat* A, mat* B);

void matScale(mat* A, float c);

float vecNorm(mat* A);


int matMinus(mat* A, mat* B, mat* C);

int matPlus(mat* A,mat* B, mat* C);

//multiply by transpose of B: (A*B^T)^T = C
int matMultTranspose(mat* A, mat* B, mat* C);



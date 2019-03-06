#include "matlib.h"
#include <cstdlib>
#include <cmath>
#include <iomanip>



using namespace matlib;





Vector::Vector(const Vector& A):Vector(A.n,A.name){
  this->alloc(A.n);
  if (A.data != nullptr){
    for (int i = 0;i<A.n;i++){
      this->data[i] = A.data[i];
    }
  }  
}

Vector& Vector::operator=(const Vector& rhs){
  if(*this == rhs) return *this;
  this->name = rhs.name;
  this->n = rhs.n;
  this->alloc(rhs.n);
  for (int i = 0;i<rhs.n;i++){
      this->data[i] = rhs.data[i];
  }
  return *this;
}

bool Vector::operator==(const Vector& rhs){
  if (this->n == rhs.n && this->data == rhs.data){
    return true;
  }
  else return false;
}


void Vector::alloc(int n){
  if (this->data != nullptr){
    this->free();
  }
  this->n = n;
  this->data = new double[n]();
}

void Vector::free(){
  if (this->data != nullptr){
    this->n = 0;
    delete [] this->data;
    this->data = nullptr;
  }
}

double& Vector::operator[](int c){
  if (c>=this->n || c<0){
    std::cerr << "Invalid read of index "<<c<<" for vector of size "<<this->n<<std::endl;
    exit(EXIT_FAILURE);
  }
  if (this->data == nullptr){
    std::cerr<< this->name<< " is not allocated!"<<std::endl;
    exit(EXIT_FAILURE);
  }

  return this->data[c];
}



Matrix::Matrix(const Matrix& A) : Matrix(A.n,A.m,A.name){
  for (int i = 0; i< A.n;i++){
    *(this->data[i]) = Vector(*(A.data[i]));
  }
}


Matrix& Matrix::operator=(const Matrix& rhs){
  if (*this == rhs) return *this;
  this->alloc(rhs.n,rhs.m);
  for (int i = 0; i< rhs.n;i++){
    *(this->data[i]) = Vector(*(rhs.data[i]));
  }
  return *this;
}

bool Matrix::operator==(const Matrix& rhs){
  if (this->n == rhs.n && this->m == rhs.m && this->data == rhs.data){
    return true;
  }
  else return false;
}


void Matrix::alloc(int n, int m){
  if (this->data != nullptr && this->n != n && this->m != m){
    this->free();
  }
  this->n = n;
  this->m = m;
  if (n!= 0 & m!=0){
    this->data = new Vector*[n];
    for (int i = 0; i<n;i++){
      this->data[i] = new Vector(m);
    }
  }
}

void Matrix::free(){
  if (this->data != nullptr){
    for (int i = 0; i<this->n;i++){
      delete data[i];
    }
    delete [] this->data;
    this->data = nullptr;
    this->n = 0;
    this->m = 0;
  }
}

Vector& Matrix::operator[](int r){
  if (this->data == nullptr){
    std::cerr<< this->name<< " is not allocated!"<<std::endl;
    exit(EXIT_FAILURE);
  }
  if (r>=this->n || r<0){
    std::cerr << "Invalid read of row "<<r<<" for matrix of size "<<this->n<<std::endl;
    exit(EXIT_FAILURE);
  }
  return *this->data[r];
}

Matrix Matrix::operator*(Matrix& B){
  Matrix C = Matrix(this->n,B.m);
  for (int i = 0;i<this->n;i++){
    for (int j = 0;j<B.m;j++){
      for (int k = 0; k<B.n;k++){
	C[i][j] += (*this)[i][k]*(B[k][j]);
      }
    }
  }
  return C;
}

Matrix Matrix::operator+(Matrix& B){
  //TODO Add error checking
  Matrix C = Matrix(this->n,this->m);
  for (int i = 0; i< this->n;i++){
    for (int j = 0; j< this->m;j++){
      C[i][j] = (*this)[i][j]+B[i][j];
    }
  }
  return C;
}

Matrix Matrix::operator-(Matrix& B){
  //TODO Add error checking
  Matrix C = Matrix(this->n,this->m);
  for (int i = 0; i< this->n;i++){
    for (int j = 0; j< this->m;j++){
      C[i][j] = (*this)[i][j]-B[i][j];
    }
  }
  return C;
}

Matrix& Matrix::operator+=(Matrix& B){
  //TODO Add error checking
  for (int i = 0; i< this->n;i++){
    for (int j = 0; j< this->m;j++){
      (*this)[i][j]+=B[i][j];
    }
  }
  return (*this);
}

Matrix& Matrix::operator-=(Matrix& B){
  //TODO Add error checking
  for (int i = 0; i< this->n;i++){
    for (int j = 0; j< this->m;j++){
      (*this)[i][j]-=B[i][j];
    }
  }
  return (*this);
}

Vector operator*(double c, Vector& V){
  Vector out = Vector(V);
  for (int i = 0; i<V.size();i++){
    out[i] = c*(out[i]);
  }
  return out;
}
Matrix operator*(double c, Matrix& A){
  Matrix out = Matrix(A);
  for (int i = 0; i<A.getN();i++){
    out[i] = c*out[i];
  }
  return out;
}



std::ostream& operator<<(std::ostream& o,  matlib::Vector& V){
  for (int i = 0; i<V.size();i++){
    o<<std::left<<std::setw(9)<<std::setfill(' ')<<std::setprecision(3);
    o<<(V[i])<<"   ";
  }
  return o;
}

std::ostream& operator<<(std::ostream& o, matlib::Matrix& A){
  if (A.getName() != "Matrix"){
    o<<A.getName()<<std::endl;
    for (int i =0;i<9*A.getM();i++) o<<'-';
    o<<std::endl;
  }
  for (int i = 0; i<A.getN();i++){
    o<<A[i]<<std::endl;
  }
  o<<std::endl;
  return o;
}

Matrix matlib::eyes(int n, int m){
  Matrix out = Matrix(n,m);
  for (int i = 0; i < n; i++){
    out[i][i] = 1;
  }
  return out;
}

Matrix matlib::LUP(Matrix& A, Matrix& L, Matrix& U){
  U = A;
  L = eyes(A.getN(),A.getM());
  Matrix P = L;
  int m = A.getM();
  
  for (int k = 0; k<m-1;k++){
    //get row with max element
    int maxInd = k;
    for (int i = k+1;i<m;i++){
      if (fabs(U[i][k])>fabs(U[maxInd][k])) maxInd = i;
    }
    
    //swap max row with kth row in U
    for (int i = k; i<m;i++){
      float temp = U[k][i];
      U[k][i] = U[maxInd][i];
      U[maxInd][i] = temp;
    }
    //swap rows in L
    for (int i = 0; i<k;i++){
      float temp = L[k][i];
      L[k][i] = L[maxInd][i];
      L[maxInd][i] = temp;
    }
    //swap rows in P
    Vector temp = P[k];
    P[k]=P[maxInd];
    P[maxInd]=temp;
    
    //Facotorize remaining rows
    for (int j = k+1;j<m;j++){
      L[j][k] = U[j][k]/U[k][k];
      for (int i = k; i<m;i++){
	U[j][i] = U[j][i] - L[j][k]*U[k][i];
      }
    }
  }
  return P;
}

Matrix matlib::LUInvert(Matrix& A){
  int n = A.getN();
  Matrix L,U,P;
  P=LUP(A,L,U);

  for (int i = 0;i<n;i++){
    if (L[i][i] == 0 || U[i][i] == 0){
      std::cerr<<"L and U cannot contain 0 in diags";
      exit(EXIT_FAILURE);
    }
  }

  Matrix X(n,n);
  Matrix Y(n,n);
  Matrix I = eyes(n,n);
  
  //Forward substitution -> L(X) = I, X = U(A^-1*P^-1)
  for (int i = 0; i<n;i++){
    for (int j = 0;j<n;j++){
      X[i][j]= I[i][j];
      for (int k = 0;k<i;k++){
	X[i][j] -= L[i][k]*X[k][j];
      }
      X[i][j] /= L[i][i];
    }
  }
  
  //backward substitution -> U(Y) = X
  for (int i = n-1;i>=0;i--){
    for (int j = 0; j<n;j++){
      Y[i][j] = X[i][j];
      for (int k = i+1;k<n;k++){
	Y[i][j] -= U[i][k]*Y[k][j];
      }
      Y[i][j] /= U[i][i];
    }
  }

  //results in B
  return Y*P;
}

Matrix matlib::transpose(Matrix& A){
  Matrix out = Matrix(A.getM(),A.getN());
  for (int i = 0; i<A.getN(); i++){
    for (int j = 0; j<A.getM();j++){
      out[j][i] = A[i][j];
    }
  }
  return out;
}

Matrix matlib::MPInvert(Matrix& A){
  Matrix out;
  Matrix AT = transpose(A);
  if (A.getM()>A.getN()){
    out = A*AT;
    out = LUInvert(out);
    out = AT*out;
  }

  return out;
}

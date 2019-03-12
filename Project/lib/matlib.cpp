#include "matlib.h"
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <string>

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
  if (this->size() != rhs.size()) this->alloc(rhs.n);
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

double Vector::magnitude() const{
  return sqrt((*this)*(*this));
}

double& Vector::operator[](int c){
  if (c>=this->n || c<0){
    throw(MatOpException("Invalid read!"));
  }
  if (this->data == nullptr){
    throw(MatOpException("Matrix not allocated!"));
  }
  return this->data[c];
}

const double& Vector::operator[] (int c) const{
  if (c>=this->n || c<0){
    throw(MatOpException("Invalid read!"));
  }
  if (this->data == nullptr){
    throw(MatOpException("Matrix not allocated!"));
  }
  return this->data[c];
}  
  

Vector Vector::operator+(const Vector& B) const{
  if (this->size() != B.size()) {
    throw MatOpException("Incompatible dimensions for vector addition");
  }
  Vector out(this->size());
  for (int i = 0; i<this->size(); i++){
    out[i] = (*this)[i]+B[i];
  }
  return out;
}

Vector Vector::operator-(const Vector& B) const{
  if (this->size() != B.size()) {
    throw MatOpException("Incompatible dimensions for vector subtraction");
  }
  Vector out(this->size());
  for (int i = 0; i<this->size(); i++){
    out[i] = (*this)[i]-B[i];
  }
  return out;
}

double Vector::operator*(const Vector& B) const{
  if (this->size() != B.size()) {
    throw MatOpException("Incompatible dimensions for vector product");
  }
  double out = 0;
  for (int i = 0; i<this->size(); i++){
    out += (*this)[i]*B[i];
  }
  return out;
}
    
    

Matrix::Matrix(const Matrix& A) : Matrix(A.n,A.m,A.name){
  for (int i = 0; i< A.n;i++){
    *(this->data[i]) = Vector(*(A.data[i]));
  }
}


Matrix& Matrix::operator=(const Matrix& rhs){
  if (*this == rhs) return *this;
  if (this->n != rhs.n) this->alloc(rhs.n,rhs.m);
  for (int i = 0; i< rhs.n;i++){
    (*this)[i] = *(rhs.data[i]);
  }
  return *this;
}

bool Matrix::operator==(const Matrix& rhs) const{
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

Vector& Matrix::operator[](int r) {
  if (this->data == nullptr){
    throw(MatOpException("Data not allocated"));
  }
  if (r>=this->n || r<0){
    throw(MatOpException("Invalid Read"));
  }
  return *this->data[r];
}

const Vector& Matrix::operator[](int r) const{
  if (this->data == nullptr){
    throw(MatOpException("Data not allocated"));
  }
  if (r>=this->n || r<0){
    throw(MatOpException("Invalid Read"));
  }
  return *this->data[r];
}

Matrix Matrix::operator*(const Matrix& B) const{
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

Matrix Matrix::operator+(const Matrix& B) const{
  if (this->getN() != B.getN() || this->getM() != B.getM()){
    throw MatOpException("Incompatible dimensions for matrix addition");
  }
  Matrix C = Matrix(this->n,this->m);
  for (int i = 0; i< this->n;i++){
    C[i] = (*this)[i]+B[i];
  }
  return C;
}

Matrix Matrix::operator-(const Matrix& B) const{
  if (this->getN() != B.getN() || this->getM() != B.getM()){
    throw MatOpException("Incompatible dimensions for matrix subtraction");
  }
  Matrix C = Matrix(this->n,this->m);
  for (int i = 0; i< this->n;i++){
    C[i] = (*this)[i]-B[i];
  }
  return C;
}



Matrix& Matrix::operator+=(const Matrix& B) {
  //TODO Add error checking
  for (int i = 0; i< this->n;i++){
    for (int j = 0; j< this->m;j++){
      (*this)[i][j]+=B[i][j];
    }
  }
  return (*this);
}

Matrix& Matrix::operator-=(const Matrix& B){
  //TODO Add error checking
  for (int i = 0; i< this->n;i++){
    for (int j = 0; j< this->m;j++){
      (*this)[i][j]-=B[i][j];
    }
  }
  return (*this);
}

void Matrix::print(){
  std::cout<<(*this);
}

Vector operator*(double c, const Vector& V){
  Vector out = Vector(V);
  for (int i = 0; i<V.size();i++){
    out[i] = c*(out[i]);
  }
  return out;
}

Matrix operator*(double c, const Matrix& A){
  Matrix out = Matrix(A);
  for (int i = 0; i<A.getN();i++){
    out[i] = c*out[i];
  }
  return out;
}

Vector operator*(const Matrix& A, const Vector& b){
  if (A.getM() != b.size()){
    throw MatOpException("Incompatible dimensions for matrix/vector multiplication");
  }
  Vector out(A.getN());
  for (int i = 0;i<A.getN();i++){
    out[i] = A[i]*b;
  }
  return out;
}
    

std::ostream& operator<<(std::ostream& o,  const matlib::Vector& V){
  for (int i = 0; i<V.size();i++){
    o<<std::left<<std::setw(9)<<std::setfill(' ')<<std::setprecision(3);
    o<<(V[i])<<"   ";
  }
  return o;
}

std::ostream& operator<<(std::ostream& o, const matlib::Matrix& A){
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

Vector matlib::magic(int n){
  Vector out(n);
  for (int i = 0; i<n;i++){
    out[i] = rand()%100;
  }
  return out;
}

Matrix matlib::magic(int n, int m){
  Matrix out(n,m);
  for (int i = 0; i<n;i++){
    out[i] = magic(m);
  }
  return out;
}
    

Matrix matlib::eyes(int n, int m){
  Matrix out = Matrix(n,m);
  for (int i = 0; i < n; i++){
    out[i][i] = 1;
  }
  return out;
}

Matrix matlib::LUP(const Matrix& A, Matrix& L, Matrix& U){
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

Matrix matlib::LUInvert(const Matrix& A){
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

Matrix matlib::transpose(const Matrix& A){
  Matrix out = Matrix(A.getM(),A.getN());
  for (int i = 0; i<A.getN(); i++){
    for (int j = 0; j<A.getM();j++){
      out[j][i] = A[i][j];
    }
  }
  return out;
}

Matrix matlib::MPInvert(const Matrix& A){
  Matrix out;
  Matrix AT = transpose(A);
  if (A.getM()>A.getN()){
    out = A*AT;
    out = LUInvert(out);
    out = AT*out;
  }
  if (A.getM() == A.getN()) out = LUInvert(A);

  return out;
}

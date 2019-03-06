#include "matlib.h"
#include <cstdlib>



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
  this->name = rhs.name;
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
  if (this->data != nullptr){
    this->free();
  }
  this->n = n;
  this->m = m;
  this->data = new Vector*[n];
  for (int i = 0; i<n;i++){
    this->data[i] = new Vector(m);
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
    o<<std::left<<std::setw(6)<<std::setfill(' ');
    o<<(V[i])<<"   ";
  }
  return o;
}

std::ostream& operator<<(std::ostream& o, matlib::Matrix& A){
  for (int i = 0; i<A.getN();i++){
    o<<A[i]<<std::endl;
  }
  return o;
}

Matrix eyes(int n, int m){
  Matrix out = Matrix(n,m);
  for (int i = 0; i < n; i++){
    out[i][i] = 1;
  }
  return out;
}


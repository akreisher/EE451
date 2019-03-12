#include <string>
#include <iostream>
#ifndef MATLIB_H
#define MATLIB_H

namespace matlib{

  class MatOpException : public std::exception{
    std::string msg;
  public:
    MatOpException(std::string msg){this->msg = msg;}
    const char* what() const throw(){return msg.c_str();}
  };
  
  class Vector
  {
  private:
    int n = 0;
    double* data = nullptr;
    std::string name = "Vector";
    void alloc(int n);
    void free();
  public:
    //constructors
    Vector () {}
    Vector(int n) {this->alloc(n);}
    Vector(int n, std::string name):Vector(n){this->name = name;}
    Vector(std::string name){this->name = name;}
    Vector(const Vector& A);
    Vector& operator=(const Vector& rhs);

    ~Vector(){this->free();}
    
    int size() const{return this->n;}
    double magnitude() const;

    //name getter and setter
    void setName(std::string name) {this->name = name;}
    std::string getName() const{return this->name;}

    //overloaded operators
    double& operator[](int c);
    const double& operator[](int c) const;
    Vector operator+(const Vector& B) const;
    Vector operator-(const Vector& B) const;
    double  operator*(const Vector& B) const;
    bool operator==(const Vector& rhs);
  };
    
  class Matrix
    {
    private:
      int n = 0, m = 0;     //rows x columns
      Vector** data  = nullptr;     //data
      std::string name = "Matrix";    //matrix name (for printing)

      //memory management
      void alloc(int n, int m);
      void free();
      
    public:
      
      //constructors
      Matrix(){alloc(0,0);}
      Matrix(int n, int m){alloc(n,m);}
      Matrix(int n, int m, std::string name):Matrix(n,m) {this->name = name;}
      Matrix(std::string name)  {this->name = name;}
      Matrix(const Matrix& A);
      Matrix& operator=(const Matrix& rhs);
      
      //deconstructor
      ~Matrix(){this->free();}
      
      //getter for n (num rows) and m (num columns)
      int getN() const{return n;}
      int getM() const{return m;}

      //name getter/setter
      void setName(std::string name) {this->name = name;}
      std::string getName() const {return this->name;}

      //operator overloads
      Vector& operator[](int r);
      const Vector& operator[](int r) const;
      Matrix& operator+=(const Matrix& B);
      Matrix& operator-=(const Matrix& B);
      Matrix operator*(const Matrix& B) const;
      Matrix operator+(const Matrix& B) const;
      Matrix operator-(const Matrix& B) const;
      bool operator==(const Matrix& rhs) const;

      void print();

    };
  Vector magic(int n);
  Matrix magic(int n, int m);
  //return nxm identity matrix 
  Matrix eyes(int n, int m);
  //LUP decomposition of A. Puts L and U by reference, returs permutation P
  Matrix LUP(const Matrix& A, Matrix& L, Matrix& U);
  //invert square matrix using LUP
  Matrix LUInvert(const Matrix& A);
  //transpose of A
  Matrix transpose(const Matrix& A);
  //Moore-Penrose Pseudo-Inverse
  Matrix MPInvert(const Matrix& A);
  
}


//Printing operators
std::ostream& operator<<(std::ostream& o,  const matlib::Vector& V);
std::ostream& operator<<(std::ostream& o, const matlib::Matrix& A);

//Scaling factor
matlib::Matrix operator*(double c, const matlib::Matrix& A);
matlib::Vector operator*(double c, const matlib::Vector& V);

matlib::Vector operator*(const matlib::Matrix& A, const matlib::Vector& b);

  
#endif

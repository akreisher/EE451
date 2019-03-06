#include <string>
#include <iostream>


namespace matlib{
  
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
    Vector(int n) {this->alloc(n);}
    Vector(int n, std::string name):Vector(n){this->name = name;}
    Vector(std::string name){this->name = name;}
    Vector(const Vector& A);
    Vector& operator=(const Vector& rhs);

    ~Vector(){this->free();}
    
    const int size(){return this->n;} 

    //name getter and setter
    void setName(std::string name) {this->name = name;}
    std::string getName() {return this->name;}

    //overloaded operators
    double& operator[](int c);
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
      int getN() {return n;}
      int getM() {return m;}

      //name getter/setter
      void setName(std::string name) {this->name = name;}
      std::string getName() {return this->name;}

      //operator overloads
      Vector& operator[](int r);
      Matrix& operator+=(Matrix& B);
      Matrix& operator-=(Matrix& B);
      Matrix operator*(Matrix& B);
      Matrix operator+(Matrix& B);
      Matrix operator-(Matrix& B);
      bool operator==(const Matrix& rhs);

      

    };
  //return nxm identity matrix 
  Matrix eyes(int n, int m);
  //LUP decomposition of A. Puts L and U by reference, returs permutation P
  Matrix LUP(Matrix& A, Matrix& L, Matrix& U);
  //invert square matrix using LUP
  Matrix LUInvert(Matrix& A);
  //transpose of A
  Matrix transpose(Matrix& A);
  //Moore-Penrose Pseudo-Inverse
  Matrix MPInvert(Matrix& A);
  
}


//Printing operators
std::ostream& operator<<(std::ostream& o,  matlib::Vector& V);
std::ostream& operator<<(std::ostream& o, matlib::Matrix& A);

//Scaling factor
matlib::Matrix operator*(double c, matlib::Matrix& A);
matlib::Vector operator*(double c, matlib::Vector& V);

  

#include <string>
#include <iostream>
#include <iomanip>

namespace matlib{
  
  class Vector
  {
  private:
    int         n    = 0;
    double*     data = nullptr;
    std::string name = "Vector";
  public:
    
    Vector(int n) {this->alloc(n);}
    Vector(int n, std::string name):Vector(n){this->name = name;}
    Vector(std::string name){this->name = name;}
    Vector(const Vector& A);
    
    Vector& operator=(const Vector& rhs);
    bool operator==(const Vector& rhs);
    
    ~Vector(){this->free();}

    const int size(){return this->n;} 
    void alloc(int n);
    void free();

    void setName(std::string name) {this->name = name;}
    std::string getName()          {return this->name;}

    double& operator[](int c);
    


    
  };
    
    class Matrix
    {
    private:
      int      n    = 0, m = 0;     //rows x columns
      Vector** data  = nullptr;     //data
      std::string   name = "Matrix";    //matrix name (for printing)

    public:
      
      //constructors
      Matrix(int n, int m){alloc(n,m);}
      Matrix(int n, int m, std::string name):Matrix(n,m) {this->name = name;}
      Matrix(std::string name)  {this->name = name;}
      Matrix(const Matrix& A);
      Matrix& operator=(const Matrix& rhs);

      bool operator==(const Matrix& rhs);
      
      //deconstructor
      ~Matrix(){this->free();}
      
      //getter for n (num rows) and m (num columns)
      int getN() {return n;}
      int getM() {return m;}

      void setName(std::string name) {this->name = name;}
      std::string getName()            {return this->name;}

      void alloc(int n, int m);
      void free();

  
      Vector &operator[](int r);
      Matrix operator*(Matrix& B);

      

    };

  
}

std::ostream& operator<<(std::ostream& o,  matlib::Vector& V);

std::ostream& operator<<(std::ostream& o, matlib::Matrix& A);

matlib::Matrix operator*(double c, matlib::Matrix& A);
matlib::Vector operator*(double c, matlib::Vector& V);

matlib::Matrix eyes(int n, int m);
  

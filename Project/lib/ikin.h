#include "matlib.h"
#include <string>

class ikinIterException : public std::exception {
  std::string msg;
  matlib::Vector lastIter = matlib::Vector();
 public:
  ikinIterException(std::string msg){this->msg = msg;}
  ikinIterException(std::string msg,matlib::Vector lastIter):ikinIterException(msg)
    {this->lastIter = lastIter;}
  
  const char* what() const throw(){return msg.c_str();}
};

matlib::Vector serialJacobian (matlib::Vector(*FK)(const matlib::Vector&), const matlib::Vector Q0, const matlib::Vector G);

matlib::Matrix rot(double theta);

matlib::Vector randPoint(double r);

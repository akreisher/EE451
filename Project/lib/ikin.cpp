#include "ikin.h"
#include <cmath>
#define PI 3.14159265

using namespace matlib;

Matrix rot(double theta){
  Matrix out(2,2);
  out[0][0] = cos(theta);
  out[0][1] =-sin(theta);
  out[1][0] = sin(theta);
  out[1][1] = cos(theta);
  return out;
}


Vector randPoint(double R){
  float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  double t = r1*2*PI;
  double r = R*sqrt(r2);
  Vector out(2);
  out[0] = r*cos(t);
  out[1] = r*sin(t);
  return out;
}

Vector serialJacobian (Vector(*FK)(const Vector&), const Vector Q0, const Vector G){
  //initial conditions
  int n = G.size();
  int m = Q0.size();
  
  Vector Q = Q0;
  Vector X = (*FK)(Q);
  Vector E = G - X;
  Matrix J = Matrix(n,m);
  double tol = 0.01;
  int iter = 0;
  while (E.magnitude() > tol){
    //estimate jacobian
    for (int i = 0; i<m; i++){
      double delta = 0.5;
      Q[i] = Q[i] + delta;
      Vector Jc = (1/delta)*((*FK)(Q)-X);
      Q[i] = Q[i] - delta;
      for (int j = 0; j<n;j++){
	J[j][i] = Jc[j];
      }
    }
    E = 0.5*E;
    Q = Q + MPInvert(J)*E;
    X = (*FK)(Q);
    E = G - X;
    if (iter > 10000){
      throw(ikinIterException("Reached max iter"));
    }
    iter++;
  }
  return Q;
}

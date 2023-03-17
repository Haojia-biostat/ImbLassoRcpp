#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double soft_thresh(
    double z,
    double gamma
    ) {
  
  if(gamma <= 0)
    stop("gamma should be positive");
  
  int sign_z;
  if(z > 0) 
    sign_z = 1;
  else if(z == 0)
    sign_z = 0;
  else if(z < 0)
    sign_z = -1;
  
  double abs_z = std::abs(z);
  double value;
  
  if(abs_z > gamma)
    value = abs_z - gamma;
  else
    value = 0.0;
  
  double S = sign_z * value;
  
  return S;
  
}


#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericVector logit_i(
    NumericMatrix X, 
    double b0,
    NumericVector b
) {
  
  int nobs = X.nrow();

  NumericVector logit_i(nobs);
  for(int i = 0; i < nobs; ++i) {
    logit_i[i] = sum(b0 + b*X.row(i));
    }

  return logit_i;
}



#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]

NumericVector prob_i(
    NumericVector logit
) {
  NumericVector p_i = 1/(1+exp(-1*logit));
  return p_i;
}

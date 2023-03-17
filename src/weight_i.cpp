
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]

NumericVector weight_i(
    NumericVector prob
) {
  NumericVector w_i = prob*(1-prob);
  return w_i;
}

#include <Rcpp.h>
#include <algorithm>

using namespace Rcpp;

// [[Rcpp::export]]
List stratifiedCpp(const NumericMatrix& X, const IntegerVector& y, const int size) {
  
  int n = X.nrow();
  int p = X.ncol();
  
  NumericMatrix X_resampled(size, p);
  IntegerVector y_resampled(size);
  
  int index = 0;
  for (int label : unique(y)) {
    int count_label = std::count(y.begin(), y.end(), label);
    int count_resampled = std::min(size / (int)unique(y).size(), count_label);
    NumericMatrix X_label(n, p);
    int j = 0;
    for (int i = 0; i < n; i++) {
      if (y[i] == label) {
        X_label(j, _) = X(i, _);
        j++;
      }
    }
    IntegerVector indices = seq(0, count_label - 1);
    std::random_shuffle(indices.begin(), indices.end());
    indices = indices[seq(0, count_resampled - 1)];
    for (int i : indices) {
      X_resampled(index, _) = X_label(i, _);
      y_resampled(index) = label;
      index++;
    }
  }
  
  return List::create(Named("data") = X_resampled, Named("classes") = y_resampled);
}

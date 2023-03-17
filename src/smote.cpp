#include <Rcpp.h>
#include <algorithm>

using namespace Rcpp;

// [[Rcpp::export]]
NumericMatrix smoteCpp(const NumericMatrix& X, const IntegerVector& y, const int k, const int m) {
  
  int n = X.nrow();
  int p = X.ncol();
  
  NumericMatrix X_resampled(n*m, p);
  IntegerVector y_resampled(n*m);
  
  int index = 0;
  for (int i = 0; i < n; i++) {
    
    if (y(i) == 1) {
      X_resampled(index, _) = X(i, _);
      y_resampled[index] = y(i);
      index++;
    } else {
      NumericVector x(p);
      for (int j = 0; j < p; j++) {
        x[j] = X(i, j);
      }
      NumericMatrix nn(n, 1);
      for (int j = 0; j < n; j++) {
        double dist = 0;
        for (int l = 0; l < p; l++) {
          dist += pow(X(j, l) - x[l], 2);
        }
        nn(j, 0) = sqrt(dist);
      }
      IntegerVector k_nn_indices = nn(_, 0).std::sort_indexes()[Range(0, k)];
      for (int j = 0; j < m; j++) {
        int index_nn = k_nn_indices[rand() % k];
        NumericVector x_nn(p);
        for (int l = 0; l < p; l++) {
          x_nn[l] = X(index_nn, l);
        }
        NumericVector x_new(p);
        for (int l = 0; l < p; l++) {
          x_new[l] = x[l] + (x_nn[l] - x[l]) * (double)(rand() % 101) / 100.0;
        }
        X_resampled(index, _) = x_new;
        y_resampled[index] = y(i);
        index++;
      }
    }
  }
  
  NumericMatrix resampled_data(index, p + 1);
  for (int i = 0; i < index; i++) {
    resampled_data(i, _) = c(X_resampled(i, _), y_resampled[i]);
  }
  
  return resampled_data;
}

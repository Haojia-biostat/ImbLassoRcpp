#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]

//'SMOTE algorithm for imbalanced binary outcome
//'
//'This function generates synthetic examples for the minority class by kNN and interpolation
//'
//'@name SMOTE
//'
//'@param X feature matrix
//'@param k number of nearest neighbors to be considered, default value is 5
//'@param N number of new synthetic examples to be generated for each observation, default value is 9
//'
//'@return A matrix of
//'
//'@examples
//'SMOTE(matrix(rnorm(100), 10))
//'
//'@export

NumericMatrix SMOTE(
    const NumericMatrix& X,
    const int& k = 5,
    const int& N = 9
) {
  int n = X.nrow();
  int p = X.ncol();

  if(k > n-1)
    stop("k (number of nearest neighbors) is larger than the number of observations - 1. Please select a smaller k value.");

  // initialize matrix of synthetic examples
  NumericMatrix new_synthetic_examples(n*N, p);
  // initialize row index of output matrix
  int row_ind = 0;
  // initialize vector of random number between 0 and 1
  NumericVector rand_vec = runif(n*N);

  for(int i = 0; i < n; ++i) {

    NumericVector x = X(i, _);

    // matrix of difference
    NumericMatrix difference(n, p);
    // vector of Euclidean distance
    NumericVector euclidean_vec(n);

    for(int j = 0; j < n; ++j) {

      // initialize cumulative sum of squared difference
      double cum_sqdiff = 0;

      for(int k = 0; k < p; ++k) {
        difference(j,k) = X(j,k) - x[k];
        cum_sqdiff += pow(X(j,k) - x[k], 2);
      }

      euclidean_vec[k] = sqrt(cum_sqdiff);
    }

    // sort indices by Euclidean distance
    IntegerVector sorted_ind = seq(0, n-1);
    std::sort(sorted_ind.begin(), sorted_ind.end(), [&euclidean_vec](int a, int b) { return euclidean_vec[a] < euclidean_vec[b]; });

    // select top k indices
    IntegerVector k_seq = seq(1, k);
    IntegerVector knn_ind = sorted_ind[k_seq];

    // sample N from k
    IntegerVector synN_ind;
    if(N <= k)
      synN_ind = sample(knn_ind, N, false);
    else
      synN_ind = sample(knn_ind, N, true);

    for(int m = 0; m < N; ++m) {

      for(int k = 0; k < p; ++k) {
        // generate synthetic example by interpolation
        new_synthetic_examples(row_ind, k) = x[k] + rand_vec[row_ind] * difference(synN_ind[m], k);
      }

      row_ind += 1;
    }

  }

  return new_synthetic_examples;

}

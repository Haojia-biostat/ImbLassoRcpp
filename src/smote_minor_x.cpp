
#include <Rcpp.h>
using namespace Rcpp;

//'SMOTE algorithm for minority class
//'
//'This function generates synthetic examples for the minority class by kNN and interpolation
//'
//'Add details
//'
//'@name smote_minor_x
//'
//'@param X feature matrix
//'@param k number of nearest neighbors to be considered, default value is 5
//'@param N number of new synthetic examples to be generated for each observation, default value is 9
//'
//'@return A list including new synthetic examples with \code{N*nrow(X)} rows and \code{ncol(X)} columns.
//'
// '@examples
// 'smote_minor_x(X = matrix(rnorm(100), 10))
//'
//'@export

// [[Rcpp::export]]
List smote_minor_x(
  const NumericMatrix& X,
  const int& k = 5,
  const int& N = 9
) {
  int n = X.nrow();
  int p = X.ncol();

  if(k > n-1)
   stop("k (number of nearest neighbors) is larger than the number of observations - 1. Please select a smaller k value.");

  // initialize matrix of Euclidean distance
  NumericMatrix euclidean_distance(n, n);
  // initialize matrix of kNN
  IntegerMatrix k_nearest_neighbors(n, k);

  // initialize matrix of synthetic examples
  NumericMatrix synthetic_examples(n*N, p);
  // initialize row index of output matrix
  int synthetic_ind = 0;
  // initialize vector of random number between 0 and 1
  NumericVector random_num = runif(n*N);

  for(int i = 0; i < n; ++i) {

    NumericVector x = X(i, _);

    // initialize matrix of difference
    NumericMatrix difference(n, p);
    // initialize vector of Euclidean distance
    NumericVector euclidean_distance_i(n);

    for(int j = 0; j < n; ++j) {

      // initialize cumulative sum of squared difference
      double cum_sqdiff = 0;

      for(int k = 0; k < p; ++k) {
        difference(j,k) = X(j,k) - x(k);
        cum_sqdiff += pow(X(j,k) - x(k), 2);
      }

      euclidean_distance_i(j) = sqrt(cum_sqdiff);
    }

    euclidean_distance(i, _) = euclidean_distance_i;

    // sort indices by Euclidean distance
    IntegerVector distance_sorted_ind = seq(0, n-1);
    std::sort(
      distance_sorted_ind.begin(), distance_sorted_ind.end(),
      [&euclidean_distance_i](int a, int b) { return euclidean_distance_i(a) < euclidean_distance_i(b); }
    );

    // select top k indices
    IntegerVector k_seq = seq(1, k);
    IntegerVector knn_ind = distance_sorted_ind[k_seq];
    k_nearest_neighbors(i, _) = knn_ind + 1;

    // sample N from k neighbors
    IntegerVector nnn_ind;
    if(N <= k)
      nnn_ind = sample(knn_ind, N, false);
    else
      nnn_ind = sample(knn_ind, N, true);

    for(int m = 0; m < N; ++m) {

      for(int k = 0; k < p; ++k) {
        // generate synthetic example by interpolation
        synthetic_examples(synthetic_ind, k) = x(k) + random_num[synthetic_ind] * difference(nnn_ind[m], k);
      }

      synthetic_ind += 1;
    }

  }

  return List::create(
    _["X"] = X,
    _["k"] = k,
    _["N"] = N,
    _["kNN"] = k_nearest_neighbors,
    _["Euclidean"] = euclidean_distance,
    _["Synthetic"] = synthetic_examples
  );

}

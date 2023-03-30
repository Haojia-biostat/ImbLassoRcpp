
#include <Rcpp.h>
using namespace Rcpp;

//'Apply SMOTE algorithm to the X of minority class
//'
//'This function generates new observations for the X of minority class in the binary outcome
//'using the SMOTE (i.e., synthetic minority over-sampling technique) algorithm.
//'
//'For each of the observations in minority class, \code{k} nearest neighbors will be defined
//'by Euclidean distance, and the following steps will be repeated for \code{N} times:
//'
//'1. Randomly draw one of the k nearest neighbors;
//'
//'2. Take difference between the observation and the neighbor;
//'
//'3. Multiply the difference by a random number in \code{(0,1]};
//'
//'4. Add this difference to the observation to generate a new synthetic example in feature space;
//'
//'5. If \code{k < N}, put the neighbor back to the sampling pool.
//'
//'@name smote_minor_x
//'
//'@param X feature matrix
//'@param k number of nearest neighbors to be identified, default value is 5
//'@param N number of new synthetic examples to be generated for each observation, default value is 9
//'
//'@return A list including:
//'\item{kNN}{matrix of the row index of nearest neighbors for each observation in \code{X}
//'with \code{nrow(X)} rows and \code{k} columns}
//'\item{Euclidean}{matrix of Euclidean distance with \code{nrow(X)} rows and columns}
//'\item{Synthetic}{matrix of new synthetic examples with \code{nrow(X)*N} rows and \code{ncol(X)} columns}
//'
//'@references
//'\url{https://medium.com/@corymaklin/synthetic-minority-over-sampling-technique-smote-7d419696b88c}
//'
//'@examples
//'X <- matrix(rnorm(50), ncol = 10)
//'smote_minor_x(X, k = 3, N = 2)
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

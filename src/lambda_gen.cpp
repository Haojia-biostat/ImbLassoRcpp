
#include <Rcpp.h>
using namespace Rcpp;

//'calculate the entry level of lambda
//'
//'Calculate the smallest value for which all the beta equal to 0.
//'
//'@name lambda_gen
//'
//'@param X input matrix, of dimension nobs x nvars; each row is an observation vector.
//'@param y binary response variable, positive outcome = 1 and negative outcome = 0.
//'@param K number of lambdas, by default is 100.
//'
//'@return A number
//'
// '@examples
// 'lambda_gen(matrix(rnorm(1000), ncol = 10), rbinom(100, 1, 0.1))
//'
//'@export

// [[Rcpp::export]]
List lambda_gen(
     NumericMatrix X,
     NumericVector y,
     int K = 100
) {
  if(y.size() != X.nrow())
    stop("The length of outcome y is different from the number of rows of matrix X.");

  int nobs = X.nrow();
  int nvars = X.ncol();

  double lambda_max = 0;
  for(int i = 0; i < nvars; ++i) {
    double dotprod = sum(X.column(i) * y);
    if(std::abs(dotprod/nobs) > lambda_max)
      lambda_max = std::abs(dotprod/nobs);
    else
      lambda_max = lambda_max;
  }

  double lambda_min_ratio;
  if(nobs > nvars)
    lambda_min_ratio = 1e-4;
  else
    lambda_min_ratio = 1e-2;

  NumericVector lambda_seq(K);
  for(int i = 0; i < K; ++i) {
    lambda_seq[i] = std::exp(log(lambda_max) + i*log(lambda_min_ratio)/(K-1));
  }

  // Returning
  return List::create(
    _["Entry value"] = lambda_max,
    _["Minimum ratio"] = lambda_min_ratio,
    _["Lambdas"] = lambda_seq
  );
}


#include <Rcpp.h>
using namespace Rcpp;

//'Generate the lambda sequence for LASSO
//'
//'This function produces a decreasing lambda sequence with a length of \code{K} for LASSO,
//'based on the feature matrix \code{X} and binary outcome \code{y}.
//'
//'@name lambda_gen
//'
//'@param X feature matrix
//'@param y binary outcome, where 1 = positive event and 0 = negative event
//'@param K number of lambdas, default value is 100
//'
//'@return A list including:
//'\item{Entry value}{\eqn{\lambda_{max}}, the smallest value for which all coefficients are zero}
//'\item{Minimum ratio}{\eqn{\lambda_{min}} as a fraction of \eqn{\lambda_{max}}}
//'\item{Lambdas}{\code{K} values of \eqn{\lambda} decreasing from \eqn{\lambda_{max}} to \eqn{\lambda_{min}} on the log scale}
//'
//'@references
//'Friedman, J., Hastie, T. and Tibshirani, R. (2008)
//'\emph{Regularization Paths for Generalized Linear Models via Coordinate Descent (2010),
//'Journal of Statistical Software, Vol. 33(1), 1-22},
//'\doi{10.18637/jss.v033.i01.}
//'
//'@examples
//'X <- matrix(rnorm(1000), ncol = 10)
//'y <- rbinom(100, 1, 0.1)
//'lambda_gen(X, y, K = 15)
//'
//'@export
//'
// [[Rcpp::export]]
List lambda_gen(
     const NumericMatrix& X,
     const NumericVector& y,
     const int& K = 100
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

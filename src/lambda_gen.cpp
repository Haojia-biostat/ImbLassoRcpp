
#include <Rcpp.h>
using namespace Rcpp;

//'Generate the lambda sequence for LASSO regression
//'
//'This function computes the solutions for a decreasing sequence of values for \eqn{\lambda},
//'starting at the smallest value \eqn{\lambda_{max}} for which the entire vector \eqn{\hat{\beta} = 0}.
//'Then the minimum value \eqn{\lambda_{min}} is determined as \eqn{\epsilon\lambda_{max}},
//'where \eqn{\epsilon = 0.0001} if \code{nobs > nvars}, otherwise \eqn{\epsilon = 0.01}.
//'A sequence of \eqn{K} values of \eqn{\lambda} decreasing from \eqn{\lambda_{max}} to \eqn{\lambda_{min}} on the log scale.
//'
//'@name lambda_gen
//'
//'@param X input matrix, of dimension nobs x nvars; each row is an observation vector.
//'@param y binary response variable, positive outcome = 1 and negative outcome = 0.
//'@param K number of lambdas, by default is 100.
//'
//'@return A list including:
//'\item {Entry value} {\eqn{\lambda_{max}}}
//'\item {Minimum ratio} {\eqn{\epsilon}}
//'\item {Lambdas} {\eqn{K} values of \eqn{\lambda} decreasing from \eqn{\lambda_{max}} to \eqn{\lambda_{min}} on the log scale}
//'
//'@references
//'Friedman, J., Hastie, T. and Tibshirani, R. (2008)
//'\emph {Regularization Paths for Generalized Linear Models via Coordinate Descent (2010),
//'Journal of Statistical Software, Vol. 33(1), 1-22},
//'\doi{}10.18637/jss.v033.i01.}
//'
//'@export

// '@examples
// 'lambda_gen(matrix(rnorm(1000), ncol = 10), rbinom(100, 1, 0.1))
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

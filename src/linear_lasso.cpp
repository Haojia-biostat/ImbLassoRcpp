#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
using namespace Rcpp;
using namespace arma;

//'run the coordinate descent algorithm on the penalized weighted-least-squares
//'
//'Update the quadratic approx
//'
//'@name single_coord_des
//'
//'@param X input matrix, of dimension nobs x nvars; each row is an observation vector.
//'@param y binary response variable, positive outcome = 1 and negative outcome = 0.
//'@param lambda A user supplied lambda
//'@param thresh Convergence threshold for coordinate descent. Each inner coordinate-descent loop continues until the maximum change in the objective after any coefficient update is less than thresh times the null deviance. Defaults value is 1E-7.
//'@param maxit Maximum number of passes over the data for all lambda values; default is 10^5.
//'
// [[Rcpp::export]]
mat betamat(
    NumericMatrix X,
    NumericVector y,
    double lambda,
    double thresh = 1e-7,
    int maxit = 1e5
) {
  if(y.size() != X.nrow())
    stop("The length of outcome y is different from the number of rows of matrix X.");

  int nobs = X.nrow();
  int nvars = X.ncol();

  mat X1(X.begin(), nobs, nvars, false);
  colvec y1(y.begin(), y.size(), false);
  // vec lambda(lambdar.begin(), lambdar.size(),false);
  // colvec ols = solve(X1,y1);
  mat betas = zeros<mat>(nvars,l);
  //
  bool converged = false;
  for (int i = 0; i < l; ++i) {
    colvec b = zeros<vec>(nvars);
    colvec r = y1-X1*b;
    while(converged == false){
      colvec beta_old = betas;
      for(int j = 0; j < nvars; ++j){
        r = r + X.col(j)*b(j);
        double X = dot(X.col(j),r);
        double S1 = X/nobs;
        double xx = sum(X.col(j)%X.col(j))/nobs;
        b(j) =((S1>0)-(S1<0))*(abs(S1)-lambda(i))/xx;
        if(b(j)>0){
          b(j)=b(j);
        }else{
          b(j)=0;
        }
        r = r - X.col(j)*b(j);
      }
      converged = (sum(abs(betas - beta_old)) < tol);
    }
    betas.col(i) = b;
  }
  return betas;
}

// // [[Rcpp::depends(RcppArmadillo)]]
//
// #include <RcppArmadillo.h>
//
// using namespace Rcpp;
// using namespace arma;
//
// //'run the coordinate descent algorithm on the penalized weighted-least-squares
// //'
// //'Update the quadratic approx
// //'
// //'@param Xr x matrix
// //'@param yr outcome
// //'@param lambdar lambda applied by user
// //'@param tol threshold
// //'
// //'@export
//
// // [[Rcpp::export]]
// arma::mat linear_lasso(
//     NumericMatrix Xr,
//     NumericVector yr,
//     NumericVector lambdar,
//     double tol=0.0000001
// ) {
//   int N = Xr.nrow(), p = Xr.ncol(), l = lambdar.size();
//   mat X(Xr.begin(), N, p, false);
//   colvec y(yr.begin(), yr.size(), false);
//   vec lambda(lambdar.begin(), lambdar.size(),false);
//   colvec ols = solve(X,y);
//   mat betas = zeros<mat>(p,l);
//   //
//   bool converged = false;
//   for (int i = 0; i < l; ++i) {
//     colvec b = zeros<vec>(p);
//     colvec r = y-X*b;
//     while(converged == false){
//       colvec beta_old = betas;
//       for(int j = 0; j < p; ++j){
//         r = r + X.col(j)*b(j);
//         double xr = dot(X.col(j),r);
//         double S1 = xr/N;
//         double xx = sum(X.col(j)%X.col(j))/N;
//         b(j) =((S1>0)-(S1<0))*(abs(S1)-lambda(i))/xx;
//         if(b(j)>0){
//           b(j)=b(j);
//         }else{
//           b(j)=0;
//         }
//         r = r - X.col(j)*b(j);
//       }
//       converged = (sum(abs(betas - beta_old)) < tol);
//     }
//     betas.col(i) = b;
//   }
//   return betas;
// }

#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
void linear_lasso() {
  Rcpp::Rcout << "This function is still a work in progress and is not yet fully functional." << std::endl;
}

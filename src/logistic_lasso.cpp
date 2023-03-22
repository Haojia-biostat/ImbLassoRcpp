// #include <Rcpp.h>
// using namespace Rcpp;
//
// // [[Rcpp::export]]
// NumericVector logit_i(
//     NumericMatrix X,
//     double b0,
//     NumericVector b
// ) {
//
//   int nobs = X.nrow();
//
//   NumericVector logit_i(nobs);
//   for(int i = 0; i < nobs; ++i) {
//     logit_i[i] = sum(b0 + b*X.row(i));
//   }
//
//   return logit_i;
// }
//
//
// #include <Rcpp.h>
// using namespace Rcpp;
//
// // [[Rcpp::export]]
//
// NumericVector prob_i(
//     NumericVector logit
// ) {
//   NumericVector p_i = 1/(1+exp(-1*logit));
//   return p_i;
// }
//
//
// #include <Rcpp.h>
// using namespace Rcpp;
//
// // [[Rcpp::export]]
//
// NumericVector weight_i(
//     NumericVector prob
// ) {
//   NumericVector w_i = prob*(1-prob);
//   return w_i;
// }
//
// #include <Rcpp.h>
// using namespace Rcpp;
//
// // [[Rcpp::export]]
// double soft_thresh(
//     double z,
//     double gamma
// ) {
//
//   if(gamma <= 0)
//     stop("gamma should be positive");
//
//   int sign_z;
//   if(z > 0)
//     sign_z = 1;
//   else if(z == 0)
//     sign_z = 0;
//   else if(z < 0)
//     sign_z = -1;
//
//   double abs_z = std::abs(z);
//   double value;
//
//   if(abs_z > gamma)
//     value = abs_z - gamma;
//   else
//     value = 0.0;
//
//   double S = sign_z * value;
//
//   return S;
//
// }
//
// #include <Rcpp.h>
// using namespace Rcpp;
//
// // [[Rcpp::export]]
// NumericVector coord_desc(
//     NumericMatrix X,
//     NumericVector y,
//     double lambda,
//     double thresh = 1e-7,
//     int maxit = 1e5
// ) {
//   if(y.size() != X.nrow())
//     stop("The length of outcome y is different from the number of rows of matrix X.");
//
//   int nobs = X.nrow();
//   int nvars = X.ncol();
//
//   // Initializing beta
//   double b0 = 0.0;
//   NumericVector b(nvars);
//
//   double conv = 1;
//   while(conv > thresh) {
//     for(int k = 0; k < maxit; ++k) {
//       NumericVector logit_k(nobs);
//       for(int i = 0; i < nobs; ++i) {
//         logit_k[i] = sum(b0 + b*X.row(i));
//       }
//       NumericVector prob_k = 1/(1+exp(-1*logit_k));
//
//       NumericVector weight_k = prob_k*(1-prob_k);
//
//       NumericVector delta_z(nobs);
//       NumericVector delta_logit(nobs);
//       NumericVector x(nobs);
//
//       NumericVector old_b(nvars);
//
//       for(int j = 0; j < (nvars+1); ++j) {
//         if(j == 0) {
//           delta_logit.fill(b0);
//           x.fill(1);
//         }
//         else {
//           delta_logit = b[j-1]*X.column(j-1);
//           x = X.column(j-1);
//         }
//
//         for(int i = 0; i < nobs; ++i) {
//           if(y[i] == 1)
//             delta_z[i] = delta_logit + exp(-1*logit_k) * (1-exp(delta_logit));
//           else
//             delta_z[i] = delta_logit - exp(logit_k) * (1-exp(delta_logit));
//         }
//
//         double s;
//         s = sum(weight_k * x * delta_z);
//         if(j == 1) {
//           double old_b0 = b0;
//           b0 = soft_thresh(s, lambda)/sum(weight_k * x * x);
//         }
//         else {
//           old_b[j-1] <- b[j-1];
//           b[j-1] <- soft_thresh(s, lambda)/sum(weight_k * x * x);
//         }
//       }
//
//       conv = sum(std::abs(old_b - b)) + std::abs(old_b0 - b0);
//     }
//   }
//
//   return b;
// }

#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
void logistic_lasso() {
  Rcpp::Rcout << "This function is still a work in progress and is not yet fully functional." << std::endl;
}

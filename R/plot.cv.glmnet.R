#' plot the cross-validation curve produced by par_smote_cv_lasso
#'
#' This functions plots the cross-validation curve, and upper and lower standard deviation curves.
#'
#' @param fitmat fitted \code{smote_cv_mseMatrix} object
#'
#' @export

plot.cv.glmnet <- function(fitmat) {
  x <- fitmat$lambda
  y <- colMeans(fitmat[,-1])
  plot(x, y, type = "l")
}

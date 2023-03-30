#' plot the cross-validation curve produced by par_smote_cv_lasso
#'
#' This functions plots the cross-validation curve, and upper and lower standard deviation curves.
#'
#' @param x fitted \code{smote_cv_mseMatrix} object
#' @param \dots other arguments that can be passed to \code{plot}
#'
#' @export

plot.cv.glmnet <- function(x, ...) {
  x <- x[,1]
  y <- rowMeans(x[,-1])
  plot(x, y, type = "l")
}

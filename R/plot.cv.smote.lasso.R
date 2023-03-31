#' Plot the parameter tuning result
#'
#' This functions plots \eqn{log(\lambda)} on x-axis and \code{deviance} on y-axis, \
#' with two verticle dashed lines for \code{lambda.min} and \code{lambda.1se}
#'
#' @param x fitted \code{cv_smote_lasso} object
#' @param \dots other arguments that can be passed to \code{plot}
#'
#' @export plot.cv.smote.lasso
#' @export

plot.cv.smote.lasso <- function(x, ...) {
  loglambda <- log(x$lambda)
  dev <- rowMeans(x$deviance)
  lambda_min <- loglambda[which.min(dev)]
  dev_1se <- min(dev) + stats::sd(dev)/sqrt(length(dev))
  lambda_1se_ind <- order(abs(dev - dev_1se))
  lambda_1se <- loglambda[lambda_1se_ind[lambda_1se_ind < which.min(dev)][1]]
  graphics::plot(loglambda, dev, xlab = "log(lambda)", ylab = "deviance", ...)
  graphics::abline(v = lambda_min, lty = 2)
  graphics::abline(v = lambda_1se, lty = 2)
}

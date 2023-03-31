#' Print the essential results for parameter tuning
#'
#' This functions prints the results of parameter tuning from cross-validation
#'
#' @param x fitted \code{cv_smote_lasso} object
#' @param \dots other arguments that can be passed to \code{print}
#'
#' @export print.cv.smote.lasso
#' @export

print.cv.smote.lasso <- function(x, ...) {

  lambda <- x$lambda
  dev <- rowMeans(x$deviance)
  lambda_min <- lambda[which.min(dev)]
  dev_1se <- min(dev) + stats::sd(dev)/sqrt(length(dev))
  lambda_1se_ind <- order(abs(dev - dev_1se))
  lambda_1se <- lambda[lambda_1se_ind[lambda_1se_ind < which.min(dev)][1]]

  res <- vector(mode = "list", length = 3)
  names(res) <- c("lambda input", "lambda tuned", "deviance")

  res$`lambda input` <- range(lambda)
  res$`lambda tuned` <- c("lambda_min" = lambda_min, "lambda_1se" = lambda_1se)
  res$deviance <- c("min" = min(dev), "min+1se" = dev_1se)

  print(res, ...)
}

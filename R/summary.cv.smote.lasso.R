#' Summarize parameter tuning results
#'
#' This functions prints the results of parameter tuning from cross-validation
#'
#' @param object fitted \code{cv_smote_lasso} object
#' @param s penalty parameter, default value is lambda.1se
#' @param \dots Additional arguments affecting the summary produced
#'
#' @export summary.cv.smote.lasso
#' @export

summary.cv.smote.lasso <- function(
    object,
    s = "lambda.1se",
    ...
) {
  x <- object
  lambda <- x$lambda
  dev <- rowMeans(x$deviance)
  lambda_min <- lambda[which.min(dev)]
  if(s == "lambda.min")
    sval = lambda_min
  else {
    dev_1se <- min(dev) + stats::sd(dev)/sqrt(length(dev))
    lambda_1se_ind <- order(abs(dev - dev_1se))
    sval <- lambda[lambda_1se_ind[lambda_1se_ind < which.min(dev)][1]]
  }

  glmnet.fit <- glmnet::glmnet(x$X, x$y, lambda = sval, family = "binomial")
  glmnet.coef <- stats::coef(glmnet.fit, s = sval)[,1]
  feature_selected <- names(glmnet.coef)[glmnet.coef != 0]
  if(feature_selected[1] == "(Intercept)") feature_selected <- feature_selected[-1]

  glm.fit <- stats::glm(stats::as.formula(paste0("y ~", paste(feature_selected, collapse = "+"))), data = data.frame(x$X, y = x$y), family = "binomial")

  res <- vector(mode = "list", length = 4)
  names(res) <- c("lambda", "penalized regression", "feature selected", "unpenalized regression")
  res$lambda <- sval
  res$`penalized regression` <- glmnet.fit
  res$`feature selected` <- feature_selected
  res$`unpenalized regression` <- glm.fit

  return(res)
}

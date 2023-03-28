#' Generate list of SMOTEd training data and test data for k-fold CV
#'
#' This function generates a list of data sets for k-fold CV and apply SMOTE algorithm to the training set in each fold. The output will be used for parameter tuning of \eqn{\lambda} in LASSO regression.
#'
#' @param X feature matrix
#' @param y binary outcome, where 1 = positive event & minority class and 0 = negative event & majority class
#' @param k_cv number of folds for the cross-validation ,default value is 10
#' @param k_nn number of nearest neighbors to be considered, default value is 5
#' @param N number of new synthetic examples to be generated for each observation, default value is 9
#' @param R size ratio of the majority class to be sampled to the SMOTEd minority class, default value is 1 so that the two classes are balanced
#'
#' @examples
#' \dontrun{
#' X <- matrix(rnorm(1000), ncol = 10)
#' y <- rbinom(100, 1, 0.1)
#' stratified_cv_smote(X, y)
#' }
#'
#' @export

stratified_cv_smote <- function(
    X,
    y,
    k_cv = 10,
    k_nn = 5,
    N = 9,
    R = 1
) {
  # check class of X and y
  if(is.null(dim(X)))
    stop("Input X should be a matrix.")
  if(is.data.frame(y)) y <- as.matrix(y)
  if(is.matrix(y)) {
    if(min(dim(y)) == 1) y <- as.vector(y)
    else stop("Input y should be a vector.")
  }
  # check size of X and y
  if(length(y) != nrow(X))
    stop("Length of outcome y is different from number of rows in X.")
  # check values in y
  if(length(unique(y)) != 2)
    stop("y should be a binary outcome.")
  if(!all.equal(sort(unique(y)), c(0,1)))
    stop("y should be only consist of 1 and 0.")

  # check distribution of y
  if(mean(y) > 0.5)
    warning(paste0("The proportion of the positive outcome (", round(100*mean(y), 1), "%) is larger than 50%. y = 1 does not seem to be the minority class."))
  else if(mean(y) > 0.2)
    warning(paste0("The proportion of the positive outcome (", round(100*mean(y), 1), "%) is larger than 20%, SMOTE might not be necessary."))

  # stratified cross-validation
  fold <- stratified_cv(y, k_cv)$Fold

  # apply SMOTE for the training set in each fold
  if(is.null(colnames(X))) colnames(X) <- paste0("V", 1:ncol(X))
  train_list <- lapply(1:k_cv, \(i) {
    smote(X = X[fold != i,], y = y[fold != i], k = k_nn, N = N, R = R)
  })
  test_list <- lapply(1:k_cv, \(i) {
    cbind(X[fold == i,], y[fold == i]) |> as.data.frame() |> setNames(c(colnames(X), "y"))
  })

  res <- list(
    train = lapply(1:k_cv, \(i) smote(X = X[fold != i,], y = y[fold != i], k = k_nn, N = N, R = R)),
    test = lapply(1:k_cv, \(i) cbind(X[fold == i,], y[fold == i]) |> as.data.frame() |> setNames(c(colnames(X), "y")))
    )

  # define class of the output
  class(res) <- c("smote_cv_dataList", "list")

  return(res)
}

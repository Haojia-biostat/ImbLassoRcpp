#' Generate list of training data for k-fold cross-validation
#'
#' This function prepares the data of cross-validation for parameter tuning of \eqn{\lambda} in LASSO regression. It provides options of whether to use stratified cross-validation on the input data or SMOTE algorithm on the training sets.
#'
#' @param X feature matrix
#' @param y binary outcome, where 1 = positive event & minority class and 0 = negative event & majority class
#' @param k_cv number of folds for the cross-validation ,default value is 10
#' @param stratified logical flag for whether to ensure that each fold contains a proportional representation of each class or stratum, default value is TRUE
#' @param SMOTE logical flag for whether to apply SMOTE to the training set, default value is TRUE
#' @param k_nn number of nearest neighbors to be considered, default value is 5
#' @param N number of new synthetic examples to be generated for each observation, default value is 9
#' @param R size ratio of the majority class to be sampled to the SMOTEd minority class, default value is 1 so that the two classes are balanced
#'
#' @return An object with class \code{"cv_smote_data"} including:
#' \item{train}{a list containing \code{k} training datasets, with or without SMOTE applied}
#' \item{test}{a list containing \code{k} test datasets}
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
    stratified = T,
    SMOTE = T,
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

  # stratified cross-validation
  fold <- stratified_cv(y, k_cv, stratified = stratified)

  if(is.null(colnames(X))) colnames(X) <- paste0("V", 1:ncol(X))

  # initialize output
  res <- vector(mode = "list", length = 2)
  names(res) <- c("train", "test")

  if(SMOTE)
    res$train <- lapply(1:k_cv, \(i) smote(X = X[fold != i,], y = y[fold != i], k = k_nn, N = N, R = R))
  else
    res$train <- lapply(1:k_cv, \(i) cbind(X[fold != i,], y[fold != i]) |> as.data.frame() |> stats::setNames(c(colnames(X), "y")))

  res$test <- lapply(1:k_cv, \(i) cbind(X[fold == i,], y[fold == i]) |> as.data.frame() |> stats::setNames(c(colnames(X), "y")))

  # define class of the output
  class(res) <- c("cv_smote_data", "list")

  return(res)
}

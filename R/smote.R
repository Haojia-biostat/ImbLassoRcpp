#' Generate balanced data ready for analysis
#'
#' This function generates the full SMOTEd dataset with both positive and negative outcome
#'
#' @param X feature matrix
#' @param y binary outcome, where 1 = positive event & minority class and 0 = negative event & majority class
#' @param k number of nearest neighbors to be considered, default value is 5
#' @param N number of new synthetic examples to be generated for each observation, default value is 9
#' @param R size ratio of the majority class to be sampled to the SMOTEd minority class, default value is 1 so that the two classes are balanced
#'
#' @details
#' The output dataset is consist of three parts:
#' 1. All the original observations from the minority class (nobs = \code{sum(y)})
#' 2. Synthetic new examples for the minority class by interpolate N values out of k nearest neighbors identified by Euclidean distance (nobs = \code{sum(y)*N})
#' 3. Sampled observations with replacement of the majority class (nobs = \code{sum(y)\*(N+1)\*(R)})
#'
#' @example
#' X <- matrix(rnorm(1000), ncol = 10)
#' y <- rbinom(100, 1, 0.1)
#' table(y)
#' smoted_data <- smote(X, y)
#' table(smoted_data$y)
#'
#' @export

smote <- function(
    X,
    y,
    k = 5,
    N = 9,
    R = 1
) {
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

  res <- rbind(
    cbind(X[sample(which(y == 0), R*(N+1)*sum(y), replace = T),], 0),
    cbind(X[which(y == 1),], 1),
    cbind(smote_minor_x(X[which(y == 1),], k = k, N = N), 1)
  ) |> as.data.frame()

  colnames(res) <- c(colnames(X), "y")
  return(res)

  }

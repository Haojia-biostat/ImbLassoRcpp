% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{stratified_cv}
\alias{stratified_cv}
\title{Stratified cross-validation sampling}
\usage{
stratified_cv(y, k = 10L, stratified = TRUE)
}
\arguments{
\item{y}{binary outcome, where 1 = positive event and 0 = negative event}

\item{k}{number of folds, default value is 10}

\item{stratified}{logical flag for whether to ensure that each fold contains a proportional representation
of each class or stratum, default value is TRUE}
}
\value{
A vector with the same length as \code{y} containing fold id from 1 to \code{k}
}
\description{
This function splits the observations to k-fold cross validation.
}
\examples{
y <- rbinom(2023, 1, 0.1)
tapply(y, stratified_cv(y), sum)
tapply(y, stratified_cv(y, stratified = FALSE), sum)

}

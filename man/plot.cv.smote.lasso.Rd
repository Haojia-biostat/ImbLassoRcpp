% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/plot.cv.smote.lasso.R
\name{plot.cv.smote.lasso}
\alias{plot.cv.smote.lasso}
\title{Plot the parameter tuning result}
\usage{
\method{plot}{cv.smote.lasso}(x, ...)
}
\arguments{
\item{x}{fitted \code{cv_smote_lasso} object}

\item{\dots}{other arguments that can be passed to \code{plot}}
}
\description{
This functions plots \eqn{log(\lambda)} on x-axis and \code{deviance} on y-axis, \
with two verticle dashed lines for \code{lambda.min} and \code{lambda.1se}
}

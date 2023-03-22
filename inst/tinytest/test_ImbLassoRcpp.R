
# Placeholder with simple test
expect_equal(1 + 1, 2)

# ---- test for stratified_cv ----

y <- rbinom(2023, 1, 0.1)
stratified_cv_test <- stratified_cv(y)
# test1: length of Fold in output should be equal to length of input y
expect_equal(length(stratified_cv_test$Fold), length(y))


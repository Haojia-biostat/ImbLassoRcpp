
# ---- smote_minor_x ----
X <- matrix(rnorm(100), 10)
smote_minor_x_test <- smote_minor_x(X)

# test1: Euclidean distance
expect_equivalent(
  smote_minor_x_test$Euclidean,
  as.matrix(dist(X))
)

# test2: k-Nearest-Neighbors
expect_equal(
  order(smote_minor_x_test$Euclidean[1,])[2:6],
  smote_minor_x_test$kNN[1,]
)

# test3: Synthetic new examples
expect_equal(
  dim(smote_minor_x_test$Synthetic),
  c(90, 10)
)

# ---- stratified_cv ----
y <- rbinom(2023, 1, 0.1)
stratified_cv_test <- stratified_cv(y)
stratified_cv_test1 <- stratified_cv(y, stratified = F)
stratified_cv_test2 <- stratified_cv(y, stratified = F)

# test1: length of output
expect_equal(
  length(stratified_cv_test),
  length(y)
)

# test2: stratified = T
expect_true(
  length(unique(tapply(y, stratified_cv_test, sum))) <= 2
)

# test3: stratified = F
expect_false(identical(
  sort(tapply(y, stratified_cv_test1, sum)),
  sort(tapply(y, stratified_cv_test2, sum))
))

# ---- lambda_gen ----
X <- matrix(rnorm(30000), ncol = 200)
y <- rbinom(150, 1, 0.1)
lambda_gen_test <- lambda_gen(X, y)

# test1: Entry value
expect_equal(
  lambda_gen_test$`Entry value`,
  max(abs(y %*% X))/150
)

# test2: Minimum ratio
expect_equal(
  lambda_gen_test$`Minimum ratio`,
  0.01
)


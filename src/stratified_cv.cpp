
#include <Rcpp.h>
using namespace Rcpp;

//'Stratified cross-validation sampling
//'
//'This function splits the observations to k-fold cross validation.
//'
//'@name stratified_cv
//'
//'@param y binary outcome, where 1 = positive event and 0 = negative event
//'@param k number of folds, default value is 10
//'@param stratified logical flag for whether to ensure that each fold contains a proportional representation
//'of each class or stratum, default value is TRUE
//'
//'@return A vector with the same length as \code{y} containing fold id from 1 to \code{k}
//'
//'@examples
//'y <- rbinom(2023, 1, 0.1)
//'tapply(y, stratified_cv(y), sum)
//'tapply(y, stratified_cv(y, stratified = F), sum)
//'
//'@export

// [[Rcpp::export]]
IntegerVector stratified_cv(
    const IntegerVector& y,
    const int& k = 10,
    const bool& stratified = true
) {

  // number of obs
  int N = y.size();

  // initialize fold id
  IntegerVector fold_ind = seq(0, k-1);
  IntegerVector fold_id = sample(fold_ind, k, false);

  // initialize fold assignment
  IntegerVector fold(N);

  // initialize sample size in each fold
  IntegerVector N_fold(k);
  int N_fold_floor = floor(N/k);
  int N_remainder = N - N_fold_floor*k;

  // fill numbers
  if(N_remainder == 0)
    std::fill(N_fold.begin(), N_fold.end(), N_fold_floor);
  else {
    std::fill(N_fold.begin(), N_fold.begin() + N_remainder, N_fold_floor + 1);
    std::fill(N_fold.begin() + N_remainder, N_fold.end(), N_fold_floor);
  }

  if(stratified) {

    // number of events
    int n = sum(y);

    // generate row indices for positive and negative outcome
    IntegerVector pos_ind_init;
    IntegerVector neg_ind_init;
    for(int i = 0; i < N; ++i) {
      if(y[i] == 1)
        pos_ind_init.push_back(i);
      else
        neg_ind_init.push_back(i);
    }

    // shuffle row indices for positive and negative outcome
    IntegerVector pos_ind = sample(pos_ind_init, n, false);
    IntegerVector neg_ind = sample(neg_ind_init, N-n, false);

    // initialize event size in each fold
    IntegerVector n_fold(k);
    int n_fold_floor = floor(n/k);
    int n_remainder = n - n_fold_floor*k;

    // fill numbers
    if(n_remainder == 0)
      std::fill(n_fold.begin(), n_fold.end(), n_fold_floor);
    else {
      std::fill(n_fold.begin(), n_fold.begin() + n_remainder, n_fold_floor + 1);
      std::fill(n_fold.begin() + n_remainder, n_fold.end(), n_fold_floor);
    }

    // assign observations to different folds
    IntegerVector pos_ind_fold;
    IntegerVector neg_ind_fold;

    for(int i = 0; i < k; ++i) {

      int fold_id_i = fold_id[i] + 1;

      int pos_size_i = n_fold[i];
      IntegerVector pos_seq_i = seq(0, pos_size_i-1);
      pos_ind_fold = pos_ind[pos_seq_i];
      fold[pos_ind_fold] = fold_id_i;
      pos_ind.erase(pos_ind.begin(), pos_ind.begin() + pos_size_i);

      int neg_size_i = N_fold[i] - n_fold[i];
      IntegerVector neg_seq_i = seq(0, neg_size_i-1);
      neg_ind_fold = neg_ind[neg_seq_i];
      fold[neg_ind_fold] = fold_id_i;
      neg_ind.erase(neg_ind.begin(), neg_ind.begin() + neg_size_i);

    }

  } else {

    // shuffle row indices
    IntegerVector obs_ind_init = seq(0, N-1);
    IntegerVector obs_ind = sample(obs_ind_init, N, false);

    // assign observations to different folds
    IntegerVector obs_ind_fold;

    for(int i = 0; i < k; ++i) {

      int fold_id_i = fold_id[i] + 1;

      int fold_size_i = N_fold[i];
      IntegerVector fold_seq_i = seq(0, fold_size_i-1);
      obs_ind_fold= obs_ind[fold_seq_i];
      fold[obs_ind_fold] = fold_id_i;
      obs_ind.erase(obs_ind.begin(), obs_ind.begin() + fold_size_i);

    }

  }

  return fold;

}

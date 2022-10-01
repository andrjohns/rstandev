
<!-- README.md is generated from README.Rmd. Please edit that file -->

# rstandev

<!-- badges: start -->
<!-- badges: end -->

An experimental, header-only interface to Stan in R

## Usage

Initialise and compile a new stanmodel object:

``` r
library(rstandev)

model = "
functions {
  row_vector rtn_rowvec(row_vector x) {
    return x;
  }
}

data {
  int<lower=0> N;
  array[N] int<lower=0,upper=1> y; // or int<lower=0,upper=1> y[N];
}
parameters {
  real<lower=0,upper=1> theta;
}
model {
  theta ~ beta(1,1);  // uniform prior on interval 0,1
  y ~ bernoulli(theta);
}

generated quantities {
  vector[N] log_lik;
  for (n in 1:N) {
    log_lik[n] = bernoulli_lpmf(y[n] | theta);
  }
}
"

stmod <- stanmodel$new(model_code = model)
stmod$compile()
#> ℹ 8 functions decorated with [[cpp11::register]]
```

The compiled `stanmodel` object can then call `log_prob` and
`grad_log_prob` as long as data is also passed:

``` r
stmod$log_prob(list(N=2, y=0:1), upars = 0.1)
#> [1] -2.777587
stmod$grad_log_prob(list(N=2, y=0:1), upars = 0.1)
#> [1] -0.09991675
#> attr(,"log_prob")
#> [1] -2.777587
```

As well as `constrain_` and `unconstrain_pars`:

``` r
stmod$constrain_pars(list(N=2, y=0:1), upars = 2.5)
#> [1] 0.9241418
stmod$unconstrain_pars(list(N=2, y=0:1), list(theta = 0.1))
#> [1] -2.197225
```

Any model functions can be exposed using the `expose_functions()`
member. By default, these functions will be added as members to the
source `stanmodel` object, but they can be exported to the global
environment using the `global = TRUE` flag:

``` r
stmod$expose_functions()
#> ℹ 1 functions decorated with [[cpp11::register]]
stmod$rtn_rowvec(c(1, 2, 3))
#> [1] 1 2 3
```

Sampling can then be performed using the `sample()` member:

``` r
samp <- stmod$sample(
  data = list(N=2, y=0:1),
  num_chains = 4,
  num_threads = 8, refresh = 1000)
```

Similarly for `optimize()` and `variational()` members.

The generated `stanfit` object can then be summarised with the
`summary()` function:

``` r
samp$summary()
#> # A tibble: 10 × 10
#>    variable      mean median    sd    mad     q5    q95     rhat ess_b…¹ ess_t…²
#>    <chr>        <dbl>  <dbl> <dbl>  <dbl>  <dbl>  <dbl>    <dbl>   <dbl>   <dbl>
#>  1 lp__        -3.31  -3.02  0.759 0.336  -4.89  -2.77   1.00e 0  3.14e3   2587.
#>  2 accept_sta…  0.928  0.975 0.114 0.0367  0.690  1      1.01e 0  1.01e4     NA
#>  3 stepsize__   0.932  0.933 0.110 0.154   0.792  1.07   1.89e14  4.02e0     NA
#>  4 treedepth__  1.40   1     0.512 0       1      2      1.00e 0  7.04e3   4008.
#>  5 n_leapfrog…  2.61   3     1.36  0       1      7      1.01e 0  9.13e2     NA
#>  6 divergent__  0      0     0     0       0      0     NA       NA          NA
#>  7 energy__     3.80   3.49  1.03  0.734   2.83   5.85   1.00e 0  3.05e3   3125.
#>  8 theta        0.498  0.495 0.220 0.251   0.140  0.860  1.00e 0  2.92e3   2775.
#>  9 log_lik.1   -0.824 -0.684 0.589 0.487  -1.96  -0.150  1.00e 0  2.92e3   2775.
#> 10 log_lik.2   -0.832 -0.703 0.585 0.506  -1.97  -0.151  1.00e 0  2.92e3   2775.
#> # … with abbreviated variable names ¹​ess_bulk, ²​ess_tail
```

And the ELPD-LOO calculated with the `loo()` member:

``` r
samp$loo()
#>
#> Computed from 8000 by 2 log-likelihood matrix
#>
#>          Estimate  SE
#> elpd_loo     -2.1 0.0
#> p_loo         0.8 0.0
#> looic         4.3 0.0
#> ------
#> Monte Carlo SE of elpd_loo is 0.0.
#>
#> All Pareto k estimates are good (k < 0.5).
#> See help('pareto-k-diagnostic') for details.
```

## Installation

You can install the development version of rstandev from
[GitHub](https://github.com/) with:

``` r
# install.packages("devtools")
devtools::install_github("andrjohns/sundialr")
devtools::install_github("andrjohns/rstandev")
```

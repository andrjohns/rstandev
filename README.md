
<!-- README.md is generated from README.Rmd. Please edit that file -->

# rstandev

<!-- badges: start -->

[![R-CMD-check](https://github.com/andrjohns/rstan.dev/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/andrjohns/rstan.dev/actions/workflows/R-CMD-check.yaml)
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
#> ℹ 1 functions decorated with [[cpp11::register]]
```

The compiled `stanmodel` object can then call `log_prob` and
`grad_log_prob` as long as data is also passed:

``` r
stmod$log_prob(list(N=2, y=0:1), upars = 0.1)
#> [1] -2.777587
stmod$grad_log_prob(list(N=2, y=0:1), upars = 0.1)
#> [1] 0
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
#>    variable     mean   median      sd     mad       q5      q95     rhat ess_b…¹
#>    <chr>       <dbl>    <dbl>   <dbl>   <dbl>    <dbl>    <dbl>    <dbl>   <dbl>
#>  1 lp__     -3.34e+0 -3.04e+0 7.65e-1 3.61e-1 -4.92e+0 -2.77e+0  1.00e 0 1720.  
#>  2 accept_…  8.16e-1  8.84e-1 1.90e-1 1.54e-1  4.19e-1  9.99e-1  1.02e 0  159.  
#>  3 stepsiz…  1.57e-3  1.59e-3 4.42e-4 6.27e-4  1.08e-3  2.02e-3  1.89e14    4.02
#>  4 treedep…  1   e+1  1   e+1 0       0        1   e+1  1   e+1 NA         NA   
#>  5 n_leapf…  1.02e+3  1.02e+3 0       0        1.02e+3  1.02e+3 NA         NA   
#>  6 diverge…  0        0       0       0        0        0       NA         NA   
#>  7 energy__  3.81e+0  3.49e+0 1.02e+0 7.51e-1  2.82e+0  5.84e+0  1.00e 0 2233.  
#>  8 theta     4.97e-1  4.95e-1 2.25e-1 2.60e-1  1.35e-1  8.67e-1  1.00e 0  889.  
#>  9 log_lik… -8.29e-1 -6.83e-1 6.03e-1 5.00e-1 -2.02e+0 -1.45e-1  1.00e 0  889.  
#> 10 log_lik… -8.39e-1 -7.04e-1 5.96e-1 5.18e-1 -2.00e+0 -1.43e-1  1.00e 0  889.  
#> # … with 1 more variable: ess_tail <dbl>, and abbreviated variable name
#> #   ¹​ess_bulk
```

And the ELPD-LOO calculated with the `loo()` member:

``` r
samp$loo()
#> 
#> Computed from 8000 by 2 log-likelihood matrix
#> 
#>          Estimate  SE
#> elpd_loo     -2.2 0.0
#> p_loo         0.8 0.0
#> looic         4.3 0.0
#> ------
#> Monte Carlo SE of elpd_loo is 0.1.
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

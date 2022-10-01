devtools::load_all("Stan Git/rstan.dev/")

#library(rstandev)
st <- stanmodel$new("bernoulli.stan")
st$compile()
st$log_prob(list(N=2,y=0:1), c(0.1))
st$grad_log_prob(list(N=2,y=0:1), c(0.1))
st$constrain_pars(list(N=2,y=0:1), c(0.1))
st$unconstrain_pars(list(N=2,y=0:1), list(theta=0.5249792))

mod_ptr <- st$get_model_ptr(list(N=2,y=0:1))
grad_log_prob(mod_ptr, c(0.1))

sf <- st$sample(
  data = list(N=2,y=0:1),
  init = list(theta = 0.1),
  num_threads=2)

sf <- st$optimize(
  data = list(N=2,y=0:1),
  algorithm = "Newton",
  num_chains = 4,
  num_threads = 4)

sf <- st$variational(
  data = list(N=2,y=0:1),
  algorithm = "meanfield",
  num_chains = 4,
  num_threads = 1)
Sys.time() -a

num_cores <- min(8,4)



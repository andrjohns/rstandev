devtools::load_all("Stan Git/rstan.dev/")
st <- stanmodel$new("bernoulli.stan", allow_undefined = TRUE, external_cpp = "inc.hpp")
sf <- st$sample(list(N=2,y=0:1),list(theta = 0.1))

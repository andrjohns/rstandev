
modcode <- "data { real y_mean; } parameters { real y; } model { y ~ normal(y_mean, 1); }"

test_that("compilation works", {
  st <- stan_model(model_code = modcode, quiet = FALSE)
  sf <- st$sample(
    data = list("y_mean" = 0.0),
    num_chains = 2,
    num_threads = 2,
    refresh = 4000)
})

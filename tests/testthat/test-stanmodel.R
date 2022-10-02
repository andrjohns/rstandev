
modcode <- "data { real y_mean; } parameters { real y; } model { y ~ normal(y_mean, 1); }"

test_that("compilation works", {
  st <- rstandev::stanmodel$new(model_code = modcode)
  st$compile(quiet = FALSE)
  sf <- st$sample(
    data = list("y_mean" = 0.0),
    num_chains = 2,
    num_threads = 1,
    refresh = 4000)
})

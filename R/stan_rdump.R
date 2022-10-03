stan_rdump <- function(list) {
  jsonlite::toJSON(list, auto_unbox = TRUE)
}

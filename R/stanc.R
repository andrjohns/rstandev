stanc <- function(model_code) {
  stanc_context$call("stanc", "model", model_code)$result
}

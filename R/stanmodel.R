#' @export
stanmodel <- R6::R6Class("stanmodel",
  public = list(
    hpp_code = NULL,
    model_code = NULL,
    env = NULL,
    initialize = function(model_path) {
      self$model_code <- paste(readLines(model_path), collapse = "\n")
      self$hpp_code <- stanc(self$model_code)
      cpp_locations <- generate_cpp(self$hpp_code)
      self$env <- compile_functions_into_env(cpp_locations)
    },
    log_prob = function(data_list, upars) {
      self$env$log_prob(stan_rdump(data_list), upars)
    },
    grad_log_prob = function(data_list, upars) {
      self$env$grad_log_prob(stan_rdump(data_list), upars)
    }
  )
)

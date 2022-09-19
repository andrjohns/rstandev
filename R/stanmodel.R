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
      args <- list(
        rdump_string = stan_rdump(data_list),
        upars = upars
      )
      self$env$log_prob(args)
    },
    grad_log_prob = function(data_list, upars) {
      args <- list(
        rdump_string = stan_rdump(data_list),
        upars = upars
      )
      self$env$grad_log_prob(args)
    },
    get_param_names = function(data_list) {
      args <- list(
        rdump_string = stan_rdump(data_list)
      )
      self$env$get_param_names(args)
    },
    get_param_dims = function(data_list) {
      args <- list(
        rdump_string = stan_rdump(data_list)
      )
      self$env$get_dims(args)
    },
    unconstrain_pars = function(data_list, init_list) {
      args <- list(
        rdump_string = stan_rdump(data_list),
        init_string = stan_rdump(init_list)
      )
      self$env$unconstrain_pars(args)
    },
    constrain_pars = function(data_list, upars) {
      args <- list(
        rdump_string = stan_rdump(data_list),
        upars = upars
      )
      self$env$constrain_pars(args)
    }
  )
)

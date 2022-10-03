
stanfit <- R6::R6Class("stanfit",
  inherit = stanmodel,
  public = list(
    draws = NULL,
    summary_table = NULL,
    loo_results = NULL,
    initialize = function(stanmodel, private_args, output_files, model_ptr) {
      raw <- do.call(rbind.data.frame, lapply(output_files, function(sample) {
        readr::read_csv(sample, comment = "#", show_col_types = FALSE)
      }))
      self$draws <- posterior::as_draws(raw)
      self$hpp_code <- stanmodel$hpp_code
      self$model_code <- stanmodel$model_code
      private$env <- private_args$env
      private$dynlib_basename <- private_args$dynlib_basename
      private$dynlib_ext <- private_args$dynlib_ext
      private$dynlib_bytes <- private_args$dynlib_bytes
      private$cpp11_export_def <- private_args$cpp11_export_def
      private$model_ptr <- model_ptr
    },
    summary = function() {
      if (is.null(self$summary_table)) {
        self$summary_table <- posterior::summarise_draws(self$draws)
        return(self$summary_table)
      }
      self$summary_table
    },
    loo = function(parameter = "log_lik", cores = getOption("mc.cores", 1)) {
      if (is.null(self$loo_results)) {
        LLarray <- suppressWarnings(as.matrix(self$draws[,grep(parameter, colnames(self$draws))]))
        r_eff <- loo::relative_eff(x = exp(LLarray), cores = cores,
                                    chain_id=self$draws$.chain)
        self$loo_results = loo::loo(LLarray, r_eff = r_eff, cores = cores,
                                    save_psis = TRUE)
      }
      self$loo_results
    }
  ),
  private = list(
    model_ptr = NULL
  )
)
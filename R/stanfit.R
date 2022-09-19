#' @export
stanfit <- R6::R6Class("stanfit",
  inherit = stanmodel,
  public = list(
    draws = NULL,
    summary = NULL,
    initialize = function(stanmodel, private_args, output_files, model_ptr) {
      raw <- purrr::map_dfr(output_files, function(sample) {
        readr::read_csv(sample, comment = "#", show_col_types = FALSE)
      }, .id = ".chain")
      self$draws <- posterior::as_draws(raw)
      self$summary <- posterior::summarise_draws(self$draws)
      self$hpp_code <- stanmodel$hpp_code
      self$model_code <- stanmodel$model_code
      private$env <- private_args$env
      private$dynlib_basename <- private_args$dynlib_basename
      private$dynlib_ext <- private_args$dynlib_ext
      private$dynlib_bytes <- private_args$dynlib_bytes
      private$cpp11_export_def <- private_args$cpp11_export_def
      private$model_ptr <- model_ptr
    }
  ),
  private = list(
    model_ptr = NULL
  )
)
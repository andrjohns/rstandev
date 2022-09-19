#' @export
stanmodel <- R6::R6Class("stanmodel",
  public = list(
    cpp_code = NULL,
    model_code = NULL,
    env = NULL,
    initialize = function(model_path) {
      self$env <- new.env()
      self$model_code <- paste(readLines(model_path), collapse = "\n")
      self$cpp_code <- stanc_context$call("stanc", "model", self$model_code)$result

      functions <- readLines(system.file("include", "cpp11", "base.cpp", package = "rstan.dev"))
      functions <- paste(functions, collapse = "\n")

      cpp_code <- paste(self$cpp_code, functions, sep = "\n")
      dir <- file.path(tempdir(), "stan_cpp11")
      if (dir.exists(dir)) {
        unlink(dir, recursive = TRUE)
      }
      dir.create(dir, recursive = TRUE)
      file <- tempfile(tmpdir = dir, fileext = ".cpp")
      brio::write_lines(cpp_code, file)

      Sys.setenv("PKG_CXXFLAGS"=cxxflags())
      Sys.setenv("PKG_LIBS"=libflags())
      cpp11::cpp_source(file=file, quiet=FALSE, cxx_std = "CXX14", dir = dir, clean = FALSE, env = self$env)
      Sys.unsetenv("PKG_CXXFLAGS")
      Sys.unsetenv("PKG_LIBS")
    },
    log_prob = function(data_list, upars) {
      self$env$log_prob(stan_rdump(data_list), upars)
    },
    grad_log_prob = function(data_list, upars) {
      self$env$grad_log_prob(stan_rdump(data_list), upars)
    }
  )
)

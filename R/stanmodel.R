#' @export
stanmodel <- R6::R6Class("stanmodel",
  public = list(
    hpp_code = NULL,
    model_code = NULL,
    env = NULL,
    initialize = function(model_path = NULL, model_code = NULL) {
      if (is.null(model_code)) {
        self$model_code <- paste(readLines(model_path), collapse = "\n")
      } else {
        self$model_code <- model_code
      }
      self$hpp_code <- stanc(self$model_code)
      cpp_locations <- generate_cpp(self$hpp_code)
      self$env <- compile_functions_into_env(cpp_locations)

      cpp_files <- list.files(file.path(cpp_locations$dir, "src"), pattern = ".cpp")
      base_cpp <- grep("cpp11", cpp_files, invert = TRUE, value = TRUE)

      private$dynlib_basename <- gsub(".cpp", "", base_cpp, fixed = TRUE)
      private$dynlib_ext <- .Platform$dynlib.ext

      dynlib_path <- file.path(cpp_locations$dir, "src",
                                paste0(private$dynlib_basename, .Platform$dynlib.ext))
      private$dynlib_bytes <- readr::read_file_raw(dynlib_path)
      private$cpp11_export_def <- readr::read_file(file.path(cpp_locations$dir, "R", "cpp11.R"))
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
    },
    refresh_env = function() {
      if (private$dynlib_ext != .Platform$dynlib.ext) {
        warning("The refresh_env() function can only reload compiled code on ",
                "the same platform, the model was re-compiled instead",
                call. = FALSE)
        self$initialize(model_code = self$model_code)
      } else {
        self$env <- new.env()
        dir <- file.path(tempdir(), "cpp11_env_refresh")
        if (dir.exists(dir)) {
          unlink(dir, recursive = TRUE)
        }
        dir.create(file.path(dir, "R"), recursive = TRUE)
        dir.create(file.path(dir, "src"), recursive = TRUE)
        cat(private$cpp11_export_def, file = file.path(dir, "R", "cpp11.R"))
        libpath <- file.path(dir, "src", paste0(private$dynlib_basename, private$dynlib_ext))
        readr::write_file(x = private$dynlib_bytes, file = libpath)

        source(file.path(dir, "R", "cpp11.R"), local = self$env)
        dyn.load(libpath, local = TRUE, now = TRUE)
      }
    }
  ),
  private = list(
    dynlib_basename = NULL,
    dynlib_ext = NULL,
    dynlib_bytes = NULL,
    cpp11_export_def = NULL
  )
)

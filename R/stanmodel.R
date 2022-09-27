#' @export
stanmodel <- R6::R6Class(
  "stanmodel",
  lock_objects = FALSE,
  public = list(
    hpp_code = NULL,
    model_code = NULL,
    initialize = function(model_path = NULL, model_code = NULL,
                          include_paths = NULL) {
      if (is.null(model_code)) {
        model_code_raw <- readr::read_file(model_path)
      } else {
        model_code_raw <- model_code
      }
      self$model_code <- insert_includes(model_code_raw, include_paths)
    },
    compile = function(standalone_functions = FALSE,
                  use_opencl = FALSE, optim_level = 0,
                  allow_undefined = FALSE, warn_pedantic = FALSE,
                  warn_uninitialized = FALSE, external_cpp = NULL) {
      self$hpp_code <- stanc(self$model_code, allow_undefined = allow_undefined, external_cpp = external_cpp)
      cpp_locations <- generate_cpp(self$hpp_code)
      private$env <- compile_cpp(cpp_locations, new.env())

      cpp_files <- list.files(file.path(cpp_locations$dir, "src"), pattern = ".cpp")
      base_cpp <- grep("cpp11", cpp_files, invert = TRUE, value = TRUE)

      private$dynlib_basename <- gsub(".cpp", "", base_cpp, fixed = TRUE)
      private$dynlib_ext <- .Platform$dynlib.ext

      dynlib_path <- file.path(cpp_locations$dir, "src",
                                paste0(private$dynlib_basename, .Platform$dynlib.ext))
      private$dynlib_bytes <- readr::read_file_raw(dynlib_path)
      private$cpp11_export_def <- readr::read_file(file.path(cpp_locations$dir, "R", "cpp11.R"))
    },
    log_prob = function(data_list = NULL, upars) {
      args <- list(
        upars = upars
      )
      if (!is.null(data_list)) {
        args$rdump_data <- stan_rdump(data_list)
      } else if (!is.null(private$model_ptr)) {
        args$model_ptr <- private$model_ptr
      }
      private$env$log_prob(args)
    },
    grad_log_prob = function(data_list = NULL, upars) {
      args <- list(
        upars = upars
      )
      if (!is.null(data_list)) {
        args$rdump_data <- stan_rdump(data_list)
      } else if (!is.null(private$model_ptr)) {
        args$model_ptr <- private$model_ptr
      }
      private$env$grad_log_prob(args)
    },
    get_param_names = function(data_list) {
      args <- list(
        rdump_string = stan_rdump(data_list)
      )
      private$env$get_param_names(args)
    },
    get_param_dims = function(data_list) {
      args <- list(
        rdump_string = stan_rdump(data_list)
      )
      private$env$get_dims(args)
    },
    unconstrain_pars = function(data_list, init_list) {
      args <- list(
        rdump_string = stan_rdump(data_list),
        init_string = stan_rdump(init_list)
      )
      private$env$unconstrain_pars(args)
    },
    constrain_pars = function(data_list, upars) {
      args <- list(
        rdump_string = stan_rdump(data_list),
        upars = upars
      )
      private$env$constrain_pars(args)
    },
    refresh_env = function() {
      if (private$dynlib_ext != .Platform$dynlib.ext) {
        warning("The refresh_env() function can only reload compiled code on ",
                "the same platform, the model was re-compiled instead",
                call. = FALSE)
        self$initialize(model_code = self$model_code)
      } else {
        dir <- file.path(tempdir(), "cpp11_env_refresh")
        private$write_source(dir = dir)

        private$env <- new.env()
        source(file.path(dir, "R", "cpp11.R"), local = private$env)
        libpath <- file.path(dir, "src", paste0(private$dynlib_basename, private$dynlib_ext))
        dyn.load(libpath, local = TRUE, now = TRUE)
      }
    },
    expose_functions = function(global = FALSE) {
      model_cpp <- stanc(self$model_code, standalone_functions = TRUE)
      model_lines <- strsplit(model_cpp, "\n", fixed = TRUE)[[1]]
      funs <- grep("// [[stan::function]]", model_lines, fixed = TRUE)
      funs <- c(funs, length(model_lines))

      stan_funs <- purrr::map_chr(seq_len(length(funs) - 1), function(ind) {
        fun_body <- model_lines[funs[ind]:(funs[ind + 1] - 1)]
        prep_fun_cpp(fun_body, model_lines)
      })

      mod_stan_funs <- paste(c(model_lines[1:(funs[1] - 1)], stan_funs), collapse = "\n")
      stan_cpp_locations <- generate_cpp(mod_stan_funs, standalone_funs = TRUE)
      if (global) {
        invisible(compile_cpp(stan_cpp_locations, globalenv(), return_env = FALSE))
      } else {
        private$standalone_env <- compile_cpp(stan_cpp_locations, new.env())
        purrr::walk(stan_funs, function(fun) {
          fun_name <- decor::parse_cpp_function(fun, is_attribute = TRUE)$name
          self[[fun_name]] <- private$standalone_env[[fun_name]]
        })
        invisible(NULL)
      }
    }
  ),
  private = list(
    env = NULL,
    standalone_env = NULL,
    dynlib_basename = NULL,
    dynlib_ext = NULL,
    dynlib_bytes = NULL,
    cpp11_export_def = NULL,
    write_source = function(dir) {
      if (dir.exists(dir)) {
        unlink(dir, recursive = TRUE)
      }
      dir.create(file.path(dir, "R"), recursive = TRUE)
      dir.create(file.path(dir, "src"), recursive = TRUE)
      cat(private$cpp11_export_def, file = file.path(dir, "R", "cpp11.R"))
      libpath <- file.path(dir, "src", paste0(private$dynlib_basename, private$dynlib_ext))
      readr::write_file(x = private$dynlib_bytes, file = libpath)
    }
  )
)


sample <- function(
  data,
  init = NULL,
  output_dir = tempdir(),
  num_chains = 4,
  num_threads = 1,
  random_seed = 0,
  num_samples = 2000,
  num_warmup = 2000,
  save_warmup = FALSE,
  num_thin = 1,
  adapt = TRUE,
  gamma = 0.05,
  delta = 0.8,
  kappa = 0.75,
  t0 = 10,
  init_buffer = 75,
  term_buffer = 50,
  window = 25,
  algorithm = "hmc",
  engine = "nuts",
  max_depth = 10,
  metric = "diag_e",
  inv_metric = NULL,
  stepsize = 1,
  stepsize_jitter = 0,
  init_radius = 2,
  refresh = 200
  ) {

  input_args <- as.list(environment())

  service_name <- algorithm

  if (algorithm != "fixed_param") {
    service_name <- paste0(c(service_name, engine, metric), collapse = "_")
    if (adapt) {
      service_name <- paste0(c(service_name, "adapt"), collapse = "_")
    }
  }
  if (!is.null(init)) {
    input_args$rdump_init <- stan_rdump(init)
  }
  input_args$rdump_data <- stan_rdump(data)
  input_args$id <- 1
  if (!is.null(inv_metric)) {
    if (metric == "diag_e") {
      input_args$inv_metric <- paste0(
        "inv_metric <- c(",
        paste(diag(inv_metric), collapse = ","), ")"
      )
    } else {
      input_args$inv_metric <- stan_rdump(list(inv_metric=inv_metric))
    }
  }

  private$env$stan_methods_wrapper(service_name, input_args)
  model_ptr <- private$env$new_model(input_args$rdump_data, seed = random_seed)
  outputs <- paste0(input_args$output_dir, "/output", "_", 1:(num_chains), ".csv")
  stanfit$new(self, private, outputs, model_ptr)
}


optimize <- function(
  data,
  init = NULL,
  algorithm = "BFGS",
  output_dir = tempdir(),
  num_chains = 4,
  num_threads = 1,
  random_seed = 0,
  num_iterations = 2000,
  save_iterations = TRUE,
  init_radius = 2,
  refresh = 200,
  history_size = 5,
  init_alpha = 0.001,
  tol_obj = 0.000000000001,
  tol_rel_obj = 10000.0,
  tol_grad = 10000000.0,
  tol_rel_grad = 10000000.0,
  tol_param = 0.00000001
  ) {

  input_args <- as.list(environment())

  service_name <- tolower(algorithm)

  if (!is.null(init)) {
    input_args$rdump_init <- stan_rdump(init)
  }
  input_args$rdump_data <- stan_rdump(data)
  input_args$id <- 1

  private$env$stan_methods_wrapper(service_name, input_args)
  model_ptr <- private$env$new_model(input_args$rdump_data, seed = random_seed)
  outputs <- paste0(input_args$output_dir, "/output", "_", 1:(num_chains), ".csv")
  stanfit$new(self, private, outputs, model_ptr)
}

variational <- function(
  data,
  init = NULL,
  algorithm = "meanfield",
  output_dir = tempdir(),
  num_chains = 4,
  num_threads = 1,
  random_seed = 0,
  max_iterations = 10000,
  init_radius = 2,
  grad_samples = 1,
  elbo_samples = 100,
  tol_rel_obj = 0.01,
  eta = 1.0,
  adapt_engaged = TRUE,
  adapt_iterations = 50,
  eval_elbo = 100,
  output_samples = 1000
  ) {

  input_args <- as.list(environment())

  service_name <- tolower(algorithm)

  if (!is.null(init)) {
    input_args$rdump_init <- stan_rdump(init)
  }
  input_args$rdump_data <- stan_rdump(data)
  input_args$id <- 1

  private$env$stan_methods_wrapper(service_name, input_args)
  model_ptr <- private$env$new_model(input_args$rdump_data, seed = random_seed)
  outputs <- paste0(input_args$output_dir, "/output", "_", 1:(num_chains), ".csv")
  stanfit$new(self, private, outputs, model_ptr)
}

stanmodel$set("public", name = "sample", value = sample)
stanmodel$set("public", name = "optimize", value = optimize)
stanmodel$set("public", name = "variational", value = variational)

#' @export
stan_model <- function(model_path = NULL, model_code = NULL,
                        include_paths = NULL, ...) {
  st <- stanmodel$new(model_path = model_path,
                      model_code = model_code,
                      include_paths = include_paths)
  st$compile(...)
  return(st)
}


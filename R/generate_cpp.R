generate_cpp <- function(hpp_code, standalone_funs = FALSE) {
  exported_functions <- ""
  if (!standalone_funs) {
    methods <- list.files(system.file("include", "exported_functions", package = "rstan.dev"))
    cpp_methods <- sapply(methods, function(method) {
      readr::read_file(system.file("include", "exported_functions", method,
                                      package = "rstan.dev"))
    })
    exported_functions <- c(exported_functions, paste(cpp_methods, collapse = "\n"))
  }

  cpp_code <- paste(c(hpp_code, exported_functions), collapse = "\n")
  dir <- file.path(tempdir(), "stan_cpp11")
  if (dir.exists(dir)) {
    unlink(dir, recursive = TRUE)
  }
  dir.create(dir, recursive = TRUE)
  file <- tempfile(tmpdir = dir, fileext = ".cpp")
  cat(cpp_code, file = file, sep = "\n")
  list(dir = dir, file = file)
}

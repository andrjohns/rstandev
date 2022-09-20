generate_cpp <- function(hpp_code, standalone_funs = FALSE) {
  includes <- ""
  if (!standalone_funs) {
    cpp_includes <- readr::read_file(system.file("include", "cpp11", "includes.hpp",
                                      package = "rstan.dev"))
    cpp_typedefs <- readr::read_file(system.file("include", "cpp11", "typedefs.hpp",
                                      package = "rstan.dev"))
    cpp_base_funs <- readr::read_file(system.file("include", "cpp11", "base.hpp",
                                      package = "rstan.dev"))

    methods <- list.files(system.file("include", "cpp11_methods", package = "rstan.dev"))
    cpp_methods <- sapply(methods, function(method) {
      readr::read_file(system.file("include", "cpp11_methods", method,
                                      package = "rstan.dev"))
    })
    cpp_methods <- paste(cpp_methods, collapse = "\n")
    includes <- c(cpp_includes, cpp_typedefs, cpp_base_funs, cpp_methods)
  }

  cpp_code <- paste(c(hpp_code, includes), collapse = "\n")
  dir <- file.path(tempdir(), "stan_cpp11")
  if (dir.exists(dir)) {
    unlink(dir, recursive = TRUE)
  }
  dir.create(dir, recursive = TRUE)
  file <- tempfile(tmpdir = dir, fileext = ".cpp")
  brio::write_lines(cpp_code, file)
  list(dir = dir, file = file)
}

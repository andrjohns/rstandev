#' @export
test <- function(hpp_path) {
  hpp <- paste(readLines(hpp_path), collapse = "\n")
  functions <- readLines(system.file("include", "cpp11", "base.cpp", package = "rstan.dev"))
  functions <- paste(functions, collapse = "\n")

  cpp_code <- paste(hpp, functions, sep = "\n")
  dir <- tempdir()
  if (!dir.exists(dir)) {
    dir.create(dir, recursive = TRUE)
  }
  file <- tempfile(tmpdir = dir, fileext = ".cpp")
  brio::write_lines(cpp_code, file)

  Sys.setenv("PKG_CXXFLAGS"=cxxflags())
  Sys.setenv("PKG_LIBS"=libflags())
  cpp11::cpp_source(file=file, quiet=FALSE, cxx_std = "CXX14")
  Sys.unsetenv("PKG_CXXFLAGS")
  Sys.unsetenv("PKG_LIBS")

  N=2;y=0:1
  theta = 0.1

  mod_ptr <- new_model(stan_rdump(list("N","y")), 1)
  samples <- vector(mode="character", length=2)
  sample_test(mod_ptr, stan_rdump("theta"), samples)

  purrr::map_dfr(samples, function(sample) {
    readr::read_csv(sample, comment = "#", show_col_types = FALSE)
  }, .id = ".chain")

  #out <- posterior::as_draws(res)
  #out_summ <- posterior::summarise_draws(out)

  #log_prob(mod_ptr, c(0.1))
}

libflags <- function() {
  paste(
    "-L", shQuote(system.file("lib",package = "RcppParallel")),
    "-ltbb",
    "-Wl,-rpath,", shQuote(system.file("lib",package = "RcppParallel"))
  )
}

cxxflags <- function() {
  paste(
    "-I", shQuote(system.file("include", "libs", package = "rstan.dev")),
    "-I", shQuote(system.file("include", "libs", "sundials", package = "rstan.dev")),
    "-I", shQuote(system.file("include", "libs", "math", package = "rstan.dev")),
    "-I", shQuote(system.file("include", "cpp11", package = "rstan.dev")),
    "-I", shQuote(system.file("include",package = "RcppEigen")),
    "-I", shQuote(system.file("include",package = "RcppParallel")),
    "-I", shQuote(system.file("include",package = "BH")),
    "-D_REENTRANT",
    "-DRCPP_PARALLEL_USE_TBB=1",
    "-DSTAN_THREADS"
)
}

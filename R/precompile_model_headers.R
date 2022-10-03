#' Pre-compile header files used in all Stan models
#' @description Reduces model compilation time by pre-compiling repeated includes
#' @param force Whether to overwrite existing pre-compiled files
#' @export
precompile_model_headers <- function(force = TRUE) {
  cxx = gsub("\n", "",callr::rcmd("CONFIG", "CXX")$stdout, fixed = TRUE)

  dotdir <- file.path(Sys.getenv("HOME"), ".rstandev")
  if (force) {
    unlink(dotdir, recursive = TRUE)
  }
  if (!dir.exists(dotdir)) {
    dir.create(dotdir, recursive = TRUE)
  }

  meta_header_path <-system.file("include", "cpp11_meta_header.hpp",
                            package = "rstandev")
  model_header_path <-system.file("include", "stan_meta_header.hpp",
                                 package = "rstandev")

  file.copy(meta_header_path, dotdir)
  file.copy(model_header_path, dotdir)

  meta_header_path <- file.path(dotdir, "stan_meta_header.hpp")
  model_header_path <- file.path(dotdir, "cpp11_meta_header.hpp")
  flags <- paste0("-I", shQuote(Sys.getenv("R_INCLUDE_DIR")))
  flags <- c(flags, "-std=gnu++14", cxxflags(collapse = FALSE))
  flags <- c(flags, gsub("\n", "",callr::rcmd("CONFIG", "CXXFLAGS")$stdout, fixed = TRUE))

  system2(cxx, c(flags, shQuote(meta_header_path)))
  system2(cxx, c(flags, shQuote(model_header_path)))
}

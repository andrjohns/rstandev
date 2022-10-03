libflags <- function() {
  paste(
    paste0("-L", shQuote(dirname(RcppParallel::tbbLibraryPath("tbb")))),
    "-ltbb",
    paste0("-Wl,-rpath,", shQuote(RcppParallel::tbbLibraryPath("tbb"))),
    paste0("-L", shQuote(dirname(RcppParallel::tbbLibraryPath("tbbmalloc")))),
    "-ltbbmalloc",
    paste0("-Wl,-rpath,", shQuote(RcppParallel::tbbLibraryPath("tbbmalloc"))),
    "-L", shQuote(system.file("", package = "sundialr")),
    "-lsundials_all"
    #"-lOpencl"
  )
}

include_precomp_header <- function(type = "model") {
  name <- ifelse(type == "model", "stan_meta_header", "cpp11_meta_header")
  path <- file.path(Sys.getenv("HOME"), ".rstandev", paste0(name, ".hpp.gch"))
  if (!file.exists(path)) {
    warning("Pre-compiled headers are not present! Call precompile_model_headers()",
            call. = FALSE)
    return("")
  }
  paste("-include-pch", shQuote(path))
}

cxxflags <- function(collapse = TRUE, precomp_header = NULL) {
  flags <- ""
  if (!is.null(precomp_header)) {
    flags <- include_precomp_header(precomp_header)
  }
  flags <- c(
    flags,
    paste0("-I", shQuote(system.file("include", package = "rstandev"))),
    paste0("-I", shQuote(system.file("include", package = "Rcpp"))),
    paste0("-I", shQuote(system.file("include", package = "RcppEigen"))),
    paste0("-I", shQuote(system.file("include", package = "RcppParallel"))),
    paste0("-I", shQuote(system.file("include", package = "BH"))),
    paste0("-I", shQuote(system.file("include", package = "sundialr"))),
    paste0("-I", shQuote(system.file("include", package = "cpp11"))),
    paste0("-I", shQuote(system.file("include", package = "rapidjsonr"))),
    "-D_REENTRANT",
    "-DRCPP_PARALLEL_USE_TBB=1",
    "-DTBB_INTERFACE_NEW",
    "-DSTAN_THREADS",
    "-DEIGEN_PERMANENTLY_DISABLE_STUPID_WARNINGS",
    "-Wno-deprecated-builtins",
    "-Wno-deprecated-declarations",
    "-Wno-unknown-attributes",
    "-Wno-ignored-attributes",
    "-Wno-sign-compare",
    "-Wno-parentheses",
    "-Wno-unused-but-set-variable",
    "-Wno-unused-variable",
    "-Wno-unknown-warning-option"
    #,
    #"-DSTAN_OPENCL"
  )

  if (collapse) {
    paste(flags, collapse = " ")
  } else {
    flags
  }
}

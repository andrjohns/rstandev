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

cxxflags <- function() {
  paste(
    paste0("-I", shQuote(system.file("include", package = "rstandev"))),
    paste0("-I", shQuote(system.file("include", package = "Rcpp"))),
    paste0("-I", shQuote(system.file("include", package = "RcppEigen"))),
    paste0("-I", shQuote(system.file("include", package = "RcppParallel"))),
    paste0("-I", shQuote(system.file("include", package = "BH"))),
    paste0("-I", shQuote(system.file("include", package = "sundialr"))),
    paste0("-I", shQuote(system.file("include", package = "cpp11"))),
    "-D_REENTRANT",
    "-DRCPP_PARALLEL_USE_TBB=1",
    "-DSTAN_THREADS",
    "-Wno-deprecated-builtins",
    "-Wno-deprecated-declarations",
    "-Wno-unknown-attributes",
    "-Wno-ignored-attributes",
    "-Wno-sign-compare",
    "-Wno-parentheses",
    "-Wno-unused-but-set-variable",
    "-Wno-unused-variable"

    #,
    #"-DSTAN_OPENCL"
)
}

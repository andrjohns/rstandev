libflags <- function() {
  paste(
    "-L", shQuote(system.file("lib",package = "RcppParallel")),
    "-ltbb",
    "-Wl,-rpath,", shQuote(system.file("lib",package = "RcppParallel")),
    "-L", shQuote(system.file("", package = "sundialr")),
    "-lsundials_all"
    #"-lOpencl"
  )
}

cxxflags <- function() {
  paste(
    "-I", shQuote(system.file("include", "libs", package = "rstan.dev")),
    "-I", shQuote(system.file("include", "libs", "math", package = "rstan.dev")),
    "-I", shQuote(system.file("include", "cpp11", package = "rstan.dev")),
    "-I", shQuote(system.file("include", package = "rstan.dev")),
    "-I", shQuote(system.file("include",package = "RcppEigen")),
    "-I", shQuote(system.file("include",package = "RcppParallel")),
    "-I", shQuote(system.file("include",package = "BH")),
    "-I", shQuote(system.file("include",package = "sundialr")),
    "-I", shQuote(system.file("include",package = "cpp11")),
    "-D_REENTRANT",
    "-DRCPP_PARALLEL_USE_TBB=1",
    "-DSTAN_THREADS",
    "-Wno-deprecated-builtins",
    "-Wno-deprecated-declarations",
    "-Wno-unknown-attributes"

    #,
    #"-DSTAN_OPENCL"
)
}

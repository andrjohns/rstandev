libflags <- function() {
  paste(
    "-L", shQuote(system.file("lib",package = "RcppParallel")),
    "-ltbb",
    "-Wl,-rpath,", shQuote(system.file("lib",package = "RcppParallel"))#,
    #"-lOpencl"
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
    "-DSTAN_THREADS"#,
    #"-DSTAN_OPENCL"
)
}

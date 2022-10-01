libflags <- function() {
  paste(
    RcppParallel::LdFlags(),
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
    "-I", shQuote(system.file("include", package = "rstandev")),
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
    "-Wno-unknown-attributes",
    "-Wno-ignored-attributes",
    "-Wno-sign-compare"

    #,
    #"-DSTAN_OPENCL"
)
}

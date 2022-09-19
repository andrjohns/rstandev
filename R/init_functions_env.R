compile_functions_into_env <- function(cpp_locations) {
  env <- new.env()
  Sys.setenv("PKG_CXXFLAGS"=cxxflags())
  Sys.setenv("PKG_LIBS"=libflags())
  cpp11::cpp_source(
    file= cpp_locations$file,
    quiet=FALSE,
    cxx_std = "CXX14",
    dir = cpp_locations$dir,
    clean = FALSE,
    env = env)
  Sys.unsetenv("PKG_CXXFLAGS")
  Sys.unsetenv("PKG_LIBS")

  env
}
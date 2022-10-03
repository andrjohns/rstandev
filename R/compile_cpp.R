compile_cpp <- function(cpp_locations, env, quiet = TRUE, return_env = TRUE) {
  cpp11env <- rlang::ns_env("cpp11")
  dir <- cpp_locations$dir
  file <- basename(cpp_locations$file)
  name <- tools::file_path_sans_ext(file)

  src_dir <- file.path(dir, "src")
  r_dir <- file.path(dir, "R")
  if (dir.exists(src_dir)) {
    unlink(src_dir, recursive = TRUE)
  }
  if (dir.exists(r_dir)) {
    unlink(r_dir, recursive = TRUE)
  }
  dir.create(src_dir, recursive = TRUE)
  dir.create(r_dir, recursive = TRUE)
  file.copy(file.path(dir, file), src_dir)

  cpp_path <- file.path(src_dir, file)

  all_decorations <- decor::cpp_decorations(file = cpp_path,
                                          is_attribute = TRUE)

  cpp11env$check_valid_attributes(all_decorations, file = cpp_path)
  funs <- cpp11env$get_registered_functions(all_decorations, "cpp11::register")

  model_hpp <- readr::read_file(cpp_locations$file)
  exported_fun <- cpp11env$generate_cpp_functions(funs, package = name)
  model_hpp <- gsub("#include <stan/model/model_header.hpp>", "#include <stan_meta_header.hpp>", model_hpp, fixed = TRUE)
  cat(model_hpp, file = file.path(src_dir, file), sep = "\n")
  cat(c("#include <cpp11_meta_header.hpp>", exported_fun), file = file.path(src_dir, "cpp11.cpp"), sep = "\n")

  cat(paste0("PKG_CXXFLAGS=", cxxflags(precomp_header="meta")),
      paste0("PKG_LIBS=", libflags()),
      "CXX_STD=CXX14",
      file = file.path(src_dir, "Makevars"), sep = "\n")

  res1 <- callr::rcmd("COMPILE", "cpp11.cpp", user_profile = TRUE, show = !quiet, wd = src_dir)

  cat(paste0("PKG_CXXFLAGS=", cxxflags(precomp_header="model")),
      paste0("PKG_LIBS=", libflags()),
      "CXX_STD=CXX14",
      file = file.path(src_dir, "Makevars"), sep = "\n", append = FALSE)

  source_files <- c(paste0(name, ".cpp"), "cpp11.o")

  res <- callr::rcmd("SHLIB", source_files, user_profile = TRUE, show = !quiet, wd = src_dir)
  r_functions <- cpp11env$generate_r_functions(funs, package = name, use_package = TRUE)
  cat(r_functions, file = file.path(r_dir, "cpp11.R"))
  source(file.path(r_dir, "cpp11.R"), local = env)
  dyn.load(file.path(src_dir, paste0(name, .Platform$dynlib.ext)), local = TRUE, now = TRUE)

  if (return_env) {
    env
  } else {
    invisible(NULL)
  }
}

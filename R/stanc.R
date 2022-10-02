stanc <- function(model_code, standalone_functions = FALSE,
                  use_opencl = FALSE, optim_level = 0,
                  allow_undefined = FALSE, warn_pedantic = FALSE,
                  warn_uninitialized = FALSE, external_cpp = NULL) {
  if (model_code == "") {
    stop("No model code provided!", call. = FALSE)
  }

  stanc_flags = c(
    ifelse(allow_undefined, "allow-undefined", ""),
    paste0("O", optim_level),
    ifelse(standalone_functions, "standalone-functions",""),
    ifelse(use_opencl, "use-opencl", ""),
    ifelse(warn_pedantic, "warn-pedantic", ""),
    ifelse(warn_uninitialized, "warn-uninitialized", "")
  )

  res <- stanc_context$call("stanc", "model", model_code,
                            as.array(stanc_flags[!(stanc_flags == "")]))

  if (!is.null(res$errors)) {
    errors <- paste(res$errors, collapse = "\n")
    stop(errors, call. = FALSE)
  }

  if (length(res$warnings) > 0) {
    warnings <- paste(warnings, collapse = "\n")
    warning(warnings, call. = FALSE)
  }
  model_hpp <- res$result
  if (!is.null(external_cpp)) {
    model_lines <- strsplit(model_hpp, split = "\n")[[1]]
    nlines <- length(model_lines)

    math_decl <- grep("using namespace stan::math;", model_lines, fixed = TRUE)
    all_includes <- paste(sapply(external_cpp, readr::read_file), collapse = "\n")
    model_lines <- c(model_lines[1:math_decl], all_includes, model_lines[(math_decl+1):nlines])
    paste(model_lines, collapse = "\n")
  } else {
    model_hpp
  }
}

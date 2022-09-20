get_plain_rtn <- function(fun_body, model_lines) {
  fun_props <- decor::parse_cpp_function(paste(fun_body[-1], collapse="\n"))
  struct_start <- grep(paste0("struct ", fun_props$name, "_functor"), model_lines)
  struct_op_start <- grep("operator()", model_lines[-(1:struct_start)])[1] + struct_start

  struct_rtn <- grep("nullptr>", model_lines[struct_start:struct_op_start], fixed = TRUE) + struct_start

  rtn_type <- paste0(model_lines[struct_rtn:struct_op_start], collapse = " ")
  rm_trailing_nullptr <- gsub(".*nullptr>[^,]", "", rtn_type)
  rm_operator <- gsub("operator().*", "", rtn_type)
  repl_dbl <- gsub("stan::promote_args_t<stan::base_type_t<T0__>>", "double", rm_operator)
  gsub("(^\\s|\\s$)", "", repl_dbl)
}

prep_fun_cpp <- function(fun_body, model_lines) {
  fun_body <- gsub("auto", get_plain_rtn(fun_body, model_lines), fun_body)
  fun_body <- gsub("// [[stan::function]]", "[[cpp11::register]]", fun_body, fixed = TRUE)
  fun_body <- gsub("std::ostream* pstream__ = nullptr", "", fun_body, fixed = TRUE)
  fun_body <- gsub("pstream__", "nullptr", fun_body, fixed = TRUE)
  fun_body <- paste(fun_body, collapse = "\n")
  gsub(pattern = ",\\s*)", replacement = ")", fun_body)
}

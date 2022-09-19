std::shared_ptr<stan::io::var_context> var_context(std::string rdump_string) {
  std::istringstream rdump_stream(rdump_string);
  stan::io::dump data_context(rdump_stream);
  return std::make_shared<stan::io::dump>(data_context);
}

[[cpp11::register]]
SEXP new_model(std::string rdump_string,
                unsigned int seed = 0) {
  cpp11::external_pointer<stan_model> ptr(
      new stan_model(
          *var_context(rdump_string),
          seed,
          &std::cout)
  );
  return ptr;
}

model_ptr_t get_model_ptr(cpp11::list args_list) {
  if (args_list["model_ptr"] != R_NilValue) {
    return model_ptr_t(args_list["model_ptr"]);
  } else {
    std::string rdump_string = cpp11::as_cpp<std::string>(args_list["rdump_string"]);
    return model_ptr_t(new_model(rdump_string));
  }
}

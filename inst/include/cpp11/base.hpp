cpp11::external_pointer<stan::io::dump> var_context(std::string rdump_string) {
  std::istringstream rdump_stream(rdump_string);
  cpp11::external_pointer<stan::io::dump> var_ptr(new stan::io::dump(rdump_stream));
  return var_ptr;
}

cpp11::external_pointer<stan_model> new_model(std::string rdump_string, unsigned int seed) {
  std::istringstream rdump_stream(rdump_string);
  stan::io::dump var_context(rdump_stream);
  std::shared_ptr<stan::io::var_context> data_ptr
    = std::make_shared<stan::io::dump>(var_context);
  cpp11::external_pointer<stan_model> ptr(
      new stan_model(
          *data_ptr,
          seed,
          &std::cout)
  );
  return ptr;
}

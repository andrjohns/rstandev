[[cpp11::register]]
cpp11::strings get_param_names(cpp11::list args_list) {
  model_ptr_t ptr = get_model_ptr(args_list);
  std::vector<std::string> names;
  ptr->get_param_names(names);
  cpp11::writable::strings ret = std::move(names);
  return ret;
}

[[cpp11::register]]
cpp11::list get_dims(cpp11::list args_list) {
  model_ptr_t ptr = get_model_ptr(args_list);
  std::vector<std::vector<size_t>> dims;
  ptr->get_dims(dims);
  std::vector<std::string> names = cpp11::as_cpp<std::vector<std::string>>(get_param_names(args_list));
  cpp11::writable::list dims_list;

  for (int i = 0; i < names.size(); ++i) {
    dims_list.push_back({
      cpp11::named_arg(names[i].data()) = dims[i]
    });
  }
  return dims_list;
}

[[cpp11::register]]
cpp11::doubles unconstrain_pars(cpp11::list args_list) {
  model_ptr_t ptr = get_model_ptr(args_list);
  std::string init_string = cpp11::as_cpp<std::string>(args_list["init_string"]);
  std::shared_ptr<stan::io::var_context> init_context
    = var_context(init_string);
  std::vector<int> params_i;
  std::vector<double> vars;
  ptr->transform_inits(*init_context.get(), params_i, vars);
  cpp11::writable::doubles ret = std::move(vars);
  return ret;
}

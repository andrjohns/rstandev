[[cpp11::register]]
double log_prob(std::string rdump_string, std::vector<double> upars) {
  cpp11::external_pointer<stan_model> ptr = new_model(rdump_string, 1);
  Eigen::VectorXd upars_vec = stan::math::to_vector(upars);
  return ptr->log_prob<false, true>(upars_vec);
}

[[cpp11::register]]
cpp11::writable::doubles grad_log_prob(std::string rdump_string, std::vector<double> upars) {
  cpp11::external_pointer<stan_model> ptr = new_model(rdump_string, 1);
  std::vector<double> gradients;
  std::vector<int> dummy_i;
  double lp = stan::model::log_prob_grad<false, true>(
    *ptr.get(), upars, dummy_i, gradients);
  cpp11::writable::doubles grad_rtn = std::move(gradients);
  grad_rtn.attr("log_prob") = lp;
  return grad_rtn;
}

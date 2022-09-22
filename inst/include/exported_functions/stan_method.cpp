
#include <helpers/method_wrappers.hpp>
#include <helpers/init_services.hpp>

[[cpp11::register]]
int stan_methods_wrapper(std::string name, cpp11::writable::list args) {
  stan::math::init_threadpool_tbb(cpp11::as_cpp<size_t>(args["num_threads"]));
  auto services = rstandev::init_services(args);

  args.push_back({cpp11::named_arg("init_contexts") = std::get<0>(services)});
  args.push_back({cpp11::named_arg("interrupt_ptr") = std::get<1>(services)});
  args.push_back({cpp11::named_arg("str_logger_ptr") = std::get<2>(services)});
  args.push_back({cpp11::named_arg("init_writer_ptr") = std::get<3>(services)});
  args.push_back({cpp11::named_arg("samp_writer_ptr") = std::get<4>(services)});
  args.push_back({cpp11::named_arg("diag_writer_ptr") = std::get<5>(services)});

  auto service_function = rstandev::lookup_function(name);
  return service_function(args);
}
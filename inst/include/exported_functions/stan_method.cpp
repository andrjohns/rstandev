
#include <helpers/method_wrappers.hpp>
#include <helpers/r_interrupt_functor.hpp>
#include <helpers/make_init_contexts.hpp>


[[cpp11::register]]
int stan_methods_wrapper(std::string name, cpp11::writable::list args) {
  size_t num_chains = cpp11::as_cpp<size_t>(args["num_chains"]);
  stan::math::init_threadpool_tbb(cpp11::as_cpp<size_t>(args["num_threads"]));

  cpp11::external_pointer<rstandev::R_CheckUserInterrupt_Functor> interrupt_ptr(new rstandev::R_CheckUserInterrupt_Functor);
  cpp11::external_pointer<stan::callbacks::stream_logger> str_logger_ptr(new stan::callbacks::stream_logger(std::cout, std::cout, std::cout, std::cerr, std::cerr));

  cpp11::external_pointer<std::vector<stan::callbacks::writer>> init_writer_ptr(
    new std::vector<stan::callbacks::writer>{num_chains, stan::callbacks::writer{}}
  );

  std::string output = cpp11::as_cpp<std::string>(args["output"]);

  cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>
  sample_writers(new std::vector<stan::callbacks::unique_stream_writer<std::ostream>>);
  sample_writers->reserve(num_chains);

  cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>
  diagnostic_writers(new std::vector<stan::callbacks::unique_stream_writer<std::ostream>>);
  diagnostic_writers->reserve(num_chains);


  for (int i = 0; i < num_chains; ++i) {
    auto output_filename = output + "_" + std::to_string(i) + ".csv";
    auto unique_fstream
        = std::make_unique<std::fstream>(output_filename, std::fstream::out);

    sample_writers->emplace_back(std::move(unique_fstream), "# ");
    diagnostic_writers->emplace_back(nullptr, "# ");
  }

  using namespace cpp11::literals;
  args.push_back({"init_contexts"_nm = rstandev::make_init_contexts(args)});
  args.push_back({"interrupt_ptr"_nm = interrupt_ptr});
  args.push_back({"str_logger_ptr"_nm = str_logger_ptr});
  args.push_back({"init_writer_ptr"_nm = init_writer_ptr});
  args.push_back({"samp_writer_ptr"_nm = sample_writers});
  args.push_back({"diag_writer_ptr"_nm = diagnostic_writers});

  auto service_function = rstandev::lookup_function(name);
  return service_function(args);
  //return 1;
}
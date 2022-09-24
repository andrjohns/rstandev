#ifndef RSTANDEV_HELPERS_MAKE_SAMPLE_WRITER_HPP
#define RSTANDEV_HELPERS_MAKE_SAMPLE_WRITER_HPP

#include <helpers/var_context.hpp>
#include <cpp11/list.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/as.hpp>
#include <vector>
#include <memory>


namespace rstandev {
  template <typename ServiceFunctionT>
  cpp11::external_pointer<stan::callbacks::unique_stream_writer<std::ostream>>
  make_sample_writer(size_t chain, std::string output_dir) {

    auto output_filename = output_dir + "/output_" + std::to_string(chain) + ".csv";
    auto unique_fstream
        = std::make_unique<std::fstream>(output_filename, std::fstream::out);

    cpp11::external_pointer<stan::callbacks::unique_stream_writer<std::ostream>>
    sample_writers(new stan::callbacks::unique_stream_writer<std::ostream>(std::move(unique_fstream), "# "));
    return sample_writers;
  }
}

#endif

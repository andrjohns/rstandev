#ifndef RSTANDEV_HELPERS_MAKE_DIAG_WRITER_HPP
#define RSTANDEV_HELPERS_MAKE_DIAG_WRITER_HPP


namespace rstandev {
  template <typename F>
  auto make_diag_writer(size_t num_chains) {
    cpp11::external_pointer<stan::callbacks::unique_stream_writer<std::ostream>>
    diagnostic_writers(new stan::callbacks::unique_stream_writer<std::ostream>(nullptr, "# "));

    return diagnostic_writers;
  }
}

#endif
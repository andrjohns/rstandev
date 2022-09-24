#ifndef RSTANDEV_HELPERS_MAKE_DIAG_WRITER_HPP
#define RSTANDEV_HELPERS_MAKE_DIAG_WRITER_HPP


namespace rstandev {
  template <typename F, stan::require_t<is_multi_chain<F>>* = nullptr>
  auto make_diag_writer(size_t num_chains) {
    cpp11::external_pointer<std::vector<stan::callbacks::unique_stream_writer<std::ostream>>>
    diagnostic_writers(new std::vector<stan::callbacks::unique_stream_writer<std::ostream>>);
    diagnostic_writers->reserve(num_chains);

    for (int i = 0; i < num_chains; ++i) {
      diagnostic_writers->emplace_back(nullptr, "# ");
    }

    return diagnostic_writers;
  }

  template <typename F, stan::require_not_t<is_multi_chain<F>>* = nullptr>
  auto make_diag_writer(size_t num_chains) {
    cpp11::external_pointer<stan::callbacks::unique_stream_writer<std::ostream>>
    diagnostic_writers(new stan::callbacks::unique_stream_writer<std::ostream>(nullptr, "# "));

    return diagnostic_writers;
  }
}

#endif
#ifndef RSTANDEV_HELPERS_MAKE_INIT_WRITER_HPP
#define RSTANDEV_HELPERS_MAKE_INIT_WRITER_HPP


namespace rstandev {
  template <typename ServiceFunctionT, stan::require_t<is_multi_chain<ServiceFunctionT>>* = nullptr>
  cpp11::external_pointer<std::vector<stan::callbacks::writer>>
  make_init_writer(size_t chains) {
    return cpp11::external_pointer<std::vector<stan::callbacks::writer>>(
      new std::vector<stan::callbacks::writer>{chains, stan::callbacks::writer{}}
    );
  }

  template <typename ServiceFunctionT, stan::require_not_t<is_multi_chain<ServiceFunctionT>>* = nullptr>
  cpp11::external_pointer<stan::callbacks::writer>
  make_init_writer(size_t chains) {
    return cpp11::external_pointer<stan::callbacks::writer>(
      new stan::callbacks::writer
    );
  }
}

#endif


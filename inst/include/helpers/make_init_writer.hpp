#ifndef RSTANDEV_HELPERS_MAKE_INIT_WRITER_HPP
#define RSTANDEV_HELPERS_MAKE_INIT_WRITER_HPP


namespace rstandev {
  template <typename ServiceFunctionT>
  cpp11::external_pointer<stan::callbacks::writer>
  make_init_writer(size_t chains) {
    return cpp11::external_pointer<stan::callbacks::writer>(
      new stan::callbacks::writer
    );
  }
}

#endif



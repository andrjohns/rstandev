struct my_stringbuf : public std::basic_stringbuf<char> {
  my_stringbuf(std::basic_stringbuf<char>* other_buf) {
    other_buf->swap(*this);
  }
  const char* my_str() const { return pbase(); }
};

using writer_t = std::vector<stan::callbacks::unique_stream_writer<std::ostringstream>>;


struct R_CheckUserInterrupt_Functor : public stan::callbacks::interrupt {
  void operator()() {
    R_CheckUserInterrupt();
  }
};

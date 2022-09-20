struct R_CheckUserInterrupt_Functor : public stan::callbacks::interrupt {
  void operator()() {
    R_CheckUserInterrupt();
  }
};

using model_ptr_t = cpp11::external_pointer<stan_model>;

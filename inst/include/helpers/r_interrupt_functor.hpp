#ifndef RSTANDEV_HELPERS_R_INTERRUPT_FUNCTOR_HPP
#define RSTANDEV_HELPERS_R_INTERRUPT_FUNCTOR_HPP

#include <stan/callbacks/interrupt.hpp>

namespace rstandev {
  struct R_CheckUserInterrupt_Functor : public stan::callbacks::interrupt {
    void operator()() {
      R_CheckUserInterrupt();
    }
  };
} // namespace rstandev

#endif

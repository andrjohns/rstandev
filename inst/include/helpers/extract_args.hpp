#ifndef RSTANDEV_HELPERS_EXTRACT_ARGS_HPP
#define RSTANDEV_HELPERS_EXTRACT_ARGS_HPP

#include <stan/math/prim/meta/index_apply.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/list.hpp>
#include <cpp11/as.hpp>
#include <tuple>

namespace rstandev {
  template <typename ArgTypesTuple>
  decltype(auto) extract_args(const ArgTypesTuple& args_tuple, const cpp11::list& args_list) {
    return stan::math::index_apply<std::tuple_size<std::decay_t<ArgTypesTuple>>::value>(
        [args_list, &args_tuple](auto... Is) {
          return std::make_tuple(
              cpp11::as_cpp<std::decay_t<typename std::decay_t<decltype(std::get<Is>(args_tuple))>::arg_type>>(
                args_list[std::get<Is>(args_tuple).arg_name]
            )...);
        });
  }
}

#endif

#ifndef RSTANDEV_HELPERS_EXTRACT_ARGS_HPP
#define RSTANDEV_HELPERS_EXTRACT_ARGS_HPP

#include <stan/math/prim/meta/index_apply.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/list.hpp>
#include <cpp11/as.hpp>
#include <tuple>

namespace rstandev {
  template <typename T>
  decltype(auto) dereference(cpp11::external_pointer<T>&& arg) {
    return *arg;
  }
  template <typename T>
  decltype(auto) dereference(T&& arg) {
    return std::forward<T>(arg);
  }

  template <typename ArgTypesTuple>
  auto extract_args(const cpp11::list& args_list, const char* ArgNames[]) {
    return stan::math::index_apply<std::tuple_size<std::decay_t<ArgTypesTuple>>::value>(
        [&args_list, &ArgNames](auto... Is) {
          return std::forward_as_tuple(
            dereference(
              cpp11::as_cpp<decltype(std::get<Is>(std::declval<ArgTypesTuple>()))>(
                args_list[ArgNames[Is]]
              )
            )...);
        });
  }
}

#endif

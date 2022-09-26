#ifndef RSTANDEV_HELPERS_GET_MODEL_PTR_HPP
#define RSTANDEV_HELPERS_GET_MODEL_PTR_HPP

#include <helpers/new_model.hpp>
#include <cpp11/list.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/as.hpp>
#include <string>

namespace rstandev {
  template <typename StanModelT>
  cpp11::external_pointer<StanModelT> get_model_ptr(cpp11::list args_list) {
    if (args_list["model_ptr"] != R_NilValue) {
      return cpp11::external_pointer<StanModelT>(args_list["model_ptr"]);
    } else {
      std::string rdump_string = cpp11::as_cpp<std::string>(args_list["rdump_data"]);
      return cpp11::external_pointer<StanModelT>(new_model<StanModelT>(rdump_string));
    }
  }
} // namespace rstandev

# endif

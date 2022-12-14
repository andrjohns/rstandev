#ifndef RSTANDEV_HELPERS_VAR_CONTEXT_HPP
#define RSTANDEV_HELPERS_VAR_CONTEXT_HPP

#include <stan/io/json/json_data.hpp>
#include <memory>

namespace rstandev {
  std::shared_ptr<stan::io::var_context> var_context(std::string rdump_string) {
    std::istringstream rdump_stream(rdump_string);
    stan::json::json_data data_context(rdump_stream);
    return std::make_shared<stan::json::json_data>(data_context);
  }
} // namespace rstandev

#endif

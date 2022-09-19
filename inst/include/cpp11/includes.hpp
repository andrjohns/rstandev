// Stan
#include <stan/model/log_prob_grad.hpp>
#include <stan/callbacks/interrupt.hpp>
#include <stan/callbacks/stream_logger.hpp>
#include <stan/callbacks/writer.hpp>
#include <stan/callbacks/unique_stream_writer.hpp>
#include <stan/services/sample/hmc_nuts_dense_e_adapt.hpp>

// cpp11
#include <cpp11.hpp>

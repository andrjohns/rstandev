// Stan
#include <stan/model/log_prob_grad.hpp>
#include <stan/callbacks/interrupt.hpp>
#include <stan/callbacks/stream_logger.hpp>
#include <stan/callbacks/writer.hpp>
#include <stan/callbacks/unique_stream_writer.hpp>
#include <stan/services/sample/hmc_nuts_dense_e_adapt.hpp>

// cpp11
#include <cpp11/R.hpp>
#include <cpp11/external_pointer.hpp>
#include <cpp11/doubles.hpp>
#include <cpp11/strings.hpp>
#include <cpp11/sexp.hpp>
#include <cpp11/list.hpp>
#include <cpp11/as.hpp>

#ifndef RSTANDEV_HELPERS_AS_CPP_EIGEN_HPP
#define RSTANDEV_HELPERS_AS_CPP_EIGEN_HPP

#include <stan/math/prim/fun/Eigen.hpp>
#include <cpp11/matrix.hpp>
#include <cpp11/doubles.hpp>
#include <cpp11/sexp.hpp>
#include <cpp11/as.hpp>
#include <cpp11/list.hpp>
#include <boost/preprocessor.hpp>

#define TO_SEQ_ELEM(z, n, data) (n)
#define MAKE_INTEGER_SEQUENCE(n) BOOST_PP_REPEAT(n, TO_SEQ_ELEM, )

#define STD_VECTOR_DEF(TYPE) std::vector<TYPE>

#define OP(s, state, x) STD_VECTOR_DEF(state)

#define NESTED_STD_TYPE(EIG_TYPE, LEVELS) \
  BOOST_PP_SEQ_FOLD_LEFT(OP, EIG_TYPE, BOOST_PP_SEQ_TAIL(MAKE_INTEGER_SEQUENCE(LEVELS)))

#define NESTED_EIG_DECL(EIG_TYPE, LEVELS) \
  template <> \
  NESTED_STD_TYPE(EIG_TYPE, LEVELS) as_cpp<NESTED_STD_TYPE(EIG_TYPE, LEVELS)>(SEXP cpp) { \
    cpp11::writable::list cpp11_obj = cpp; \
    NESTED_STD_TYPE(EIG_TYPE, LEVELS) rtn; \
    for (auto&& elem : cpp11_obj) { \
      rtn.push_back(as_cpp<std::decay_t<decltype(rtn[0])>>(elem)); \
    } \
    return rtn; \
  } \

#define NESTED_EIG_SEXP_DECL(EIG_TYPE, LEVELS) \
  auto as_sexp(const NESTED_STD_TYPE(EIG_TYPE, LEVELS)& eigen_obj) { \
    cpp11::writable::list eig_array; \
    for (auto&& eig : eigen_obj) { \
      eig_array.push_back(as_sexp(eig)); \
    } \
    return as_sexp(eig_array); \
  } \

namespace cpp11 {
  template <>
  Eigen::MatrixXd as_cpp<Eigen::MatrixXd>(SEXP cpp) {
    cpp11::writable::doubles_matrix<cpp11::by_column> cpp11_obj(cpp);
    return Eigen::Map<Eigen::MatrixXd>(REAL(cpp11_obj.data()), cpp11_obj.nrow(), cpp11_obj.ncol());
  }

  template <>
  Eigen::VectorXd as_cpp<Eigen::VectorXd>(SEXP cpp) {
    cpp11::writable::doubles cpp11_obj(cpp);
    return Eigen::Map<Eigen::VectorXd>(REAL(cpp11_obj.data()), cpp11_obj.size());
  }

  template <>
  Eigen::RowVectorXd as_cpp<Eigen::RowVectorXd>(SEXP cpp) {
    cpp11::writable::doubles cpp11_obj(cpp);
    return Eigen::Map<Eigen::RowVectorXd>(REAL(cpp11_obj.data()), cpp11_obj.size());
  }

  inline auto as_sexp(const Eigen::VectorXd& eigen_obj) {
    cpp11::writable::doubles dbls({eigen_obj.data(), eigen_obj.data() + eigen_obj.size()});
    return as_sexp(dbls);
  }

  inline auto as_sexp(const Eigen::RowVectorXd& eigen_obj) {
    cpp11::writable::doubles dbls({eigen_obj.data(), eigen_obj.data() + eigen_obj.size()});
    return as_sexp(dbls);
  }

  inline auto as_sexp(const Eigen::MatrixXd& eigen_obj) {
    cpp11::writable::doubles_matrix<cpp11::by_column> mat(eigen_obj.rows(), eigen_obj.cols());
    for (size_t j = 0; j < eigen_obj.cols(); ++j) {
      for (size_t i = 0; i < eigen_obj.rows(); ++i) {
        mat(i, j) = eigen_obj(i, j);
      }
    }
    return as_sexp(mat);
  }

  NESTED_EIG_DECL(Eigen::VectorXd, 2)
  NESTED_EIG_DECL(Eigen::VectorXd, 3)
  NESTED_EIG_DECL(Eigen::VectorXd, 4)
  NESTED_EIG_DECL(Eigen::VectorXd, 5)

  NESTED_EIG_DECL(Eigen::RowVectorXd, 2)
  NESTED_EIG_DECL(Eigen::RowVectorXd, 3)
  NESTED_EIG_DECL(Eigen::RowVectorXd, 4)
  NESTED_EIG_DECL(Eigen::RowVectorXd, 5)

  NESTED_EIG_DECL(Eigen::MatrixXd, 2)
  NESTED_EIG_DECL(Eigen::MatrixXd, 3)
  NESTED_EIG_DECL(Eigen::MatrixXd, 4)
  NESTED_EIG_DECL(Eigen::MatrixXd, 5)

  NESTED_EIG_SEXP_DECL(Eigen::VectorXd, 2)
  NESTED_EIG_SEXP_DECL(Eigen::VectorXd, 3)
  NESTED_EIG_SEXP_DECL(Eigen::VectorXd, 4)
  NESTED_EIG_SEXP_DECL(Eigen::VectorXd, 5)

  NESTED_EIG_SEXP_DECL(Eigen::RowVectorXd, 2)
  NESTED_EIG_SEXP_DECL(Eigen::RowVectorXd, 3)
  NESTED_EIG_SEXP_DECL(Eigen::RowVectorXd, 4)
  NESTED_EIG_SEXP_DECL(Eigen::RowVectorXd, 5)

  NESTED_EIG_SEXP_DECL(Eigen::MatrixXd, 2)
  NESTED_EIG_SEXP_DECL(Eigen::MatrixXd, 3)
  NESTED_EIG_SEXP_DECL(Eigen::MatrixXd, 4)
  NESTED_EIG_SEXP_DECL(Eigen::MatrixXd, 5)
} //namespace cpp11

#endif

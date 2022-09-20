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

  template <>
  std::vector<Eigen::Matrix<double, -1, 1>> as_cpp<std::vector<Eigen::Matrix<double, -1, 1>>>(SEXP cpp) {
    cpp11::writable::list cpp11_obj = cpp;
    std::vector<Eigen::Matrix<double, -1, 1>> rtn;
    for (auto&& elem : cpp11_obj) {
      rtn.push_back(as_cpp<Eigen::Matrix<double, -1, 1>>(elem));
    }
    return rtn;
  }

  template <>
  std::vector<Eigen::Matrix<double, 1, -1>> as_cpp<std::vector<Eigen::Matrix<double, 1, -1>>>(SEXP cpp) {
    cpp11::writable::list cpp11_obj = cpp;
    std::vector<Eigen::Matrix<double, 1, -1>> rtn;
    for (auto&& elem : cpp11_obj) {
      rtn.push_back(as_cpp<Eigen::Matrix<double, 1, -1>>(elem));
    }
    return rtn;
  }

  template <>
  std::vector<Eigen::Matrix<double, -1, -1>> as_cpp<std::vector<Eigen::Matrix<double, -1, -1>>>(SEXP cpp) {
    cpp11::writable::list cpp11_obj = cpp;
    std::vector<Eigen::Matrix<double, -1, -1>> rtn;
    for (auto&& elem : cpp11_obj) {
      rtn.push_back(as_cpp<Eigen::Matrix<double, -1, -1>>(elem));
    }
    return rtn;
  }

  template <typename T>
  std::enable_if_t<
    stan::is_std_vector<T>::value && stan::is_std_vector<stan::value_type_t<T>>::value,
    T>
  as_cpp(SEXP cpp) {
    cpp11::writable::list cpp11_obj = cpp;
    T rtn;
    for (auto&& elem : cpp11_obj) {
      rtn.push_back(as_cpp<stan::value_type_t<T>>(elem));
    }
    return rtn;
  }

  inline auto as_sexp(const Eigen::Matrix<double, -1, 1>& eigen_obj) {
    cpp11::writable::doubles dbls({eigen_obj.data(), eigen_obj.data() + eigen_obj.size()});
    return as_sexp(dbls);
  }

  inline auto as_sexp(const Eigen::Matrix<double, 1, -1>& eigen_obj) {
    cpp11::writable::doubles dbls({eigen_obj.data(), eigen_obj.data() + eigen_obj.size()});
    return as_sexp(dbls);
  }

  inline auto as_sexp(const Eigen::Matrix<double, -1, -1>& eigen_obj) {
    cpp11::writable::doubles_matrix<cpp11::by_column> mat(eigen_obj.rows(), eigen_obj.cols());
    for (size_t j = 0; j < eigen_obj.cols(); ++j) {
      for (size_t i = 0; i < eigen_obj.rows(); ++i) {
        mat(i, j) = eigen_obj(i, j);
      }
    }
    return as_sexp(mat);
  }

  template <int R, int C>
  inline auto as_sexp(const std::vector<Eigen::Matrix<double, R, C>>& eigen_obj) {
    cpp11::writable::list eig_array;
    for (auto&& eig : eigen_obj) {
      eig_array.push_back(as_sexp(eig));
    }
    return as_sexp(eig_array);
  }
}

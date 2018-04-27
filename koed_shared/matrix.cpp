#include "matrix.h"

using namespace koed_shared;
using namespace std;

// static
Matrix Matrix::BuildIdentity(size_t n) {
  Matrix matrix(n);
  for (size_t i = 0; i < matrix.size(); i++) {
    matrix[i][i] = 1;
  }
  return move(matrix);
}

Matrix Matrix::BuildStandardizedMatrix() {
  Matrix x(size_n(), size_m());
  for (size_t i = 0; i < size_n(); i++) {
    for (size_t j = 0; j < size_m(); j++) {
      x[i][j] = ((*this)[i][j] - mean_of_values(GetColumn(j))) /
                sqrt(std_deviation_of_values(GetColumn(j)));
    }
  }
  return move(x);
}

Matrix Matrix::BuildCovarianceMatrix() {
  Matrix x(size_m(), size_m());
  for (size_t i = 0; i < x.size_n(); i++) {
    for (size_t j = 0; j < x.size_m(); j++) {
      for (size_t k = 0; k < size_n(); k++) {
        x[i][j] += ((*this)[k][i] - mean_of_values(GetColumn(i))) *
                   ((*this)[k][j] - mean_of_values(GetColumn(j)));
      }
      x[i][j] /= size_n();
    }
  }
  return move(x);
}

Matrix Matrix::BuildCorrelationMatrix() {
  auto x = [this](size_t i, size_t j) {
    double res = (*this)[i][j] - mean_of_values(GetColumn(j));
    return res / sqrt(dispersion_of_values(GetColumn(j)));
  };
  Matrix r(size_m(), size_m());
  for (size_t i = 0; i < r.size_m(); i++) {
    for (size_t j = 0; j < r.size_m(); j++) {
      for (size_t k = 0; k < size_n(); k++) {
        r[i][j] += x(k, i) * x(k, j);
      }
      r[i][j] /= size_n();
    }
  }
  return move(r);
}

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

Matrix Matrix::BuildTransposeMatrix() const {
  Matrix x(size_m(), size_n());
  for (size_t i = 0; i < size_n(); i++) {
    for (size_t j = 0; j < size_m(); j++) {
      x[j][i] = (*this)[i][j];
    }
  }
  return move(x);
}

Matrix koed_shared::operator*(const Matrix& a,
                              const Matrix& b) throw(MatrixMultException) {
  if (a.size_m() != b.size_n()) throw MatrixMultException();
  size_t l = a.size_m();
  Matrix res(a.size_n(), b.size_m());
  for (size_t i = 0; i < res.size_n(); i++) {
    for (size_t j = 0; j < res.size_m(); j++) {
      double val = 0.0;
      for (size_t k = 0; k < l; k++) {
        val += a[i][k] * b[k][j];
      }
      res[i][j] = val;
    }
  }
  return move(res);
}

Matrix koed_shared::operator+(const Matrix& a,
                              const Matrix& b) throw(MatrixSumException) {
  if (a.size_n() != b.size_n() || a.size_m() != b.size_m())
    throw MatrixSumException();
  Matrix res(a.size_n(), a.size_m());
  for (size_t i = 0; i < res.size_n(); i++) {
    for (size_t j = 0; j < res.size_m(); j++) {
      res[i][j] = a[i][j] + b[i][j];
    }
  }
  return move(res);
}
Matrix koed_shared::operator-(const Matrix& a,
                              const Matrix& b) throw(MatrixSumException) {
  if (a.size_n() != b.size_n() || a.size_m() != b.size_m())
    throw MatrixSumException();
  Matrix res(a.size_n(), a.size_m());
  for (size_t i = 0; i < res.size_n(); i++) {
    for (size_t j = 0; j < res.size_m(); j++) {
      res[i][j] = a[i][j] - b[i][j];
    }
  }
  return move(res);
}

bool koed_shared::operator==(const Matrix& a, const Matrix& b) {
  double eps = 1e-8;
  if (a.size_n() != b.size_n() || a.size_m() != b.size_m()) return false;
  for (size_t i = 0; i < a.size_n(); i++) {
    for (size_t j = 0; j < a.size_m(); j++) {
      if (abs(a[i][j] - b[i][j]) > eps) return false;
    }
  }
  return true;
}

Matrix Reverse(Matrix a) {
  if (a.size_m() != a.size_n()) throw MatrixMultException();
  size_t sz = a.size_m();
  Matrix res = Matrix::BuildIdentity(sz);

  for (size_t k = 0; k < sz; k++) {
    size_t maxValK = k;
    for (size_t i = k + 1; i < sz; i++) {
      if (a.GetColumn(k)[maxValK] < a.GetColumn(k)[i]) {
        maxValK = i;
      }
    }
    if (k != maxValK) {
      for (size_t i = 0; i < sz; i++) {
        a.GetRow(k)[i] += a.GetRow(maxValK)[i];
        res.GetRow(k)[i] += res.GetRow(maxValK)[i];
      }
    }

    if (a[k][k] == 0) throw MatrixMultException();

    for (size_t i = 0; i < sz; i++) {
      if (i == k) continue;
      double targ = a.GetRow(i)[k];
      double m = -(targ / a[k][k]);

      for (size_t j = 0; j < sz; j++) {
        a.GetRow(i)[j] += m * a.GetRow(k)[j];
        res.GetRow(i)[j] += m * res.GetRow(k)[j];
      }
    }
    double m = 1.0 / a[k][k];
    size_t i = k;
    for (size_t j = 0; j < sz; j++) {
      a.GetRow(i)[j] *= m;
      res.GetRow(i)[j] *= m;
    }
  }
  return res;
}

Matrix koed_shared::mpow(const Matrix& a, int p) throw(MatrixMultException) {
  if (a.size_m() != a.size_n()) throw MatrixMultException();
  if (p < -1) throw MatrixMultException();
  if (p == -1) {
    return Reverse(a);
  }
  if (p == 0) return Matrix::BuildIdentity(a.size());
  if (p == 1) return a;
  if (p % 2 == 0) {
    return mpow(a * a, p / 2);
  } else {
    return a * mpow(a, p - 1);
  }
  throw MatrixMultException();
}

// Stream
istream& koed_shared::operator>>(istream& in, Matrix& matrix) {
  for (auto& row : matrix) {
    for (auto& val : row) {
      in >> val;
    }
  }
  return in;
}
ostream& koed_shared::operator<<(ostream& out, Matrix& matrix) {
  for (auto& row : matrix) {
    for (auto& val : row) {
      out << val;
      if (&val != &row.back()) out << "\t";
    }
    if (&row != &matrix.back()) out << endl;
  }
  return out;
}

void MatrixColumn::push_back(double val) {
  if (owner->size_m() > 1) throw MatrixRectBroken{};
  owner->push_back({val});
  data.push_back(&owner->back().back());
}

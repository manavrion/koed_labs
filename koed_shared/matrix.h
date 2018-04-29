#pragma once
#include <algorithm>
#include <initializer_list>
#include <istream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <vector>
#include "matrix_alg.h"

namespace koed_shared {

struct Matrix;

struct MatrixMultException : public std::exception {};
struct MatrixSumException : public std::exception {};

struct MatrixRectBroken : public std::exception {};

struct MatrixRow final : std::vector<double> {
  MatrixRow(const std::vector<double>& list) : std::vector<double>(list) {}
  MatrixRow(const std::initializer_list<double>& list)
      : std::vector<double>(list) {}
  MatrixRow(size_t m, double def = 0) : std::vector<double>(m, def) {}
  double operator[](size_t i) const throw(std::out_of_range) { return at(i); }
  double& operator[](size_t i) throw(std::out_of_range) { return at(i); }
};

struct MatrixColumn final {
  MatrixColumn(std::vector<double*> data, Matrix* owner)
      : data(std::move(data)), owner(owner) {}
  double operator[](size_t i) const throw(std::out_of_range) {
    return *data.at(i);
  }
  double& operator[](size_t i) throw(std::out_of_range) { return *data.at(i); }
  size_t size() { return data.size(); }

  class iterator
      : public std::iterator<
            std::forward_iterator_tag,              // iterator_category
            double,                                 // value_type
            std::vector<double*>::difference_type,  // difference_type
            double*,                                // pointer
            double&                                 // reference
            > {
    std::vector<double*>::iterator it;

   public:
    explicit iterator(std::vector<double*>::iterator it) : it(it) {}
    iterator& operator++() {
      it++;
      return *this;
    }
    iterator operator++(int) {
      iterator retval = *this;
      ++(*this);
      return retval;
    }
    bool operator==(iterator other) const { return it == other.it; }
    bool operator!=(iterator other) const { return !(*this == other); }
    reference operator*() const { return *(*it); }
  };
  iterator begin() { return MatrixColumn::iterator(data.begin()); }
  iterator end() { return MatrixColumn::iterator(data.end()); }

  class reverse_iterator
      : public std::iterator<
            std::forward_iterator_tag,              // iterator_category
            double,                                 // value_type
            std::vector<double*>::difference_type,  // difference_type
            double*,                                // pointer
            double&                                 // reference
            > {
    std::vector<double*>::reverse_iterator it;

   public:
    explicit reverse_iterator(std::vector<double*>::reverse_iterator it)
        : it(it) {}
    reverse_iterator& operator++() {
      it++;
      return *this;
    }
    reverse_iterator operator++(int) {
      reverse_iterator retval = *this;
      ++(*this);
      return retval;
    }
    bool operator==(reverse_iterator other) const { return it == other.it; }
    bool operator!=(reverse_iterator other) const { return !(*this == other); }
    reference operator*() const { return *(*it); }
  };
  reverse_iterator rbegin() {
    return MatrixColumn::reverse_iterator(data.rbegin());
  }
  reverse_iterator rend() {
    return MatrixColumn::reverse_iterator(data.rend());
  }

  void push_back(double val);

  double& back() { return *data.back(); }

 private:
  std::vector<double*> data;
  Matrix* owner;
};

inline MatrixColumn::iterator begin(MatrixColumn& m) { return m.begin(); }
inline MatrixColumn::iterator end(MatrixColumn& m) { return m.end(); }

struct Matrix final : std::vector<MatrixRow> {
  Matrix(std::vector<std::vector<double>> list)
      : std::vector<MatrixRow>(list.begin(), list.end()) {
    size_t size = 0;
    for (auto& l : list) {
      size = std::max(size, l.size());
    }
    for (auto& ob : *this) {
      ob.resize(size);
    }
  }
  Matrix(std::initializer_list<std::initializer_list<double>> list)
      : std::vector<MatrixRow>(list.begin(), list.end()) {
    size_t size = 0;
    for (auto& l : list) {
      size = std::max(size, l.size());
    }
    for (auto& ob : *this) {
      ob.resize(size);
    }
  }
  Matrix(size_t n, size_t m) : std::vector<MatrixRow>(n, MatrixRow(m)) {}
  Matrix(size_t n) : Matrix(n, n) {}
  MatrixRow& operator[](size_t i) throw(std::out_of_range) { return at(i); }
  const MatrixRow& operator[](size_t i) const throw(std::out_of_range) {
    return at(i);
  }

  static Matrix BuildIdentity(size_t n);

  Matrix BuildStandardizedMatrix();
  Matrix BuildCovarianceMatrix();
  Matrix BuildCorrelationMatrix();

  // Sizes
  size_t size_of_column() const { return size(); }  // BAD, TODO fix
  size_t size_of_row() const { return (size() == 0 ? 0 : front().size()); }
  size_t size_n() const { return size(); }
  size_t size_m() const { return (size() == 0 ? 0 : front().size()); }

  // Getters
  MatrixRow& GetRow(size_t i) throw(std::out_of_range) { return at(i); }
  MatrixColumn GetColumn(size_t j) throw(std::out_of_range) {
    std::vector<double*> columns;
    for (size_t i = 0; i < size_n(); i++) {
      columns.push_back(&(*this)[i][j]);
    }
    return MatrixColumn(columns, this);
  }

  MatrixColumn AsVector() throw(std::out_of_range) { return GetColumn(0); }

  // Math
  Matrix BuildTransposeMatrix() const;

  Matrix operator-() const {
    Matrix m = (*this);
    for (size_t i = 0; i < size_n(); i++) {
      for (size_t j = 0; j < size_m(); j++) {
        m[i][j] *= -1;
      }
    }
    return move(m);
  }

  bool IsIdentity(double eps) const {
    for (size_t i = 0; i < size_n(); i++) {
      for (size_t j = 0; j < size_m(); j++) {
        if (i == j) {
          if (abs(1 - (*this)[i][j]) > eps) return false;
        } else {
          if (abs((*this)[i][j]) > eps) return false;
        }
      }
    }
    return true;
  }

  bool IsZero(double eps) const {
    for (size_t i = 0; i < size_n(); i++) {
      for (size_t j = 0; j < size_m(); j++) {
        if (abs((*this)[i][j]) > eps) return false;
      }
    }
    return true;
  }

  bool Equals(const Matrix& b, double eps) const {
    const Matrix& a = (*this);
    if (a.size_n() != b.size_n() || a.size_m() != b.size_m()) return false;
    for (size_t i = 0; i < a.size_n(); i++) {
      for (size_t j = 0; j < a.size_m(); j++) {
        if (abs(a[i][j] - b[i][j]) > eps) return false;
      }
    }
    return true;
  }

  // TODO
  void push_back_column(double val) {
    for (auto& row : (*this)) {
      row.push_back(val);
    }
  }
};

Matrix operator*(const Matrix& a, const Matrix& b) throw(MatrixMultException);
Matrix operator+(const Matrix& a, const Matrix& b) throw(MatrixSumException);
Matrix operator-(const Matrix& a, const Matrix& b) throw(MatrixSumException);

bool operator==(const Matrix& a, const Matrix& b);

Matrix mpow(const Matrix& a, int p) throw(MatrixMultException);

std::istream& operator>>(std::istream& in, Matrix& matrix);
std::ostream& operator<<(std::ostream& out, Matrix& matrix);

}  // namespace koed_shared

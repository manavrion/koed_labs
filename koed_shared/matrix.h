#pragma once
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace koed_shared {

template <class C>
double mean_of_values(C c) {
  return mean_of_values(begin, end);
}

template <class I>
double mean_of_values(I begin, I end) {
  auto sum = std::accumulate(begin, end, 0.0);
  return sum / std::distance(begin, end);
}

struct MatrixRow final : std::vector<double> {
  MatrixRow(const std::initializer_list<double>& list)
      : std::vector<double>(list) {}
  MatrixRow(size_t m) : std::vector<double>(m) {}
  double operator[](size_t i) const throw(std::out_of_range) { return at(i); }
  double& operator[](size_t i) throw(std::out_of_range) { return at(i); }
};

struct MatrixColumn final {
  MatrixColumn(std::vector<double*> data) : data(std::move(data)) {}
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

 private:
  std::vector<double*> data;
};

struct Matrix final : std::vector<MatrixRow> {
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

  // Sizes
  size_t size_of_column() { return size(); }
  size_t size_of_row() { return (size() == 0 ? 0 : front().size()); }
  size_t size_n() { return size(); }
  size_t size_m() { return (size() == 0 ? 0 : front().size()); }

  // Getters
  MatrixRow& GetRow(size_t i) throw(std::out_of_range) { return at(i); }
  MatrixColumn GetColumn(size_t j) throw(std::out_of_range) {
    std::vector<double*> columns;
    for (size_t i = 0; i < size_n(); i++) {
      columns.push_back(&(*this)[i][j]);
    }
    return MatrixColumn(columns);
  }
};

}  // namespace koed_shared

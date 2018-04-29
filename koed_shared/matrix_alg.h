#pragma once

namespace koed_shared {

template <class C, class T>
double accumulate(C c, typename C::iterator::value_type val = {}) {
  return std::accumulate(begin(c), end(c), val);
}

template <class C, class T>
double accumulate(C c, T val) {
  return std::accumulate(begin(c), end(c), val);
}

template <class C>
double mean_of_values(C c) {
  return mean_of_values(begin(c), end(c));
}

template <class I>
double mean_of_values(I begin, I end) {
  auto sum = std::accumulate(begin, end, 0.0);
  return sum / std::distance(begin, end);
}

template <class C>
double dispersion_of_values(C c) {
  return dispersion_of_values(begin(c), end(c));
}

template <class I>
double dispersion_of_values(I begin, I end) {
  double mean = mean_of_values(begin, end);
  double sum = 0;
  double distance = std::distance(begin, end);
  while (begin != end) {
    sum += pow(*begin - mean, 2);
    begin++;
  }
  sum /= distance;
  return sum;
}

template <class C>
double std_deviation_of_values(C c) {
  return sqrt(dispersion_of_values(begin(c), end(c)));
}

template <class I>
double std_deviation_of_values(I begin, I end) {
  return sqrt(dispersion_of_values(begin, end));
}

template <class C>
typename C::iterator::value_type max_of_values(C c) {
  return max_of_values(begin(c), end(c));
}

template <class I>
typename I::value_type max_of_values(I begin, I end) {
  if (begin == end) return {};
  auto mx = *begin;
  while (begin != end) {
    mx = max(mx, *begin);
    begin++;
  }
  return mx;
}

template <class C>
typename C::iterator::value_type min_of_values(C c) {
  return min_of_values(begin(c), end(c));
}

template <class I>
typename I::value_type min_of_values(I begin, I end) {
  if (begin == end) return {};
  auto mx = *begin;
  while (begin != end) {
    mx = min(mx, *begin);
    begin++;
  }
  return mx;
}

template <class C>
typename C::iterator::value_type square_accumulate(
    C c, typename C::iterator::value_type res = {}) {
  return square_accumulate(begin(c), end(c), res);
}

template <class I>
typename I::value_type square_accumulate(I begin, I end,
                                         typename I::value_type res = {}) {
  while (begin != end) {
    res += std::pow(*begin, 2);
    begin++;
  }
  return res;
}

}  // namespace koed_shared

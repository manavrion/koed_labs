#pragma once

namespace koed_shared {

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

}  // namespace koed_shared

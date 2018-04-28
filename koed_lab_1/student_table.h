#pragma once
#pragma once
#include <exception>

namespace lab {

struct ValueNotFound : public std::exception {};

double GetStudentTableValue(double df, size_t f) throw(ValueNotFound);

}  // namespace lab

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

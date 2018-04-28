#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "matrix.h"

using namespace koed_shared;

namespace lab {

std::vector<std::vector<std::string>> ReadTable(std::ifstream file);

Matrix BuildMyMatrix(std::vector<std::vector<std::string>> table, size_t begin,
                     size_t end);

}  // namespace lab

#include "table_reader.h"
using namespace koed_shared;
using namespace std;

std::vector<std::vector<std::string>> lab::ReadTable(ifstream file) {
  std::vector<std::vector<std::string>> table;

  string line;
  while (!file.eof()) {
    getline(file, line);
    line.push_back('\t');

    vector<string> row;
    string buf;
    for (auto ch : line) {
      if (ch == '\t') {
        row.push_back(move(buf));
        buf.clear();
      } else {
        buf.push_back(ch);
      }
    }
    table.push_back(move(row));
  }

  size_t maxm = 0;
  for (auto& row : table) {
    maxm = max(maxm, row.size());
  }
  for (auto& row : table) {
    row.resize(maxm);
  }

  return move(table);
}

Matrix lab::BuildMyMatrix(std::vector<std::vector<std::string>> table,
                          size_t begin, size_t end) {
  vector<vector<double>> matrix_data;
  for (int i = 2; i < table.size() - 1; i++) {
    vector<double> row;
    for (size_t j = begin; j < end; j++) {
      string number = table[i][j];
      row.push_back(atof(number.c_str()));
    }
    bool correct = true;
    for (auto ob : row) {
      if (ob == 0) {
        correct = false;
        break;
      }
    }
    if (correct) matrix_data.push_back(move(row));
  }
  Matrix matrix = matrix_data;

  return move(matrix);
}

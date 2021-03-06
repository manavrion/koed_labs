#include <fstream>
#include <iostream>
#include "matrix.h"
#include "student_table.h"
#include "table_reader.h"
using namespace koed_shared;
using namespace lab;
using namespace std;

Matrix GetStatHypothesis(Matrix& m, Matrix& correlationMatrix, double alpha) {
  Matrix res(m.size_m(), m.size_m());
  for (size_t i = 0; i < m.size_m(); i++) {
    for (size_t j = 0; j < m.size_m(); j++) {
      if (i == j) {
        res[i][j] = INFINITY;
        continue;
      }
      double n = m.size_n();
      double r = correlationMatrix[i][j];
      double ch = r * sqrt(n - 2);
      double zn = sqrt(1 - r * r);
      double t = ch / zn;
      res[i][j] = t;
    }
  }
  return move(res);
}

int main() {
  ifstream table("table.txt");
  if (table.fail()) {
    cout << "Data table not found!" << endl;
    cin.get();
    return 1;
  }

  ofstream cout("lab1_output.txt");

  Matrix m = BuildMyMatrix(ReadTable(move(table)), 71, 81);

  cout << "Readed matrix " << m.size_n() << "x" << m.size_m() << endl;
  cout << m << endl << endl;

  // lab4
  ofstream lab4out("lab4out.txt");
  lab4out << 50 << endl;
  for (size_t i = 0; i < 50; i++) {
    lab4out << m.GetColumn(0)[i] << " ";
  }
  lab4out << endl;
  lab4out.close();

  cout << "Column means:" << endl;
  for (size_t j = 0; j < m.size_m(); j++) {
    cout << mean_of_values(m.GetColumn(j)) << "\t";
  }
  cout << endl << endl;

  cout << "Column dispersions:" << endl;
  for (size_t j = 0; j < m.size_m(); j++) {
    cout << dispersion_of_values(m.GetColumn(j)) << "\t";
  }
  cout << endl << endl;

  cout << "Column std deviations:" << endl;
  for (size_t j = 0; j < m.size_m(); j++) {
    cout << std_deviation_of_values(m.GetColumn(j)) << "\t";
  }
  cout << endl << endl;

  auto standardizedMatrix = m.BuildStandardizedMatrix();
  cout << "Standardized matrix " << standardizedMatrix.size_n() << "x"
       << standardizedMatrix.size_m() << endl;
  cout << standardizedMatrix << endl << endl;

  auto covarianceMatrix = m.BuildCovarianceMatrix();
  cout << "Covariance matrix " << covarianceMatrix.size_n() << "x"
       << covarianceMatrix.size_m() << endl;
  cout << covarianceMatrix << endl << endl;

  // lab 3 req
  ofstream lab3out("lab3out.txt");

  auto correlationMatrix = m.BuildCorrelationMatrix();
  cout << "Correlation matrix " << correlationMatrix.size_n() << "x"
       << correlationMatrix.size_m() << endl;
  cout << correlationMatrix << endl << endl;

  lab3out << correlationMatrix.size_n() << " " << correlationMatrix.size_m()
          << endl;
  lab3out << correlationMatrix << endl;
  lab3out.close();

  double df = 0.05;
  size_t f = m.size_n() - 2;
  double t_table = GetStudentTableValue(df, f);

  cout << "Student" << endl;
  cout << "df = " << df << endl;
  cout << "f = " << f << endl;
  cout << "t_table = " << t_table << endl << endl;

  auto statHypothesis = GetStatHypothesis(m, correlationMatrix, df);
  cout << "Stat hypothesis matrix (t - matrix) " << statHypothesis.size_n()
       << "x" << statHypothesis.size_m() << endl;
  cout << statHypothesis << endl << endl;

  Matrix boolMatrix(statHypothesis.size_n(), statHypothesis.size_m());
  cout << "Bool matrix " << statHypothesis.size_n() << "x"
       << statHypothesis.size_m() << endl;
  cout << "\t";
  for (size_t i = 0; i < statHypothesis.size_n(); i++) {
    cout << i << "\t\t";
  }
  cout << endl;
  for (size_t i = 0; i < statHypothesis.size_n(); i++) {
    cout << i << "\t";
    for (size_t j = 0; j < statHypothesis.size_m(); j++) {
      if (i == j) {
        cout << "middle\t";
        boolMatrix[i][j] = 1;
        continue;
      }
      if (abs(statHypothesis[i][j]) >= t_table) {
        cout << "true\t";
        boolMatrix[i][j] = 1;
      } else {
        cout << "false\t";
        boolMatrix[i][j] = 0;
      }
    }
    cout << endl;
  }
  cout << endl << endl;

  cout.close();

  // lab 2 req
  ofstream lab2out("lab2out.txt");

  size_t selectedColumn = 0;
  for (int i = 0; i < boolMatrix.size_n(); i++) {
    if (accumulate(boolMatrix[selectedColumn], 0.0) <
        accumulate(boolMatrix[i], 0.0)) {
      selectedColumn = i;
    }
  }
  lab2out << "Y vector, selected column = " << selectedColumn << endl;
  lab2out << m.GetColumn(selectedColumn).size() << endl;

  for (auto ob : m.GetColumn(selectedColumn)) {
    lab2out << ob << " ";
  }
  lab2out << endl;
  size_t sizeN = 0;
  for (size_t j = 0; j < m.size_m(); j++) {
    if (abs(correlationMatrix[j][selectedColumn]) < 0.3 || j == selectedColumn)
      continue;
    sizeN++;
  }
  lab2out << sizeN << " " << m.size_n() << endl;
  for (size_t j = 0; j < m.size_m(); j++) {
    if (abs(correlationMatrix[j][selectedColumn]) < 0.3 || j == selectedColumn)
      continue;
    for (auto ob : m.GetColumn(j)) {
      lab2out << ob << " ";
    }
    lab2out << endl;
  }

  lab2out.close();

  system("lab1_output.txt");
  system("lab2out.txt");
  system("lab3out.txt");
  system("lab4out.txt");
  return 0;
}

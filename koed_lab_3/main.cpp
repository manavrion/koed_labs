#include <fstream>
#include <iostream>
#include <utility>
#include "matrix.h"
using namespace koed_shared;
using namespace std;

struct JacobiMethodCannotSolveException {};

pair<Matrix, Matrix> JacobiMethod(Matrix a, double eps) {
  if (a.size_n() != a.size_m()) throw "error";

  Matrix t = Matrix::BuildIdentity(a.size());

  double alpha = 0.0;
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a.size(); j++) {
      if (i == j) continue;
      alpha += a[i][j] * a[i][j];
    }
  }
  alpha = sqrt(alpha) / (double)a.size();

  double alpha0 = alpha;
  int n = 0;
  while (true) {
    n++;
    if (n == 1'000'000) {
      throw JacobiMethodCannotSolveException{};
    }

    Matrix a_k = a;
    size_t i_mx = -1;
    size_t j_mx = -1;
    for (size_t i = 0; i < a.size(); i++) {
      for (size_t j = 0; j < a.size(); j++) {
        if (i == j) continue;
        if (abs(a[i][j]) > alpha) {
          if (i_mx == -1 || abs(a[i_mx][j_mx]) < abs(a[i][j])) {
            i_mx = i;
            j_mx = j;
          }
        }
      }
    }

    if (i_mx != -1 && j_mx != -1) {
      double d = sqrt(pow(a[i_mx][i_mx] - a[j_mx][j_mx], 2) +
                      4 * pow(a[i_mx][j_mx], 2));

      double sgn_p = a[i_mx][j_mx] * (a[i_mx][i_mx] - a[j_mx][j_mx]);
      double sgn = (sgn_p < 0 ? -1 : 1);

      double s =
          sgn * sqrt(0.5 * (1.0 - abs(a[i_mx][i_mx] - a[j_mx][j_mx]) / d));
      double c = sqrt(0.5 * (1.0 + abs(a[i_mx][i_mx] - a[j_mx][j_mx]) / d));

      for (size_t k = 0; k < a.size(); k++) {
        if (k != i_mx && k != j_mx) {
          a_k[k][i_mx] = a_k[i_mx][k] = a[k][i_mx] * c + a[k][j_mx] * s;
          a_k[k][j_mx] = a_k[j_mx][k] = -a[k][i_mx] * s + a[k][j_mx] * c;
        }
      }

      auto s2 = s * s;
      auto c2 = c * c;
      auto sc = s * c;
      auto aii = a[i_mx][i_mx];
      auto aij = a[i_mx][j_mx];
      auto ajj = a[j_mx][j_mx];

      a_k[i_mx][i_mx] = aii * c2 + 2 * aij * sc + ajj * s2;
      a_k[j_mx][j_mx] = aii * s2 - 2 * aij * sc + ajj * c2;
      a_k[j_mx][i_mx] = a_k[i_mx][j_mx] = 0;

      Matrix t_k = t;
      for (size_t k = 0; k < t.size(); k++) {
        t_k[k][i_mx] = t[k][i_mx] * c + t[k][j_mx] * s;
        t_k[k][j_mx] = -t[k][i_mx] * s + t[k][j_mx] * c;
      }
      t = t_k;
    }
    a = a_k;
    alpha /= double(a.size() * a.size());

    bool cont = false;
    for (size_t i = 0; i < a.size(); i++) {
      for (size_t j = 0; j < a.size(); j++) {
        if (i == j) continue;
        if (abs(a[i][j]) > alpha0 * eps) {
          cont = true;
          break;
        }
      }
    }
    if (!cont) break;
  }
  cout << "Total iterations " << n << endl;
  return {move(a), move(t)};
}

int main() {
  ifstream file("../koed_lab_1/lab3out.txt");

  // ifstream file("lab3_test1.txt");
  // http://www.wolframalpha.com/input/?i=eigenvalues%7B%7B1,0.42,0.54,0.66%7D,%7B0.42,1,0.32,0.44%7D,%7B0.54,0.32,1,0.22%7D,%7B0.66,0.44,0.22,1%7D%7D

  // ifstream file("lab3_test2.txt");
  // http://www.wolframalpha.com/input/?i=eigenvalues%7B%7B2,1,1%7D,%7B1,2.5,1%7D,%7B1,1,3%7D%7D
  if (file.fail()) {
    cout << "Lab1 result not found!" << endl;
    cin.get();
    return 1;
  }

  ofstream cout("lab3_output.txt");
  double eps = 1e-5;

  int sn, sm;
  file >> sn >> sm;
  Matrix a_in(sn, sm);
  file >> a_in;

  cout << "eps = " << eps << endl << endl;
  cout.precision(3);
  cout << "Readed matrix " << a_in.size_n() << "x" << a_in.size_m() << endl;
  cout << a_in << endl << endl;

  cout << "Run Jacobi Method ..." << endl << endl;
  Matrix a(0), t(0);
  try {
    auto [ag, tg] = JacobiMethod(a_in, eps);
    a = ag;
    t = tg;
  } catch (const JacobiMethodCannotSolveException&) {
    cout << "Cannot solve " << endl;
    cout.close();
    system("lab3_output.txt");
    return 0;
  }

  // cout << "t " << endl;
  // cout << t << endl;
  // cout << "a " << endl;
  // cout << a << endl;

  std::vector<pair<double, MatrixColumn>> lambdas;
  for (size_t i = 0; i < a.size(); i++) {
    lambdas.push_back({a[i][i], t.GetColumn(i)});
  }

  sort(lambdas.rbegin(), lambdas.rend(),
       [](const auto& a, const auto& b) { return a.first < b.first; });

  std::vector<double> l;
  for (size_t i = 0; i < lambdas.size(); i++) {
    l.push_back(lambdas[i].first);
  }

  cout << "Calc lambdas" << endl;
  size_t num = 1;
  for (auto l : lambdas) {
    cout << "lambda" << num << " = ";
    cout << l.first << endl;
    num++;
  }
  cout << endl;

  cout << "Corresponding eigenvectors" << endl;
  num = 1;
  for (auto& l : lambdas) {
    for (auto& ob : l.second) {
      ob /= *l.second.rbegin();
    }
    cout.precision(4);
    cout << fixed;
    cout << "v" << num << " =\t";
    for (auto ob : l.second) {
      cout << ob << "\t";
    }
    cout << endl;
    num++;
  }
  cout << endl;

  cout << "Run I(p') searching ..." << endl;
  double Ip = 0;
  size_t d_p = 1;
  for (; d_p <= lambdas.size(); d_p++) {
    Ip = accumulate(l.begin(), l.begin() + d_p, 0.0) / accumulate(l, 0.0);
    if (Ip > 0.95) break;
  }
  cout << "p' = " << d_p << endl;
  cout << "I(p') = " << Ip << endl;

  cout.close();
  system("lab3_output.txt");
}

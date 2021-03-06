#include <fstream>
#include <iostream>
#include <string>
#include "matrix.h"
using namespace koed_shared;
using namespace std;

pair<Matrix, Matrix> BuildLabMatrixes(ifstream cin) {
  string line;
  getline(cin, line);
  size_t n;
  cin >> n;

  Matrix y(n, 1);
  for (size_t i = 0; i < n; i++) {
    cin >> y[i][0];
  }

  size_t m;
  cin >> m >> n;
  Matrix x(n, m);
  for (size_t j = 0; j < m; j++) {
    for (size_t i = 0; i < n; i++) {
      cin >> x[i][j];
    }
  }

  return {move(x), move(y)};
}

Matrix Mnk(const Matrix& x, const Matrix& y) {
  auto xt = x.BuildTransposeMatrix();
  return mpow(xt * x, -1) * xt * y;
}

int main() {
  ifstream file("../koed_lab_1/lab2out.txt");
  // ifstream file("lab2_test.txt");
  if (file.fail()) {
    cout << "Lab1 result not found!" << endl;
    cin.get();
    return 1;
  }

  ofstream cout("lab2_output.txt");

  auto [x, y] = BuildLabMatrixes(move(file));
  auto xt = x.BuildTransposeMatrix();

  cout << "Readed matrix X " << x.size_n() << "x" << x.size_m() << endl;
  cout << x << endl << endl;

  cout << "Readed vector Y " << y.size_n() << "x" << y.size_m() << endl;
  cout << y << endl << endl;

  auto a = Mnk(x, y);
  cout << "Vector A " << a.size_n() << "x" << a.size_m() << endl;
  cout << a << endl << endl;

  auto epsilon = y - x * a;
  cout << "Vector Epsilon " << epsilon.size_n() << "x" << epsilon.size_m()
       << endl;
  cout << epsilon << endl << endl;

  auto epsilon_mean = mean_of_values(epsilon.AsVector());
  cout << "Mean Epsilon = " << epsilon_mean << endl;

  double alpha_m = epsilon_mean;

  double eps = 1e-2;
  if (abs(epsilon_mean) > eps) {
    cout << "Check 3 failed, run 3'" << endl;

    // Update epsilon
    auto e_m = epsilon;
    for (auto& ob : e_m.AsVector()) ob -= alpha_m;

    // Update x
    auto x_m = x;
    x_m.push_back_column(1);

    if ((abs(mean_of_values(e_m.AsVector())) < eps)) {
      epsilon = e_m;
      x = x_m;
      xt = x.BuildTransposeMatrix();
      a = Mnk(x, y);
      cout << "Update vectors A, E and X done!" << endl << endl;
    } else {
      cout << "fail" << endl;
      cin.get();
      return 1;
    }

  } else {
    cout << "Check 3 success" << endl;
  }

  ///

  cout << "Run checks" << endl;

  auto yR = x * a;

  auto e = (y - yR);
  cout << "Estimated deviations vector E " << e.size_n() << "x" << e.size_m()
       << endl;
  cout << e << endl << endl;

  cout << "Check_1 = ";
  cout << y.Equals(x * a + e, eps) << endl;

  cout << "Check_2 = ";
  cout << ((-xt) * y + xt * x * a).IsZero(eps) << endl;

  cout << "Check_3 = ";
  cout << (xt * (y - x * a)).IsZero(eps) << endl;

  cout << "Check_4 = ";
  cout << (xt * (y - yR)).IsZero(eps) << endl;

  cout << "Check_5 = ";
  cout << (xt * e).IsZero(eps) << endl;

  cout << "Check_6 = ";
  cout << (y - (yR + e)).IsZero(eps) << endl;

  cout << "Check_7 = ";
  cout << (abs(mean_of_values(e.GetColumn(0))) < eps) << endl;

  cout << "Check_8 = ";
  cout << (abs(mean_of_values(y.AsVector()) - mean_of_values(yR.AsVector())) <
           eps)
       << endl;

  cout << endl;

  /// determ coef

  double ch = square_accumulate(e.AsVector());
  double zn = 0;
  double y_mean = mean_of_values(y.AsVector());
  for (auto yk : y.AsVector()) zn += pow(yk - y_mean, 2);

  double r = 1.0 - (ch / zn);

  cout << "R = " << r << endl << endl;

  cout.close();
  system("lab2_output.txt");
}

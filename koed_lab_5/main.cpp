#include <fstream>
#include <iostream>
#include <utility>
#include "matrix.h"
using namespace koed_shared;
using namespace std;

vector<double> BuildInputVector(int variant) {
  ifstream file("lab5_table.txt");
  if (file.fail()) {
    cout << "Lab data not found!" << endl;
    cin.get();
    exit(1);
  }
  size_t n, m;
  file >> n >> m;
  Matrix matrix(n, m);
  file >> matrix;
  auto col = matrix.GetColumn(variant - 1);
  return vector<double>(col.begin(), col.end());
}

vector<double> ModVeid(vector<double> x, double alpha) {
  if (alpha <= 0 || alpha >= 1) throw "alpha err";

  double beta = 1 - alpha;

  double spre = (1.0 / 5.0) * accumulate(x.begin(), x.begin() + 5, 0.0);
  vector<double> s(x.size());
  for (size_t t = 0; t < x.size(); t++) {
    s[t] = alpha * x[t] + beta * spre;
    spre = s[t];
  }
  return move(s);
}

int main() {
  vector<double> x;

#if 0
  x = BuildInputVector(1);
#else
  x.resize(100);
  cout.precision(3);
  for (size_t t = 0; t < x.size(); t++) {
    const double a = 1;
    const double b = 1;
    const double w = 0.18;
    x[t] = a * cos(w * t) + b * sin(w * t);
  }
#endif

  cout << "Readed vector x " << x.size() << endl;
  for (auto ob : x) cout << ob << " ";
  cout << endl << endl;

  cout << "Mod Veid" << endl;
  cout << "x, alp:\t";
  for (size_t i = 1; i < 10; i++) {
    double alpha = 1 - i / 10.0;
    cout << alpha << "\t";
  }
  cout << endl;

  Matrix ans(x.size(), 10);
  auto x_it = x.begin();
  for (auto& ob : ans.GetColumn(0)) ob = *(x_it++);

  for (size_t i = 1; i < 10; i++) {
    double alpha = 1 - i / 10.0;
    auto v = ModVeid(x, alpha);
    auto v_it = v.begin();
    for (auto& ob : ans.GetColumn(i)) ob = *(v_it++);
  }

  cout << ans << endl << endl;

  // find minimum
  // tuple<rate, alpha, ans>
  vector<tuple<double, double, vector<double>>> answers;

  for (size_t j = 1; j < ans.size_m(); j++) {
    auto col = ans.GetColumn(j);
    vector<double> s(col.begin(), col.end());

    double r = 0.0;
    for (size_t t = 0; t < x.size(); t++) {
      r += pow(x[t] - s[t], 2);
    }

    answers.push_back({r, 1 - j / 10.0, move(s)});
  }

  sort(answers.begin(), answers.end(),
       [](const auto& a, const auto& b) { return get<0>(a) < get<0>(b); });

  auto foundAns = answers.front();

  cout << "Found ans with alpha = " << get<1>(foundAns) << endl;
  cout << "rate = " << get<0>(foundAns) << endl;

  ofstream pait_task("paint_task.txt");

  pait_task << x.size() << endl;
  double begin = -10;
  double end = 10;
  double pos_x = begin;
  for (size_t i = 0; i < x.size(); i++) {
    pos_x += abs(begin - end) / x.size();
    pait_task << pos_x << " ";
  }
  pait_task << endl;

  int color = 255;
  pait_task << (1 + ans.size_m()) << endl;

  pait_task << x.size() << " " << color << endl;
  for (auto ob : x) pait_task << ob << " ";
  pait_task << endl;

  for (size_t i = 0; i < ans.size_m(); i++) {
    color -= 255 / ans.size_m();
    pait_task << ans.GetColumn(i).size() << " " << color << endl;
    for (auto ob : ans.GetColumn(i)) pait_task << ob << " ";
    pait_task << endl;
  }
  pait_task.close();
  system("lab5_painter.exe");

  cin.get();
}

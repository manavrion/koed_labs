#include <math.h>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include "matrix.h"
using namespace koed_shared;
using namespace std;

int main() {
  ifstream file("lab4_test.txt");
  if (file.fail()) {
    cout << "Input file not found!" << endl;
    cin.get();
    return 1;
  }

  double eps = 1e-4;

  int n;
  file >> n;
  vector<double> x(n);
  for (auto& ob : x) file >> ob;

  cout << "Readed vector " << x.size() << endl;
  for (auto& ob : x) cout << ob << " ";
  cout << endl << endl;

  cout << "Build ranges ..." << endl;
  sort(x.begin(), x.end());

  double dx = (x.back() - x.front()) / (1.0 + 3.31 * log10(x.size()));
  //
  dx = int(dx);

  double dmax = x.back();
  double dmin = x.front();
  double k = (dmax - dmin) / dx;
  cout << "dx = " << dx << endl;
  cout << "x'max = " << dmax << endl;
  cout << "x'min = " << dmin << endl;
  cout << "k = " << k << endl;

  if (abs(k - (int)(k + (1 - eps))) > eps) {
    cout << endl;
    cout << "Bad k, search shift ..." << endl;

    double prefer_k = int(k + (1 - eps));

    auto f = [&dx, &x, &prefer_k](double m) {
      double dmax = x.back();
      double dmin = x.front();
      double k = ((dmax - dmin) + m) / dx;
      return abs(k - prefer_k);
    };

    auto tern_search_min = [eps](auto f, double l, double r) {
      while (abs(r - l) > eps) {
        auto h = abs(r - l) / 3.0;
        if (f(l + h) > f(r - h)) {
          l += h;
        } else {
          r -= h;
        }
      }
      return (l + r) / 2.0;
    };

    double m = tern_search_min(f, 0, dx);

    cout << "Shift selected = " << m << endl;
    cout << "Recalc ..." << endl;

    double dmin_pos = int(dmin / dx) * dx;
    double dmin_dx = min(abs(dmin_pos - dmin), m);
    m -= dmin_dx;
    dmin = x.front() - dmin_dx;
    dmax = x.back() + m;
    k = int(((dmax - dmin) + m) / dx + eps);
    cout << "dx = " << dx << endl;
    cout << "x'max = " << dmax << endl;
    cout << "x'min = " << dmin << endl;
    cout << "k = " << k << endl;
  }

  cin.get();
}

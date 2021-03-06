#include <math.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include "data_set.h"
#include "matrix.h"
using namespace koed_shared;
using namespace lab;
using namespace std;

int main() {
  //ifstream file("../koed_lab_1/lab4out.txt");
  ifstream file("lab4_test.txt");
  if (file.fail()) {
    cout << "Lab1 result not found!" << endl;
    cin.get();
    return 1;
  }

  ofstream cout("lab4_output.txt");

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
    cout << endl;

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
    cout << endl;
  }

  cout << "Build DataSet ..." << endl;
  DataSet ds(x, dmin, dx, k);

  auto infoVec = ds.GetInfoVec();

  double max_f_l = 0;
  double max_f_c = 0;
  size_t i = 1;
  for (auto info : infoVec) {
    cout << "Pile " << i++ << " :" << endl;
    cout << "Bounds(c1, c2) = [" << info.c1 << ", " << info.c2 << ")" << endl;
    cout << "Middle(l) = " << info.l << endl;
    cout << "Frequency(ni) = " << info.n << endl;
    cout << "Relative frequency(mi) = " << info.m << endl;
    cout << "Sum frequency(si) = " << info.sum_n << endl;
    cout << "Integr Func(F(ci)) = " << info.f_c << endl;
    cout << "Diff Func(F(li)) = " << info.f_l << endl;
    cout << endl;

    max_f_l = max(max_f_l, info.f_l);
    max_f_c = max(max_f_c, info.f_c);
  }

  cout << "Sample Mean = " << mean_of_values(x) << endl;
  cout << "Sample Dispersion = " << dispersion_of_values(x) << endl;
  cout << "Sample Std deviation = " << std_deviation_of_values(x) << endl;
  cout << "Variance reach = " << *x.rbegin() - *x.begin() << endl;
  cout << "Variance coef = " << std_deviation_of_values(x) / mean_of_values(x)
       << endl;
  cout << endl;

  int st_v[] = {5, 10, 15, 20, 25, 30, 36, 40, 50, 70, 100, 300};
  double st_apl_0_5[] = {2.02, 1.81, 1.75, 1.73, 1.71, 1.70,
                         1.69, 1.68, 1.67, 1.66, 1.65, 1.64};

  cout << "M(x) search ... " << endl;
  double alpha = 0.05;
  cout << "alpha = " << alpha << endl;
  double pd = 1 - alpha;
  cout << "pd = " << pd << endl;
  int v = x.size() - 1;
  cout << "v = " << v << endl;
  double std_d = std_deviation_of_values(x);
  cout << "std_d = " << std_d << endl;

  if (alpha != 0.05) {
    throw "Err, 0.05 only";
  }

  auto tlb = lower_bound(begin(st_v), end(st_v), int(v));
  double t_s = st_apl_0_5[distance(begin(st_v), tlb)];
  cout << "t_s = " << t_s << endl;

  double delta;
  if (x.size() <= 30) {
    delta = t_s * std_d / sqrt(x.size());
  } else {
    delta = t_s * std_d / sqrt(x.size() - 1);
  }
  cout << "delta = " << delta << endl;
  double mean = mean_of_values(x);
  double mu = delta / mean;
  cout << "M(x) <- [" << mean - delta << ", " << mean + delta
       << "] with precision " << pd * 100 << " %" << endl;
  cout << "mu = " << mu << endl << endl;

  cout << "Diff Func(F(li)) histogram" << endl;
  for (auto info : infoVec) {
    cout << "[" << info.c1 << ", " << info.c2 << ")\t";
    size_t perm_count = 50;
    double count_per = info.f_l / max_f_l;
    for (size_t i = 0; i < size_t(perm_count * count_per); i++) {
      cout << "=";
    }
    cout << endl;
  }
  cout << endl;
  
  cout << "Integr Func(F(ci)) histogram" << endl;
  for (auto info : infoVec) {
    cout << "[" << info.c1 << ", " << info.c2 << ")\t";
    size_t perm_count = 50;
    double count_per = info.f_c / max_f_c;
    for (size_t i = 0; i < size_t(perm_count * count_per); i++) {
      cout << "=";
    }
    cout << endl;
  }


  cout.close();
  system("lab4_output.txt");
}

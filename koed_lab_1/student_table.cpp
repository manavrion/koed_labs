#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "student_table.h"

using namespace std;

double lab::GetStudentTableValue(double df, size_t f) throw(ValueNotFound) {
  ifstream in("student_table.txt");
  string line;
  stringstream ss;
  getline(in, line);
  ss << line;
  ss >> line;  // df throw

  vector<double> keys;
  while (!ss.eof()) {
    double val;
    ss >> val;
    keys.push_back(val);
  }

  map<double, vector<double>> mp;
  for (auto key : keys) {
    mp[key].push_back(0);
  }

  while (!in.eof()) {
    getline(in, line);
    stringstream ss;
    ss << line;
    ss >> line;  // num throw
    for (auto key : keys) {
      double val;
      ss >> val;
      mp[key].push_back(val);
    }
  }
  in.close();
  if (mp.count(df) == 0 || mp[df].size() <= f) {
    throw ValueNotFound{};
  }
  return mp[df].at(f);
}

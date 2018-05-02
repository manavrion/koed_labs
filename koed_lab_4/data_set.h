#pragma once
#include <algorithm>
#include <vector>

namespace lab {

struct DataSet {
  struct PileInfo {
    double c1;
    double c2;
    double l;
    size_t n;
    double m;
    size_t sum_n;
    double f_c;
    double f_l;
  };

  struct Pile {
    Pile(double begin, double end)
        : begin(begin), end(end), middle((begin + end) / 2.0), samples() {}
    double begin;
    double end;
    double middle;
    size_t frequency() { return samples.size(); }
    void push(double s) { samples.push_back(s); }

   private:
    std::vector<double> samples;
  };
  struct PileStat {
    double relativeFrequency;
    size_t sumFrequency;
    double integrFunc;
    double diffFunc;
  };

  DataSet(std::vector<double> data, double startPos, double pileSize,
          size_t pileCount);

  std::vector<PileInfo> GetInfoVec();

 private:
  std::vector<Pile> piles;
  std::vector<PileStat> stats;
};

}  // namespace lab

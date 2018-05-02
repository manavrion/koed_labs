#include "data_set.h"

using namespace lab;
using namespace std;

DataSet::DataSet(std::vector<double> data, double startPos, double pileSize,
                 size_t pileCount) {
  sort(data.begin(), data.end());
  auto it = data.begin();
  for (size_t i = 0; i < pileCount; i++) {
    Pile pile(startPos + i * pileSize, startPos + (i + 1) * pileSize);

    while (it != data.end() && *it < pile.end) {
      pile.push(*it);
      it++;
    }

    piles.push_back(pile);
  }

  //
  stats.resize(piles.size());
  size_t total_f = 0.0;
  double total_rel = 0.0;
  for (size_t i = 0; i < stats.size(); i++) {
    total_f += piles[i].frequency();
    stats[i].sumFrequency = total_f;

    stats[i].relativeFrequency = (double)piles[i].frequency() / data.size();
    total_rel += stats[i].relativeFrequency;
    stats[i].integrFunc = total_rel;

    stats[i].diffFunc =
        stats[i].relativeFrequency / (piles[i].end - piles[i].begin);
  }
}

vector<DataSet::PileInfo> DataSet::GetInfoVec() {
  vector<PileInfo> res;
  for (size_t i = 0; i < piles.size(); i++) {
    res.push_back(PileInfo{piles[i].begin, piles[i].end, piles[i].middle,
                           piles[i].frequency(), stats[i].relativeFrequency,
                           stats[i].sumFrequency, stats[i].integrFunc,
                           stats[i].diffFunc});
  }
  return move(res);
}

#ifndef RAND_DIST_H
#define RAND_DIST_H

#include <bits/stdc++.h>
#include <chrono>
#include <random>

using namespace std;

namespace rand_dist {
  class NormalGenerator{
  private:
    unsigned int seed;
    default_random_engine *generator;
    normal_distribution<double> *distribution;
    double min_val, max_val;

  public:
    NormalGenerator(double m, double sd, double mi, double ma);
    double generate();
    double generateBounded();
  };
}

#endif

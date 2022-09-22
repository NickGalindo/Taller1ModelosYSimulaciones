#include "rand_dist.h"

using namespace std;

namespace rand_dist {
  NormalGenerator::NormalGenerator(double m, double sd, double mi, double ma){
      /*
       * Make the normal generator with max and min values mean+sd and mean-sd
       */

      seed = chrono::system_clock::now().time_since_epoch().count();
      generator = new default_random_engine(seed);
      distribution = new normal_distribution<double>(m, sd);
      min_val = mi;
      max_val = ma;
  }

  double NormalGenerator::generate(){
    return (*distribution)(*generator);
  }

  double NormalGenerator::generateBounded(){
    while(true){
      double num = this->generate();
      if(num >= min_val && num <= max_val)
        return num;
    }
  }

}

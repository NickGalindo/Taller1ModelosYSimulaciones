#include <bits/stdc++.h>
#include "simlib.h"
#include "lcgrand.h"

#define ff first
#define ss second

#define BUS_CNT_LIMIT 10
#define BUSY 1
#define IDLE 0

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;


int main(){
  lcgrandst(102930019211, 4);
  for(int i = 0; i < 10; i++){
    cout << lcgrandgt(4) << endl;
  }
  cout << "ddd" << endl;

  return 0;
}

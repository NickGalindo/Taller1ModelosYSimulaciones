#ifndef SIM_H
#define SIM_H

#include <bits/stdc++.h>
#define ff first
#define ss second

using namespace std;

class Sim{
  public:
    priority_queue<pair<int, string> > scheduler;
    queue<double> ppl;
    queue<double> bus;
    double time;
    int max_ppl_queue_length;
    stringstream log;

  Sim(int max_ppl_q);

  void schedule(double t, string type);
  pair<int, string> nextEvent();
  bool addPpl(double time_arrival);
  double removePpl();
  bool hasPpl();
  void addBus(double time_arrival);
  double removeBus();
  void printScheduler();
  void printPpl();
};

#endif


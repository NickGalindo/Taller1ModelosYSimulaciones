#include "sim.h"

using namespace std;

Sim::Sim(int max_ppl_q){
  time = 0;
  max_ppl_queue_length = max_ppl_q;
  return;
}

void Sim::schedule(double t, string type){
  scheduler.push({-t, type});
  return;
}

pair<int, string> Sim::nextEvent(){
  auto aux = scheduler.top();
  aux.ff = -aux.ff;
  scheduler.pop();
  time = aux.ff;

  log << aux.ff << " " << aux.ss << endl; 

  return aux;
}

bool Sim::addPpl(double time_arrival){
  if(ppl.size() >= 10)
    return false;

  ppl.push(time_arrival);
  return true;
} 

double Sim::removePpl(){
  auto aux = ppl.front();
  ppl.pop();
  return aux;
}

bool Sim::hasPpl(){
  return this->ppl.size() > 0;
}

void Sim::addBus(double time_arrival){
  bus.push(time_arrival);
  return ;
}

double Sim::removeBus(){
  auto aux = bus.front();
  bus.pop();
  return aux;
}

// HELPER SHIT
void Sim::printScheduler(){
  priority_queue<pair<double, string>> aux;

  while(!this->scheduler.empty()){
    auto aux1 = this->scheduler.top();
    aux.push(aux1);
    cout << "Scheduler: " << aux1.ff << " " << aux1.ss << endl;
    this->scheduler.pop();
  }

  while(!aux.empty()){
    auto aux1 = aux.top();
    this->scheduler.push(aux1);
    aux.pop();
  }
}

void Sim::printPpl(){
  queue<double> aux;

  while(!ppl.empty()){
    auto aux1 = ppl.front();
    aux.push(aux1);
    cout << "Ppl: " << aux1 << endl;
    ppl.pop();
  }

  while(!aux.empty()){
    auto aux1 = aux.front();
    aux.pop();
    ppl.push(aux1);
  }
}

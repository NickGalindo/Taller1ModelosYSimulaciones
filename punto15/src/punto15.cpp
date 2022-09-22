#include <bits/stdc++.h>
#include "sim.h"
#include "rand_dist.h"

#define ff first
#define ss second

using namespace std;
typedef long long ll;

// SIMULATION VARIABLES
Sim *sim;
rand_dist::NormalGenerator *ppl_arrival, *bus_arrival;

// SYSTEM VARIABLES
double ppl_arrival_mean, ppl_arrival_sd, ppl_boarding_time, bus_arrival_mean, bus_arrival_sd, bus_await_time;
int max_ppl_q, max_bus_sim;

// AUXILIARY VARIABLES
int bus_sim = 0;
int ppl_boarded_total=0, ppl_left_total=0;
double ppl_boarded_wait_time_sum=0.0, bus_wait_time_sum=0.0;

// HELPER VaRIABLE
string outfile;

// NECESARY RUNTIME FUNCTIONS
void init_model();
double ppl_arrive();
double ppl_boarded();
double bus_arrive();
double bus_depart();
void update_time_avg_stats(pair<double, string> event, double aux);
void report(ofstream& fout);

int main(){
  // Get all of the enviornment variables
  ifstream fin("in.txt");

  fin >> ppl_arrival_mean >> ppl_arrival_sd >> ppl_boarding_time >> bus_arrival_mean >> bus_arrival_sd >> bus_await_time >> max_ppl_q >> max_bus_sim >> outfile;

  ofstream fout(outfile);

  fout << "CONSTANTES DE EJECUCION\n";
  fout << "\t TASA LLEGADA PERSONAS:    " << ppl_arrival_mean << "\n";
  fout << "\t SD LLEGADA PERSONAS:      " << ppl_arrival_sd << "\n";
  fout << "\t TIEMPO ABORDAJE PERSONAS: " << ppl_boarding_time << "\n";
  fout << "\t TASA LLEGADA BUSES:       " << bus_arrival_mean << "\n";
  fout << "\t SD LLEGADA BUSES:         " << bus_arrival_sd << "\n";
  fout << "\t TIEMPO ESPERA BUS:        " << bus_await_time << "\n";
  fout << "\t MAX QUEUE DE PERSONAS:    " << max_ppl_q << "\n";
  fout << "\t MAX SIMULACION BUSSES:    " << max_bus_sim << "\n";

  init_model();

  while(bus_sim < max_bus_sim){
    //cout << "WHAT THE FUCK" << endl;
    //sim->printScheduler();

    auto aux = sim->nextEvent();

    //cout << "\t" << aux.ss << endl;
    //sim->printScheduler();
    //sim->printPpl();

    //cin.get();
    
    double cnt=0;

    if(aux.ss == "PPL_ARRIVAL")
      cnt = ppl_arrive();
    else if(aux.ss == "BUS_ARRIVAL")
      cnt = bus_arrive();
    else if(aux.ss == "PPL_BOARDED")
      cnt = ppl_boarded();
    else if(aux.ss == "BUS_DEPART")
      cnt = bus_depart();

    update_time_avg_stats(aux, cnt);
  }

  report(fout);

  return 0;
}


void init_model(){
  sim = new Sim(max_ppl_q);
  ppl_arrival = new rand_dist::NormalGenerator(ppl_arrival_mean, sqrt(ppl_arrival_sd), ppl_arrival_mean - ppl_arrival_sd, ppl_arrival_mean + ppl_arrival_sd);
  bus_arrival = new rand_dist::NormalGenerator(bus_arrival_mean, sqrt(bus_arrival_sd), bus_arrival_mean - bus_arrival_sd, bus_arrival_mean + bus_arrival_sd);

  sim->schedule(sim->time + ppl_arrival->generateBounded(), "PPL_ARRIVAL");
  sim->schedule(sim->time + bus_arrival->generateBounded(), "BUS_ARRIVAL");

  return ;
}

double ppl_arrive(){
  auto rand_time_arrival = sim->time + ppl_arrival->generateBounded();
  sim->schedule(rand_time_arrival, "PPL_ARRIVAL");
  if(!sim->addPpl(rand_time_arrival))
    return 1.0;

  return 0.0;
}

double ppl_boarded(){
  double aux = sim->removePpl();

  if(sim->hasPpl())
    sim->schedule(sim->time + ppl_boarding_time, "PPL_BOARDED");
  else
    sim->schedule(sim->time, "BUS_DEPART");

  return aux;
}

double bus_arrive(){
  sim->schedule(sim->time + bus_arrival->generateBounded(), "BUS_ARRIVAL");
  sim->schedule(sim->time + bus_await_time, "BUS_DEPART");

  sim->addBus(sim->time);

  if(sim->hasPpl())
    sim->schedule(sim->time + ppl_boarding_time, "PPL_BOARDED");

  return 0.0;
}

double bus_depart(){
  double aux = sim->removeBus();
  return aux;
}

void update_time_avg_stats(pair<double, string> event, double aux){
  if(event.ss == "PPL_BOARDED"){
    ppl_boarded_total++;
    ppl_boarded_wait_time_sum += sim->time - aux;
    return ;
  }
  if(event.ss == "PPL_ARRIVAL"){
    ppl_left_total += int(aux);
    return ;
  }
  if(event.ss == "BUS_ARRIVAL"){
    return;
  }
  if(event.ss == "BUS_DEPART"){
    bus_wait_time_sum += aux;
    bus_sim++;
  }
  return ;
}

void report(ofstream& fout){
  ofstream logout("log.txt");
  logout << sim->log.rdbuf();

  fout << "\n\nRESULTADOS DE EJECUCION\n";
  fout << "\tTIEMPO PROMEDIO ESPERA EN LA COLA: " << ppl_boarded_wait_time_sum/double(ppl_boarded_total) << endl;
  fout << "\tTAMANO DE COLA PROMEDIO: " << double(ppl_boarded_total) / double(max_bus_sim) << endl;
  fout << "\tPROMEDIO DE PERSONAS SIN ABORDAR POR COLA " << double(ppl_left_total) / double(max_bus_sim) << endl;
  fout << "\tTOTAL DE PERSONAS SIN ABORDAD: " << ppl_left_total << endl;
  fout << "\tTOTAL DE PERSONAS ABORDADAS: " << ppl_boarded_total << endl;
  fout << "\tTIEMPO PROMEDIO DE ABORDAJE POR BUS: " << bus_wait_time_sum / double(max_bus_sim) << endl;
}

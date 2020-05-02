#define NDEBUG
// #define NPRINT

#include <iostream>
using namespace std;

const double LOWER_BOUND = 19400000;

#include "Constants.cpp"
#include "WFCRP.cpp"
#include "Solution.cpp"
#include "SimulatedAnnealing.cpp"


int main() {
  // ios::sync_with_stdio(false);
  // cin.tie(nullptr);
  cout.tie(nullptr);
  cerr << fixed << setprecision(9);

  // read("in");
  // read("inr");
  read("../instances/wf01_cb01_capex.crp");

  Solution solution = Solution::bestInitial();
  cout << solution;

  Solution sa = simulatedAnnealing(1000000, 1, 0.999, solution);
  cout << sa;

  /*
  if (!freopen("inopt", "r", stdin)) {
    return 1;
  }

  int w[103];
  for (int i = 0; i < n; ++i) {
    int u, v;
    cin >> u >> v >> w[i];
    solution.next[u] = v;
  }
  solution.evalFitness();
  cout << solution;
  debug(solution.fitness, c, k);
  for (int i = 0; i < n; ++i) {
    debug(i, w[i], solution.energy[i], bestCable(solution.energy[i]));
    assert(w[i] == bestCable(solution.energy[i]));
  }
  //*/
  return 0;
}

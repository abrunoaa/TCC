#define NDEBUG
// #define NPRINT

#include <iostream>
using namespace std;

const double LOWER_BOUND = 0;

#include "Constants.cpp"
#include "WFCRP.cpp"
#include "Solution.cpp"
#include "SimulatedAnnealing.cpp"


int main(int argc, char **argv) {
  // ios::sync_with_stdio(false);
  // cin.tie(nullptr);
  cout.tie(nullptr);
  cerr << fixed << setprecision(9);

  string dirname = "../instances/";
  string filename = argv[1] + string(".crp");

  // read("in");
  // read("inr");
  read((dirname + filename).data());

  if (!freopen(("../results/sa/" + filename).data(), "w", stdout)) {
    cerr << "Can't open output\n";
    return 1;
  }

  auto start = clock();
  Solution solution = Solution::bestInitial();
  cout << solution << "Elapsed: " << (double)(clock() - start) / CLOCKS_PER_SEC << "s\n\n";


  Solution sa = simulatedAnnealing(10000000, 1, 0.9999, solution);
  cout << sa << "Elapsed: " << (double)(clock() - start) / CLOCKS_PER_SEC << "s\n\n";

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

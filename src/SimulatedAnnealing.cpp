#ifndef SIMULATED_ANNEALING_CPP
#define SIMULATED_ANNEALING_CPP 1

#include "Constants.cpp"
#include "Solution.cpp"


Solution simulatedAnnealing(double startT,
                            double minT,
                            double alpha,
                            Solution x) {
  assert(cmp(0, alpha) <= 0 && cmp(alpha, 1) < 0);
  assert(cmp(0, minT) <= 0 && cmp(minT, startT) <= 0);

  uniform_real_distribution<double> urd;

  Solution best = x;
  for (double temp = startT; temp >= minT; temp *= alpha) {
    Solution y = x.neighbor();
    y.localSearch();
    if (cmp(y.fitness, x.fitness) < 0 || urd(rng) < exp((x.fitness - y.fitness) / temp)) {
      x = move(y);
      if (cmp(x.fitness, best.fitness) < 0) {
        debug(best.fitness);
        best = x;
      }
    }
  }

  return best;
}

#endif // SIMULATED_ANNEALING_CPP

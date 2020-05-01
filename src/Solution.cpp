#ifndef SOLUTION_CPP
#define SOLUTION_CPP 1

#include <algorithm>
#include <functional>
#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
using namespace std;

#include "Constants.cpp"
#include "WFCRP.cpp"


struct Solution {
  static const int MAX_ATTEMPTS = 100;

  int reachSubstation;
  double fitness;
  vector<int> next;
  vector<int> energy;

  int chooseNeighbor(int u) const {
    assertValid();
    if (next[u] == n && reachSubstation == 1) {
      return -1;
    }

    auto deep = next;
    deep[u] = u;
    function<int(int)> find;
    find = [&find, &deep](int v) {
      return deep[v] == v || deep[v] == n ? v : deep[v] = find(deep[v]);
    };

    vector<int> candidate;
    for (int v = 0; v < n; ++v) {
      if (v != u && v != next[u] && find(v) != u && energy[u] + energy[find(v)] <= k.back()) {
        bool flag = 1;
        for (int w = 0; w < n; ++w) {
          if (intersect(turbines[u], turbines[v], turbines[w], turbines[next[w]])) {
            debug(turbines[u], turbines[v], turbines[w], turbines[next[w]]);
            flag = 0;
            break;
          }
        }
        if (flag) {
          candidate.push_back(v);
        }
      }
    }

    if (candidate.empty()) {
      return -1;
    }

    debug(u, candidate);
    for (int x : candidate) {
      debug(u, x, find(x));
    }
    uniform_int_distribution<int> uid(0, (int)candidate.size() - 1);
    return candidate[uid(rng)];
  }

  Solution neighbor() const {
    assertValid();
    uniform_int_distribution<int> uid(0, (int)next.size() - 1);
    for (int i = 0; i < MAX_ATTEMPTS; ++i) {
      int u = uid(rng);
      int v = chooseNeighbor(u);
      debug("connect", u, v);
      if (v != -1) {
        Solution that;
        that.next = next;
        that.reachSubstation = reachSubstation + (v == n) - (next[u] == n);
        that.next[u] = v;
        that.evalFitness();
        return that;
      }
    }

    int withoutConnection = 0;
    for (int u = 0; u < n; ++u) {
      int v = chooseNeighbor(u);
      withoutConnection += v == -1;
    }
    cerr << " ** no change! **\n";
    if (withoutConnection == n) {
      cerr << " ***** Trapped!!! *****\n";
    }
    return *this;
  }

  void localSearch() {
  }

  static Solution bestInitial() {
    Solution best;
    best.fitness = 1e100;
    for (int i = 0; i < n; ++i) {
      Solution other = Solution::build(i);
      if (other.fitness < best.fitness) {
        best = move(other);
      }
    }
    debug(best.fitness, best.next, best.energy);
    best.assertValid();
    return best;
  }

  static Solution build(int start) {
    debug(start);

    Solution solution;
    solution.fitness = 1e20;
    solution.reachSubstation = 0;

    auto &g = solution.next;
    g.resize(n);
    for (auto zone : getZones(start)) {
      debug(zone);
      int u = zone.back();
      for (int i = (int)zone.size() - 2; i >= 0; --i) {
        int v = zone[i];
        g[u] = v;
        u = v;
      }
      g[u] = n;
      ++solution.reachSubstation;
    }

    solution.evalFitness();
    solution.assertValid();
    return solution;
  }

  void evalEnergy() {
    assertValid(false);

    energy = production;

    vector<int> used(n + 1, 0);
    for (int u = 0; u < n; ++u) {
      ++used[next[u]];
    }
    debug(next);
    debug(used);
    assert(1 <= used[n] && used[n] <= C);

    queue<int> q;
    for (int u = 0; u < n; ++u) {
      if (used[u] == 0) {
        q.push(u);
      }
    }
    debug(q);
    assert(q.size() > 0);

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      int v = next[u];
      debug(u, v);
      if (v != n) {
        energy[v] += energy[u];
        --used[v];
        if (used[v] == 0) {
          q.push(v);
        }
      }
    }

    debug(used);
    for (int u = 0; u < n; ++u) {
      debug(u, used[u]);
      assert(used[u] == 0);
    }
  }

  void evalFitness() {
    evalEnergy();
    fitness = 0;
    for (int u = 0; u < n; ++u) {
      int v = next[u];
      fitness += dist(u, v) * bestCableCost(energy[u]);
    }
    assertValid();
  }

  void assertValid(bool hasEnergy = true) const {
#ifndef NDEBUG
    debug(fitness, next, energy);

    // valid state
    assert((int)next.size() == n);

    // valid connection
    auto &g = next;
    for (int u = 0; u < n; ++u) {
      assert(g[u] != u);
      assert(0 <= g[u] && g[u] <= n);
    }

    // without loop
    vector<int> vis(n + 1, 0);
    vis[n] = 1;
    for (int u = 0; u < n; ++u) {
      if (vis[u] == 0) {
        int v;
        for (v = u; vis[v] == 0; v = next[v]) {
          vis[v] = 2;
        }
        debug(u, v);
        assert(vis[v] == 1);
        for (v = u; vis[v] == 2; v = next[v]) {
          vis[v] = 1;
        }
      }
    }

    // must go to substation
    assert((int)g.size() == n);
    int go_sub = 0;
    for (int u = 0; u < n; ++u) {
      go_sub += g[u] == n;
    }
    debug(go_sub, reachSubstation);
    assert(go_sub > 0);
    assert(go_sub == reachSubstation);

    if (!hasEnergy) {
      return;
    }

    // must have energy
    assert((int)energy.size() - 1 == n);
    for (int u = 0; u < n; ++u) {
      assert(cmp(energy[u], production[u]) >= 0);
    }

    // cable that support the energy must exist
    for (int u = 0; u < n; ++u) {
      int cable = bestCable(energy[u]);
      assert(k[cable] >= energy[u]);
      assert(cmp(c[cable], bestCableCost(energy[u])) == 0);
    }

    // valid cost
    double expectedFitness = 0;
    for (int u = 0; u < n; ++u) {
      expectedFitness += dist(u, next[u]) * bestCableCost(energy[u]);
    }
    debug(expectedFitness, fitness);
    assert(cmp(expectedFitness, fitness) == 0);

    // optimal solution
    assert(cmp(fitness, 8604208.925966221839190) >= 0);
#endif // NDEBUG
  }
};

ostream& operator<<(ostream& out, const Solution& s) {
  debug(s.next);
  assert((int)s.next.size() == n);
  debug(s.energy);
  assert((int)s.energy.size() - 1 == n);

  out << fixed << setprecision(15);
  out << "Solução: " << s.fitness << '\n';
  out << "Conexões:\n";
  out << "> Subestação = " << n << '\n';
  out << "> Valores: origem destino cabo\n";

  for (int u = 0; u < n; ++u) {
    int v = s.next[u];
    int cable = bestCable(s.energy[u]);
    out << u << ' ' << v << ' ' << cable << '\n';
  }
  return out;
}

#endif // SOLUTION_CPP

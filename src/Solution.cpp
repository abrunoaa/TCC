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


struct Solution;

ostream& operator<<(ostream& out, const Solution& s);


struct Solution {
  struct ExchangeCandidate {
    lf change;
    int u, v;

    bool operator<(const ExchangeCandidate& that) const {
      return change < that.change;
    }
  };

  int reachSubCount;
  lf fitness;
  vector<int> next;
  vector<int> energy;

  /**
   * Check if changing the parent of node u to v creates an intersection.
   * @param u Node to change parent
   * @param v Candidate parent
   * @return true If isn't possible to change parent of u to v.
   */
  bool createIntersection(int u, int v) const {
    for (int w = 0; w < n; ++w)
      if (w != u && intersect(u, v, w, next[w]))
        return 1;
    return 0;
  }

  /**
   * Find the depth of u on the tree
   * @param u Node to verify.
   * @return Depth of node u.
   */
  int depth(int u) const {
    int d = 0;
    for (; u != n; u = next[u]) ++d;
    return d;
  }

  /**
   * Check what will happen to cost if the parent of u is changed to v.
   * @param u Node to change parent
   * @param v Candidate node to be parent of u
   * @return The change that will happen if parent of u is changed to v
   */
  lf changeInCost(int u, int v) const {
    assert(0 <= u && u < n);
    assert(0 <= v && v <= n);
    assert(u != v);

    int e = energy[u];
    // debug(u, v, e);
    assert(cmp(e, 0) > 0);
    lf ans = (dist(u, next[u]) - dist(u, v)) * bestCableCost(e);
    // debug(ans, dist(u, v), dist(u, next[u]), bestCableCost(e));

    auto upCost = [this](int x, int add) {
      // debug(x, next[x], dist(x, next[x]), energy[x], add, energy[x] + add, bestCableCost(energy[x] + add), bestCableCost(energy[x]));
      return dist(x, next[x]) * (bestCableCost(energy[x]) - bestCableCost(energy[x] + add));
    };

    u = next[u];
    int du = depth(u), dv = depth(v);
    // debug(u, v, du, dv);
    for (; du < dv; v = next[v], --dv) ans += upCost(v, +e);
    for (; du > dv; u = next[u], --du) ans += upCost(u, -e);
    for (; v != u; v = next[v], u = next[u]) ans += upCost(v, +e) + upCost(u, -e);

    return ans;
  }

  void updateEnergy(int u, int e) {
    for (; u != n; u = next[u]) {
      energy[u] += e;
      assert(cmp(energy[u], 0) > 0);
    }
  }

  Solution neighbor() const {
    assertValid();

    vector<ExchangeCandidate> candidates;
    for (int u = 0; u < n; ++u) {
      // ignore if it's the only node to reach substation
      if (next[u] == n && reachSubCount == 1) {
        continue;
      }

      // farthest parent of node which reach substation
      // u is the flag for invalid
      auto p = next;
      function<int(int)> reachSub;
      reachSub = [&](int w) {
        return w == u || p[w] == n ? w : p[w] = reachSub(p[w]);
      };

      // check for candidates
      for (int v = 0; v < n; ++v)
        if (u != v && v != next[u] && reachSub(v) != u && energy[u] + energy[reachSub(v)] <= k.back() && !createIntersection(u, v))
          candidates.push_back({changeInCost(u, v), u, v});
      if (next[u] != n && reachSubCount < C && !createIntersection(u, n))
        candidates.push_back({changeInCost(u, n), u, n});
    }

    if (candidates.empty()) {
      cout << "Current solution:\n";
      cout << *this;
      cerr << next << '\n';
      throw domain_error("Solution trapped!");
    }

    sort(candidates.begin(), candidates.end());
    lf sum = 0;
    for (auto candidate : candidates) {
      sum += candidate.change - candidates[0].change + 1;
    }

    uniform_real_distribution<lf> urd(0, sum);
    lf r = urd(rng);
    for (auto [change, u, v] : candidates) {
      r -= change - candidates[0].change + 1;
      if (r < 0) {
        debug(u, v, change, fitness, fitness - change);
        Solution that = *this;
        // that.assertValid();
        that.updateEnergy(next[u], -energy[u]);
        that.updateEnergy(v, energy[u]);
        that.reachSubCount += (v == n) - (that.next[u] == n);
        that.next[u] = v;
        that.fitness -= change;
        assert(cmp(changeInCost(u, v), change) == 0);
        // Solution &other = that;
        // debug(this->next);
        // debug(other.next);
        // debug(this->energy);
        // debug(other.energy);
        that.assertValid();
        return that;
      }
    }
    assert(0);
    exit(1);
  }

  void localSearch() {
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
    for (int u = 0; u < n; ++u)
      if (used[u] == 0)
        q.push(u);
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
        if (used[v] == 0)
          q.push(v);
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
      fitness += dist(u, next[u]) * bestCableCost(energy[u]);
    }
    assertValid();
  }

  static Solution bestInitial() {
    Solution best;
    best.fitness = 1e100;
    for (int i = 0; i < n; ++i)
      if (Solution other = Solution::build(i); other.fitness < best.fitness)
        best = move(other);
    debug(best.fitness, best.next, best.energy);
    best.assertValid();
    return best;
  }

  static Solution build(int start) {
    debug(start);

    Solution solution;
    solution.fitness = 1e20;
    solution.reachSubCount = 0;

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
      ++solution.reachSubCount;
    }

    solution.evalFitness();
    solution.assertValid();
    return solution;
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

    // without cycle
    vector<int> vis(n + 1, 0);
    vis[n] = 1;
    for (int u = 0; u < n; ++u) {
      if (vis[u] == 0) {
        int v;
        for (v = u; vis[v] == 0; v = next[v]) {
          vis[v] = 2;
        }
        // debug(u, v);
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
    debug(go_sub, reachSubCount);
    assert(go_sub > 0);
    assert(go_sub == reachSubCount);

    if (!hasEnergy) {
      return;
    }

    // everybody must have energy...
    assert((int)energy.size() - 1 == n);
    for (int u = 0; u < n; ++u) {
      assert(cmp(energy[u], production[u]) >= 0);
    }

    // except substation
    assert(energy.back() == 0);

    // cable that support the energy must exist
    for (int u = 0; u < n; ++u) {
      int cable = bestCable(energy[u]);
      assert(k[cable] >= energy[u]);
      assert(cmp(c[cable], bestCableCost(energy[u])) == 0);
    }

    // valid cost
    lf expectedFitness = 0;
    for (int u = 0; u < n; ++u) {
      // auto d = dist(u, next[u]);
      // auto bcc = bestCableCost(energy[u]);
      // debug(u, next[u], energy[u], d, bcc, d * bcc);
      expectedFitness += dist(u, next[u]) * bestCableCost(energy[u]);
    }
    debug(expectedFitness, fitness);
    assert(cmp(expectedFitness, fitness) == 0);

    // optimal solution
    assert(cmp(fitness, LOWER_BOUND) >= 0);
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

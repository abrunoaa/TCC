#ifndef WFCRP_CPP
#define WFCRP_CPP 1

#include <cassert>
#include <cmath>

#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

#include "Constants.cpp"
#include "Point.cpp"


int n, t, C;                // number of turbines, cables and max connections to substation
vector<int> c;              // cable cost
vector<int> k;              // cable capacity
Point substation;           // substation location
vector<Point> turbines;     // turbines location (substation is apendded)
vector<int> production;     // production of each turbine (0 for substation)
vector<int> order;          // turbines sorted by angle around substation

vector<vector<lf>> graph;   // pre-calculed distance

lf dist(int u, int v) {
  assert(0 <= u && u <= n);
  assert(0 <= v && v <= n);
  return graph[u][v];
}

int bestCable(int energy) {
  assert(energy > 0);
  int cable = 0;
  for (; k[cable] < energy; ++cable) assert(cable + 1 < (int)k.size());
  return cable;
}

int bestCableCost(int energy) {
  return c[bestCable(energy)];
}

bool intersect(int u, int v, int w, int x) {
  return intersect(turbines[u], turbines[v], turbines[w], turbines[x]);
}

void buildGraph() {
  graph.assign(n + 1, vector<lf>(n + 1, 0.0));
  for (int i = 0; i <= n; ++i) {
    for (int j = i + 1; j <= n; ++j) {
      graph[i][j] = graph[j][i] = dist(turbines[i], turbines[j]);
      assert(cmp(graph[i][j], dist(turbines[j], turbines[i])) == 0);
    }
  }
}

void sortTurbines() {
  order.resize(n);
  for (int i = 0; i < n; ++i) {
    order[i] = i;
  }

  sort(order.begin(), order.end(), [](int i, int j) {
    auto x1 = turbines[i].x - substation.x;
    auto y1 = turbines[i].y - substation.y;
    auto x2 = turbines[j].x - substation.x;
    auto y2 = turbines[j].y - substation.y;
    return cmp(atan2(y1, x1), atan2(y2, x2)) < 0;
  });
}

// should call sortTurbines at least once before calling this function
vector<vector<int>> getZones(int start) {
  assert(0 <= start && start < n);
  assert(t >= 2);
  assert(n > 1);

  const auto biggestCapacity = k[t - 2];

  vector<int> zone;
  vector<vector<int>> zones;
  int used = 0;

  for (int i = start - n; true; ++i) {
    int pos = order[i < 0 ? i + n : i];
    if (i == start || used + production[pos] > biggestCapacity) {
      zones.push_back(move(zone));
      used = 0;
    }
    if (i == start) {
      return zones;
    }
    used += production[pos];
    zone.push_back(pos);
  }
}

void read(const string& filename) {
  ifstream in(filename);

  in >> n >> t >> C;
  assert(n > 0);
  assert(t > 0);
  assert(C > 0);
  in >> substation;

  turbines.resize(n + 1);
  production.resize(n + 1);
  for (int i = 0; i < n; ++i) {
    in >> turbines[i];
    // in >> production[i];
    production[i] = 1;
    turbines[i].x -= substation.x;
    turbines[i].y -= substation.y;
  }
  turbines[n] = {0, 0};
  production[n] = 0;

  c.resize(t);
  k.resize(t);
  for (int i = 0; i < t; ++i) {
    in >> k[i] >> c[i];
    assert(i == 0 || c[i] > c[i - 1]);
    assert(i == 0 || k[i] > k[i - 1]);
  }
  for (auto p : production) {
    assert(0 <= p && p <= k.back());
  }

  // c.push_back(inf);
  // k.push_back(inf);
  // ++t;

  sortTurbines();
  buildGraph();
}

#endif // WFCRP_CPP

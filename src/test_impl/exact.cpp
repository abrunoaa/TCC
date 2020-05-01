#include <iostream>
using namespace std;

#define st first
#define nd second

typedef pair<int,int> ii;

const int inf = 0x3f3f3f3f;
const int maxn = 15;
const int maxm = 5;
const int maxt = 10;
const int maxs = maxn + maxm;

int n, m, t;                  // number of turbines, substations and cables
int prod[maxn];               // production of i-th turbine
int c[maxt], cap[maxt];       // cost and capacity of cable
int graph[maxs][maxs];        // graph[u][v] representing distance between u and v
ii g[maxs][1 << maxn];        // g function
int dp[maxm][1 << maxn];

ii& operator+=(ii& x, const ii& y) {
  x.st += y.st;
  x.nd += y.nd;
  return x;
}

int chooseCable(int energy) {
  int i;
  for (i = t; i > 0 && energy <= cap[i - 1]; --i);
  return i;
}

void build_g() {
  for (int u = 0; u < n + m; ++u) {
    g[u][0] = ii(0, 0);
  }
  for (int u = 0; u < n + m; ++u) {
    for (int k = 1; k < (1 << n); ++k) {
      ii &cur = g[u][k] = ii(inf, 0);
      for (int h = k; h != 0; h = (h - 1) & k) {
        for (int v = 0; v < n; ++v) {
          if ((h & (1 << v))) {
            ii r = g[v][h ^ (1 << v)];
            int cable = chooseCable(r.nd + prod[v]);
            if (cable != t) {
              r.st += graph[u][v] * c[cable];
              r.nd += prod[v];
              r += g[u][k - h];
              if (r.st < cur.st) {
                cur = r;
              }
            }
          }
        }
      }
    }
  }
}

int f() {
  for (int k = 0; k < (1 << n); ++k) {
    dp[0][k] = g[k][n].st;
  }
  for (int s = 1; s < m; ++s) {
    dp[s][0] = 0;
    for (int k = 1; k < (1 << n); ++k) {
      dp[s][k] = dp[s - 1][k];
      for (int h = k; h; h = (h - 1) & k) {
        dp[s][k] = min(dp[s][k], g[h][s + n].st + dp[s - 1][k - h]);
      }
    }
  }
  return dp[m - 1][(1 << n) - 1];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  return 0;
}

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef double lf;

const lf inf = 1e20;
const lf eps = 1e-6;

const int maxn = 100;
const int maxt = 10;

// graph
int n;
lf x[maxn + 1], y[maxn + 1];
int p[maxn + 1], ps[maxn + 1]; // ps[i] = sum{p[j] | 0 <= j <= i}
lf G[maxn + 1][maxn + 1];

// cables
int T;
int c[maxt], k[maxt];

// solution
lf memo[maxn + 1][maxn][maxn];
lf memo2[maxn + 1][maxn][maxn];

/*******************************************************************/

lf bestCost(int energy) {
  for (int i = 0; i < T; ++i) {
    if (k[i] >= energy) {
      // cerr << " $$ " << energy << ' ' << k[i] << " == " << c[i] << endl;
      return c[i];
    }
  }
  return inf;
}

lf cost(int u, int v, int energy) {
  // cerr << " -- " << u << ' ' << v << ' ' << energy << " == " << G[u][v] << ' ' << bestCost(energy) << endl;
  return G[u][v] * bestCost(energy);
}

lf f(int v, int i, int j) {
  if (i > j) return 0;

  // cerr << " >> " << v << ' ' << i << ' ' << j << endl;
  auto &dp = memo[v][i][j];
  if (dp >= 0) return dp;
  if (i == j) return dp = cost(v, i, p[i]);

  dp = inf;
  int energy = ps[j] - (i == 0 ? 0 : ps[i - 1]);
  for (int u = i; u <= j; ++u) {
    dp = min(dp, cost(v, u, energy) + f(u, i, u - 1) + f(u, u + 1, j));
  }
  for (int u = i; u < j; ++u) {
    dp = min(dp, f(v, i, u) + f(v, u + 1, j));
  }
  return dp;
}

void build() {
  for (int v = 0; v <= n; ++v) {
    for (int i = 0; i < n; ++i) {
      memo2[v][i][i] = cost(v, i, p[i]);
    }
  }
  for (int i = n - 1; i >= 0; --i) {
    for (int j = i + 1; j < n; ++j) {
      for (int v = 0; v <= n; ++v) {
        auto &dp = memo2[v][i][j] = inf;
        int energy = ps[j] - (i == 0 ? 0 : ps[i - 1]);
        for (int u = i; u <= j; ++u) {
          dp = min(dp, cost(v, u, energy)
                        + (i > u - 1 ? 0 : memo2[u][i][u - 1])
                        + (u + 1 > j ? 0 : memo2[u][u + 1][j]));
        }
        for (int u = i; u < j; ++u) {
          dp = min(dp, memo2[v][i][u] + memo2[v][u + 1][j]);
        }
      }
    }
  }
}

void rec(int v, int i, int j, string format) {
  if (i > j) return;

  // cerr << " >> " << v << ' ' << i << ' ' << j << endl;
  if (i == j) {
    cout << format << "< " << i << ' ' << p[i] << " §§ " << v << '\n';
    return;
  }

  auto &dp = memo[v][i][j];
  int energy = ps[j] - (i == 0 ? 0 : ps[i - 1]);
  // cerr << " ## " << v << ' ' << i << ' ' << j << " == " << dp << ' ' << energy << endl;
  for (int u = i; u <= j; ++u) {
    lf tmp = cost(v, u, energy) + f(u, i, u - 1) + f(u, u + 1, j);
    // cerr << " == " << tmp << endl;
    if (abs(dp - tmp) < eps) {
      cout << format << "> " << u << ' ' << energy << " §§ " << v << '\n';
      rec(u, i, u - 1, format + "   ");
      rec(u, u + 1, j, format + "   ");
      return;
    }
  }
  for (int u = i; u < j; ++u) {
    lf tmp = f(v, i, u) + f(v, u + 1, j);
    // cerr << " ++ " << tmp << " ++ " << dp-tmp << endl;
    if (abs(dp - tmp) < eps) {
      rec(v, i, u, format);
      rec(v, u + 1, j, format);
      return;
    }
  }
  assert(0);
}

void print(int v, int i, int j) {
  cout << "# " << v << ' ' << ps[j] - (i == 0 ? 0 : ps[i - 1]) << '\n';
  rec(v, i, j, "   ");
}

int main() {
  assert(freopen("in", "r", stdin));
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);

  cin >> n;
  cin >> x[n] >> y[n] >> p[n];
  assert(p[n] == 0);
  for (int i = 0; i < n; ++i) {
    cin >> x[i] >> y[i] >> p[i];
    ps[i] = p[i];
    if (i) ps[i] += ps[i - 1];
  }
  for (int i = 0; i <= n; ++i) {
    for (int j = i; j <= n; ++j) {
      G[i][j] = G[j][i] = hypot(x[i] - x[j], y[i] - y[j]);
    }
  }

  cin >> T;
  for (int i = 0; i < T; ++i) {
    cin >> k[i] >> c[i];
    assert(i == 0 || k[i] > k[i - 1]);
    assert(i == 0 || c[i] > c[i - 1]);
  }

  memset(memo, -1, sizeof(memo));
  cout << fixed << setprecision(3);
  // cout << f(n, 0, n - 1) << endl;
  // print(n, 0, n - 1);
  build();
  for (int v = 0; v <= n; ++v) {
    for (int l = 0; l < n; ++l) {
      for (int r = 0; r < n; ++r) {
        // cout << setw(7) << f(v, l, r) << " \n"[r + 1 == n];
        cerr << " >> " << v << ' ' << l << ' ' << r << " == " << f(v, l, r) << ' '<< memo2[v][l][r] << endl;
        assert(abs(f(v, l, r) - memo2[v][l][r]) < eps);
      }
    }
    // cout << "------------------------\n";
  }

  return 0;
}

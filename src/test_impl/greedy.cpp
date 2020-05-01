#include <bits/stdc++.h>
using namespace std;

typedef double lf;

const int maxn = 1003;
const int inf = 0x3f3f3f3f;

int n;
lf x[maxn], y[maxn];
int p[maxn];
bool selected[maxn];
lf g[maxn][maxn];
lf min_e[maxn];
lf w[maxn];
int to[maxn];

void add(int v) {
  selected[v] = true;
  for (int u = 0; u < n; ++u) {
    if (!selected[u] && g[v][u] < w[u]) {
      w[u] = g[v][u];
      to[u] = v;
    }
  }
}

lf prim() {
  for (int v = 0; v <= n; ++v) {
    selected[v] = false;
    w[v] = 1e100;
  }
  add(n);

  lf ans = 0;
  for (int i = 0; i < n; ++i) {
    int v = -1;
    for (int u = 0; u < n; ++u) {
      if (!selected[u] && (v == -1 || w[u] < w[v])) {
        v = u;
      }
    }

    ans += g[v][to[v]];
    cout << v << " -> " << to[v] << '\n';
    add(v);
  }

  return ans;
}

int main() {
  assert(freopen("in", "r", stdin));
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);

  cin >> n;
  cin >> x[n] >> y[n] >> p[n];
  assert(p[n] == 0);
  for (int i = 0; i < n; ++i) {
    cin >> x[i] >> y[i] >> p[i];
  }
  for (int i = 0; i <= n; ++i) {
    for (int j = i; j <= n; ++j) {
      g[i][j] = g[j][i] = hypot(x[i] - x[j], y[i] - y[j]);
    }
  }

  cout << prim() << '\n';
  return 0;
}

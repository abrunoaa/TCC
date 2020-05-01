#include <bits/stdc++.h>
using namespace std;

typedef double lf;

struct Point {
  lf x, y;
  int p;

  bool operator<(const Point& point) const {
    return atan2(y, x) <= atan2(point.y, point.x) - eps;
  }

  Point& operator+=(const Point& point) {
    x += point.x;
    y += point.y;
    return *this;
  }

  Point& operator-=(const Point& point) {
    x -= point.x;
    y -= point.y;
    return *this;
  }
};

istream& operator>>(istream& in, Point& p) {
  return in >> p.x >> p.y >> p.p;
}

const lf eps = 1e-6;
const int maxn = 1003;
const int maxt = 10;

int n, T;
int c[maxt], k[maxt];
Point sub;
Point p[maxn];
lf g[maxn + 1][maxn + 1];

int main() {
  assert(freopen("in", "r", stdin));
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);

  cin >> n;
  cin >> sub;
  assert(sub.p == 0);
  for (int i = 0; i < n; ++i) {
    cin >> p[i];
  }

  for (int i = 0; i < n; ++i) {
    p[i] -= sub;
  }
  sort(p, p + n);
  for (int i = 0; i < n; ++i) {
    p[i] += sub;
  }

  for (int i = 0; i < n; ++i) {
    for (int j = i; j < n; ++j) {
      g[i][j] = g[j][i] = hypot(p[i].x - p[j].x, p[i].y - p[j].y);
    }
  }
  for (int i = 0; i < n; ++i) {
    g[i][n] = g[n][i] = hypot(p[i].x - sub.x, p[i].y - sub.y);
  }
  g[n][n] = 0;

  cin >> T;
  for (int i = 0; i < T; ++i) {
    cin >> k[i] >> c[i];
    assert(i == 0 || k[i] > k[i - 1]);
    assert(i == 0 || c[i] > c[i - 1]);
  }

  const int i = 0;
  vector<int> zone;
  int used = 0;
  for (int j = i - n; true; ++j) {
    int pos = (j + n) % n;
    if (i == j || used + p[pos].p > k[T - 1]) {
      cout << "Use " << used << " for:\n";
      for (int v : zone) {
        cout << '(' << p[v].x << ", " << p[v].y << ")\n";
      }
      zone.clear();
      used = 0;
    }
    used += p[pos].p;
    zone.push_back(pos);
    if (i == j) {
      break;
    }
  }

  return 0;
}

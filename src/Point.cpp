#ifndef POINT_CPP
#define POINT_CPP 1

#include <iostream>
using namespace std;

#include "Constants.cpp"


struct pt {
  int x, y;
  bool operator==(const pt& p) const { return x == p.x && y == p.y; }
  bool operator!=(const pt& p) const { return !(*this == p); }
};

typedef pt Point;

pt operator*(int s, const pt& a)        { return pt{a.x * s, a.y * s}; }
pt operator*(const pt& a, int s)        { return pt{a.x * s, a.y * s}; }
pt operator+(const pt& a, const pt& b)  { return pt{a.x + b.x, a.y + b.y}; }
pt operator-(const pt& a, const pt& b)  { return pt{a.x - b.x, a.y - b.y}; }
ll operator~(const pt& a)               { return (ll)a.x * a.x + (ll)a.y * a.y; }
ll operator*(const pt& a, const pt& b)  { return (ll)a.x * b.x + (ll)a.y * b.y; }
ll operator^(const pt& a, const pt& b)  { return (ll)a.x * b.y - (ll)a.y * b.x; }
lf dist(const pt& a, const pt& b)       { return hypot(a.x - b.x, a.y - b.y); }

lf angle(const pt& a, const pt& b) { return acos((lf)(a * b) / sqrt(~a * ~b)); }
bool ccw(const pt& a, const pt& b) { return (a ^ b) > 0; }

bool intersect(pt p, pt r, pt q, pt s) {
  pt pq = q - p;
  pt pr = r - p;
  pt qs = s - q;
  ll c1 = pr ^ qs;
  ll c2 = pq ^ pr;
  ll c3 = pq ^ qs;
  if (!c1) {
    if (c2) return 0;
    lf t0 = (lf)(pq * pr) / (lf)(pr * pr);
    lf t1 = t0 + (lf)(qs * pr) / (lf)(pr * pr);
    if (cmp(t0, t1) > 0) swap(t0, t1);
    return cmp(0, t0) < 0 && cmp(t1, 1) < 0;
  }
  lf t = (lf)c3 / (lf)c1;
  lf u = (lf)c2 / (lf)c1;
  // debug(t, u, c1, c2, c3);
  return cmp(0, t) < 0 && cmp(t, 1) < 0 && cmp(0, u) < 0 && cmp(u, 1) < 0;
}

istream& operator>>(istream& in, pt& p) {
  return in >> p.x >> p.y;
}

ostream& operator<<(ostream& out, const pt& p) {
  return out << '(' << p.x << ", " << p.y << ')';
}

#endif // POINT_CPP

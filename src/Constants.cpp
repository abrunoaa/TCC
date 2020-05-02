#ifndef CONSTANT_CPP
#define CONSTANT_CPP 1


#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <iostream>
#include <random>
using namespace std;


typedef long long ll;
typedef long double lf;

const lf eps = 1e-9;
const int inf = 0x3f3f3f3f;

inline int cmp(lf x, lf y) {
  return (abs(x - y) < eps ? 0 : x < y ? -1 : +1);
}

random_device rd;
// mt19937 rng(rd());
mt19937 rng(0);

template<class A, class B>ostream &operator<<(ostream &f, const pair<A, B> &x) {return f << '(' << x.first << ", " << x.second << ')';}

#define DEF1(_class...) ostream&operator<<(ostream&f,const _class&x){int b=0;f<<'[';for(auto&y:x){if(b++)f<<", ";f<<y;}return f<<']';}
#define DEF2(get,_class...) ostream&operator<<(ostream&f,_class x){int b=0;f<<'[';while(x.size()){if(b++)f<<", ";f<<x.get();x.pop();}return f<<']';}
template<class T> DEF1(vector<T>) template<class T, class C> DEF1(set<T, C>) template<class T, class C> DEF1(multiset<T, C>) template<class K, class T, class C> DEF1(map<K, T, C>) template<class K, class T, class C> DEF1(multimap<K, T, C>)
template<class T> DEF2(front, queue<T>) template<class T> DEF2(top, stack<T>) template<class T, class V, class C> DEF2(top, priority_queue<T, V, C>)

__attribute__((destructor))static void __destroy__() {cerr << "\nElapsed: " << (double)clock() / CLOCKS_PER_SEC << "s.\n";}

#if !defined NDEBUG && !defined NPRINT
void debug() {}
template<class t, class... u> void debug(const t &x, const u &...y) {cerr << ' ' << x, debug(y...);}

#define debug(x...) cerr<<"\x1b[91m"<<__LINE__<<" ["#x"] =",debug(x),cerr<<"\x1b[0m\n"
#define sleep(x) this_thread::sleep_for(chrono::milliseconds(x))
#else
#define debug(x...) void(0)
#define sleep(x) void(0)
#endif // NDEBUG && NPRINT

#endif // CONSTANT_CPP

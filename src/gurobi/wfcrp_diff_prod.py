#!/usr/bin/env python3.7

import gurobipy as gp
import matplotlib.pyplot as plt
import sys

from gurobipy import GRB
from itertools import permutations
from math import hypot


INSTANCES = '../../instances/'
RESULTS = '../../results/'

colors = [
  'tab:cyan',
  'tab:orange',
  'tab:blue',
  'tab:purple',
  'tab:green',
  'tab:red',
  'tab:gray',
  'tab:brown',
  'tab:olive',
  'tab:pink',
]

# instance
#  sys.stdin = open('in', 'r')
filename = sys.argv[1]
sys.stdin = open(INSTANCES + filename + '.crp', 'r')
sys.stdout = open(RESULTS + filename + '.out', 'w')

n, t, C = map(int, input().split())
substation = tuple(map(int, input().split()))
turbines = [tuple(map(int, input().split())) for _ in range(n)] + [substation]
capacity, cost = (list(x) for x in zip(*[tuple(map(int, input().split())) for _ in range(t)]))
production = [1] * n + [0]

def distance(u, v):
  dx = turbines[u][0] - turbines[v][0]
  dy = turbines[u][1] - turbines[v][1]
  return hypot(dx, dy)

def orientation(p, q, r):
  ans = (q[1] - p[1]) * (r[0] - p[0]) - (q[0] - p[0]) * (r[1] - p[1])
  return 0 if ans == 0 else -1 if ans < 0 else 1

def intersect(a, b, c, d):
  if orientation(turbines[a], turbines[b], turbines[c]) * orientation(turbines[a], turbines[b], turbines[d]) >= 0:
    return False
  if orientation(turbines[c], turbines[d], turbines[a]) * orientation(turbines[c], turbines[d], turbines[b]) >= 0:
    return False
  return True

def callback(model, where):
  if where == GRB.Callback.MIPSOL:
    flow = model.cbGetSolution(model._flow)
    segm = []
    for i, j in model._flow.keys():
      if flow[i, j] > 0.001:
        for h, k in segm:
          if intersect(i, j, h, k):
            model.cbLazy(model._graph.sum(i, j, '*') + model._graph.sum(h, k, '*') <= 1)
            return
        segm.append((i, j))


# objective variable
index_with_cable, m_cost = gp.multidict({(i, j, k): distance(i, j) * cost[k]
    for i in range(n + 1) for j in range(n + 1) if i != j for k in range(t)})

index = [(i, j) for i in range(n + 1) for j in range(n + 1) if i != j]

# create model
model = gp.Model("wfcrp")

# variables
x = model.addVars(index_with_cable, obj=m_cost, vtype=GRB.BINARY, name="x")
f = model.addVars(index, lb=0, ub=sum(production), name="f")

# constraints
if C < n:
  model.addConstr(x.sum('*', n, '*') <= C)

model.addConstr(x.sum(n, '*', '*') == 0)
model.addConstrs(x.sum(i, '*', '*') == 1 for i in range(n))
model.addConstrs(f.sum('*', h) + production[h] == f.sum(h, '*') for h in range(n))
model.addConstrs(f[i, j] <= gp.quicksum(x[i, j, k] * capacity[k] for k in range(t)) for i in range(n) for j in range(n + 1) if i != j)

model._flow = f
model._graph = x
model.Params.lazyconstraints = 1
model.optimize(callback)

# print result
vals = model.getAttr('x', x)
flow = model.getAttr('x', f)
g = [[] for _ in range(n + 1)]
for i in range(n):
  for j in range(n + 1):
    if i != j:
      for k in range(t):
        if vals[i, j, k] > .5:
          assert flow[i, j] > 0
          g[i].append((j, k))
          g[j].append((i, k))

x, y = zip(*turbines)
x = [k - substation[0] for k in x]
y = [k - substation[1] for k in y]

def dfs_print(u, p, s):
  for v, w in g[u]:
    if v != p:
      print('{}< {} {}'.format(' ' * s, v, w))
      dfs_print(v, u, s + 2)
    else:
      plt.plot([x[v], x[u]], [y[v], y[u]], color=colors[w], zorder=1)
  if u == n:
    g[u] = []
  else:
    g[u] = (p, s)

print('\nSolution: {}'.format(model.objVal))
print(n, '(Substation)')
dfs_print(n, -1, 0)

print('\nAdjacency list:')
print('\n'.join('{} -> {}'.format(i, x) for i, x in enumerate(g[:-1])))

plt.scatter(0, 0, marker='s', color='red', zorder=2)
plt.scatter(x[:-1], y[:-1], marker='2', s=200, color='black', zorder=2)
plt.savefig(RESULTS + filename + '.png', dpi=200)

#!/usr/bin/env python3.7

import gurobipy as gp
import matplotlib.pyplot as plt
import sys

from gurobipy import GRB
from itertools import permutations
from math import hypot


INSTANCES = '../../instances/'
RESULTS = '../../results/gurobi'
TIME_LIMIT = 2 * 60 * 60
#  TIME_LIMIT = None

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
      if flow[i, j] > 0.5:
        for h, k in segm:
          if intersect(i, j, h, k):
            model.cbLazy(model._graph.sum(i, j, '*') + model._graph.sum(h, k, '*') <= 1)
            return
        segm.append((i, j))


# objective variable
index_with_cable, conn_cost = gp.multidict({(i, j, k): distance(i, j) * cost[k]
    for i in range(n + 1) for j in range(n + 1) if i != j for k in range(t)})

index = [(i, j) for i in range(n + 1) for j in range(n + 1) if i != j]

# create model
model = gp.Model("wfcrp")

# variables
x = model.addVars(index_with_cable, obj=conn_cost, vtype=GRB.BINARY, name="x")
f = model.addVars(index, lb=0, ub=n, name="f")

# constraints
if C < n:
  model.addConstr(x.sum('*', n, '*') <= C)

model.addConstr(f.sum(n, '*') == 0)
model.addConstrs(x.sum(i, '*', '*') == 1 for i in range(n))
model.addConstrs(f.sum('*', h) + 1 == f.sum(h, '*') for h in range(n))
model.addConstrs(f[i, j] <= gp.quicksum(x[i, j, k] * capacity[k] for k in range(t)) for i in range(n) for j in range(n + 1) if i != j)

# optimization
if TIME_LIMIT != None:
  model.Params.TimeLimit = TIME_LIMIT - model.getAttr(GRB.Attr.Runtime)

model._flow = f
model._graph = x
model.Params.lazyconstraints = 1
model.optimize(callback)

# get result
result = model.objVal
vals = model.getAttr('x', x)
flow = model.getAttr('x', f)
g = [[] for _ in range(n + 1)]
for i in range(n):
  for j in range(n + 1):
    if i != j and flow[i, j] > 0.5:
      k = next(k for k in range(t) if vals[i, j, k] > 0.5)
      g[i].append((j, k))
      g[j].append((i, k))

def dfs_print(u, p):
  for v, w in g[u]:
    if v != p:
      dfs_print(v, u)
      g[v] = (u, w)
dfs_print(n, -1)

# print
print('\nSolução: {:0.15f}'.format(result))
print('Conexões:')
print('> Subestação = {}'.format(n))
print('> Valores: origem destino cabo')
print('\n'.join('{} {} {}'.format(i, *x) for i, x in enumerate(g[:-1])))

# plot
plot_location = [tuple(x[d] - substation[d] for d in range(2)) for x in turbines]
plotted = [False] * t
for i in range(n):
  a = plot_location[i]
  b = plot_location[g[i][0]]
  cable = g[i][1]

  label = None if plotted[cable] else 'Cabo {}'.format(cable + 1)
  plotted[cable] = True

  plt.plot([a[0], b[0]], [a[1], b[1]], color=colors[cable], zorder=1, label=label)

x, y = zip(*plot_location)
plt.scatter(x[-1], y[-1], marker='s', color='red', zorder=2, label='Subestação')
plt.scatter(x[:-1], y[:-1], marker='2', s=200, color='black', zorder=2, label='Turbina')
plt.legend(markerscale=.7, scatterpoints=1, fontsize=8)

plt.xlabel('offset = {}'.format(substation[0]))
plt.ylabel('offset = {}'.format(substation[1]))
plt.grid(linestyle=":")

plt.savefig(RESULTS + filename + '.png', dpi=200)

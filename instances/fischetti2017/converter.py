import sys


turb_file = sys.argv[1]
cable_file = sys.argv[2:]

with open(turb_file) as f:
  turbines = f.read().split('\n')

turbines = [tuple(x.split()[:-1]) for x in turbines if len(x.split()) > 0]

for filename in cable_file:
  with open(filename, 'r') as f:
    cables = f.read().split('\n')

  cables = [tuple(x.split()[:-1]) for x in cables if len(x.split()) > 0]

  with open(filename.split('.')[0] + '.crp', 'w') as f:
    f.write('{} {} {}\n'.format(len(turbines) - 1, len(cables), 10**6))
    f.write(''.join('{} {}\n'.format(x[0], x[1]) for x in turbines))
    f.write(''.join('{} {}\n'.format(x[0], x[1]) for x in cables))

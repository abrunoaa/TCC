files = [
  #  'wf02_cb01_capex',
  #  'wf02_cb02_capex',
  #  'wf02_cb04_capex',
  #  'wf02_cb05_capex',
  #  'wf03_cb03_capex',
  #  'wf03_cb04_capex',
  #  'wf01_cb01_capex',
  #  'wf01_cb02_capex',
  #  'wf01_cb05_capex',
  #  'wf04_cb01_capex',
  'wf04_cb05_capex',
  #  'wf05_cb04_capex',
  'wf05_cb05_capex',
]

import os

for f in files:
  print('----------------------------------------------------------')
  print('Processing {}.'.format(f))
  os.system("./main {}".format(f))
  #  break

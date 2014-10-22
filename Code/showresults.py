from pylab import *
import scipy
from postprocess import logsumexp

# Temperatures
T1, T2 = 0.1, 1.

# First calculate things about the scalars (e.g. the normalising constant)
scalars = loadtxt('scalars.txt')
sample_info = loadtxt('sample_info.txt')
smallest = min([scalars.shape[0], sample_info.shape[0]])
scalars = scalars[0:smallest, :]
sample_info = sample_info[0:smallest, :]

num_runs = sum(sample_info[:,1] == 1)
print('Number of runs = {n}'.format(n=num_runs))

## Simulate logX values
#logX = empty(scalars.shape[0])
#for i in xrange(0, num_runs):
#  which = nonzero()[0]

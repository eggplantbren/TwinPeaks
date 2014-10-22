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
print('Number of runs = {n}.'.format(n=num_runs))

# Simulate logX values but only for points with logY=0
which = sample_info[:,2] == 0
_scalars = scalars[which, :].copy()
_sample_info = sample_info[which, :].copy()

_logX = empty(_scalars.shape[0])
_logX[0] = log(scipy.random.beta(_sample_info[0, 0], 1))
for i in xrange(1, _logX.size):
  if _sample_info[i, 1] == (_sample_info[i-1, 1] + 1):
    _logX[i] = _logX[i-1] + log(scipy.random.beta(_sample_info[0, 0], 1))
  else:
    _logX[i] = log(scipy.random.beta(_sample_info[0, 0], 1))

# Associate logX values to the scalars
ii = argsort(_scalars[:,0])
_scalars = _scalars[ii, :]
_logX.sort()

# Now assign logX values to ALL points
logX = empty(scalars.shape[0])
for i in xrange(0, scalars.shape[0]):
  if sample_info[i, 2] == 0:
    # Find the scalars in _scalars
    which = nonzero(logical_and(scalars[i, 0]==_scalars[:,0],\
			scalars[i, 1]==_scalars[:,1]))[0]
    if len(which) != 1:
      print('Error')



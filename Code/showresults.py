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

# Assign a run identity to each point
run_id = zeros(sample_info.shape[0]).astype('int')
for i in xrange(1, sample_info.shape[0]):
  if sum(sample_info[i, 1:]) == 1:
    run_id[i] = run_id[i-1] + 1
  else:
    run_id[i] = run_id[i-1]

# Simulate logX and logY values
logX = empty(sample_info.shape[0])
logY = empty(sample_info.shape[0])
for i in xrange(0, sample_info.shape[0]):
  if i==0 or run_id[i] != run_id[i-1]:
    previous = [0., 0.]
    previous_row = array([sample_info[i, 0], 0., 0.])
  else:
    previous = [logX[i-1], logY[i-1]]
    previous_row = sample_info[i-1, :]

  if sample_info[i, 1] > previous_row[1]:
    logX[i] = previous[0] + log(scipy.random.beta(sample_info[i, 0], 1))
    logY[i] = previous[1]
  elif sample_info[i, 2] > previous_row[2]:
    logX[i] = previous[0]
    logY[i] = previous[1] + log(scipy.random.beta(sample_info[i, 0], 1))
  else:
    print('# This shouldn\'t happen.')

plot(logX[logY==0], scalars[logY==0,0], 'b.')
show()

# Sort subset of points with logY = 0.
which = logY == 0.
logX_sorted = logX[which].copy()
scalars_sorted = scalars[which, :].copy()
index = argsort(scalars_sorted[:,0])
scalars_sorted = scalars_sorted[index, :]
logX_sorted.sort()
logX_sorted = logX_sorted[::-1]

# Go through all points again
logX_new = empty(sample_info.shape[0])
for i in xrange(0, sample_info.shape[0]):
  # Look for the ancestor with logY = 0
  ancestor = nonzero(logical_and(logX == logX[i], logY == 0.))[0]
  ancestor = scalars[ancestor, :].flatten()

  # Find the ancestor's scalars in scalars_sorted
  where = logical_and(scalars_sorted[:,0] == ancestor[0],\
			scalars_sorted[:,1] == ancestor[1])
  where = nonzero(where)[0]
  logX_new[i] = logX_sorted[where]

plot(logX_new[logY==0], scalars[logY==0,0], 'b.')
show()


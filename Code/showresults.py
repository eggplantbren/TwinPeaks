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

# Don't need old logX assignments anymore
logX = logX_new

# Delete logY=0 points
keep = nonzero(logY != 0)[0]
logX = logX[keep]
logY = logY[keep]
scalars = scalars[keep, :]

# Clothesline plot
plot(logX, logY, 'b.', markersize=1)
xlim([1.05*logX.min(), -0.05*logX.min()])
ylim([1.05*logY.min(), -0.05*logY.min()])
xlabel(r'$\log(X)$', fontsize=16)
ylabel(r'$\log(Y)$', fontsize=16)
show()

# Define some useful functions
# for doing the integrals
def logsumexp(values):
  biggest = max(values)
  x = values - biggest
  result = log(sum(exp(x))) + biggest
  return result

def logdiffexp(x1, x2):
  biggest = x1
  xx1 = x1 - biggest
  xx2 = x2 - biggest
  result = log(exp(xx1) - exp(xx2)) + biggest
  return result


log_dx = empty(scalars.shape[0])
log_dy = empty(scalars.shape[0])
for i in xrange(0, scalars.shape[0]):
  # Find points to the left and right
  left = nonzero(logX < logX[i])[0]
  right = nonzero(logX > logX[i])[0]
  # Find neighbours
  if len(left) != 0:
    nearest_left = logX[left].max()
  else:
    nearest_left = logX[i]
  if len(right) != 0:
    nearest_right = logX[right].min()
  else:
    nearest_right = logX[i]
  log_dx[i] = logdiffexp(nearest_right, nearest_left) - log(2.)

  # Find points above and below
  below = nonzero(logical_and(logX == logX[i], logY < logY[i]))[0]
  above = nonzero(logical_and(logX == logX[i], logY > logY[i]))[0]
  # Find neighbours
  if len(below) != 0:
    nearest_below = logY[below].max()
  else:
    nearest_below = logY[i]
  if len(above) != 0:
    nearest_above = logY[above].min()
  else:
    nearest_above = logY[i]
  log_dy[i] = logdiffexp(nearest_above, nearest_below) - log(2.)

# Calculate a normalising constant
T1, T2 = 0.1, 1.
logL = scalars[:,0]/T1 + scalars[:,1]/T2

log_dx -= logsumexp(log_dx)
log_dy -= logsumexp(log_dy)

# Calculate prior weights and normalise
logprior = log_dx + log_dy
logprior -= logsumexp(logprior)
logZ = logsumexp(logprior + logL)
print('logZ = {logZ}'.format(logZ=logZ))


from pylab import *
from postprocess import logsumexp

output = atleast_2d(loadtxt('output.txt'))

w = ones(output.shape[0])#exp(output[:,0] - output[:,0].max())
w /= w.sum()
logw = log(w + 1E-300)

ENR = exp(-sum(w*logw))

logZ = logsumexp(logw + output[:,0])

print('logZ = {logZ}'.format(logZ=logZ))
print('N = {N}'.format(N=output.shape[0]))
print('ENR = {ENR}'.format(ENR=ENR))

hist(output[output[:,0] > -1E300, 0], 500, alpha=0.2)
axvline(-209.053, color='r')
show()


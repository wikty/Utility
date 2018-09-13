import numpy as np
import matplotlib.pyplot as plt

class KDE(object):

	def __init__(self, h=2, kernel_name='gaussian'):
		kernels = set(['gaussian', 'parzen'])
		if kernel_name not in kernels:
			raise Exception('invalid kernel name')
		if h <= 0:
			raise Exception('invalid h')
		self.h = h
		if kernel_name == 'parzen':
			self.kernel = self.parzen_kernel
		else:
			self.kernel = self.gaussian_kernel

	def gaussian_kernel(self, x, xn):
		return self.h*np.power(2*np.pi, 0.5)*np.exp(-(np.linalg.norm(x-xn))/(2*(self.h ** 2)))

	def parzen_kernel(self, x, xn):
		u = np.abs((x-xn) / self.h)
		if np.isscalar(x):
			if u > 1/2:
				return 0
			else:
				return 1/self.h
		for i in u:
			if i > 1/2:
				return 0
		return 1/np.power(self.h, len(x))

	def fit(self, x, xn):
		p = [np.average([self.kernel(i, j) for j in xn]) for i in x]
		return p / sum(p)
		
# def noise_data(n_sample=1000):
# 	x = np.linspace(0, np.pi, n_sample)
# 	y1 = np.sin(x)*10
# 	y2 = y1 + (np.random.rand(1, n_sample).flatten('C')-0.5)
# 	plt.plot(x, y1, color='g')
# 	plt.scatter(x, y2, color='r')
# 	plt.show()
# 	return (x, y2)

def noise_data(mu1, mu2, sigma, n_sample=1000):
	x1 = np.random.normal(mu1, sigma, n_sample)
	x2 = np.random.normal(mu2, sigma, n_sample)
	x3 = np.random.normal(mu2+10, sigma, n_sample)
	x = np.hstack((x1, x2, x3))
	return x

def test():
	N = 1000
	h = 0.4
	mu1, mu2, sigma = 0, 10, 3
	xn = noise_data(mu1, mu2, sigma, N)
	plt.hist(xn, bins='auto', color='b', normed=True)
	x = np.linspace(min(xn), max(xn), 100)
	kde = KDE(h, 'gaussian')
	p = kde.fit(x, xn)
	plt.plot(x, p, color='r')
	plt.show()
	
test()
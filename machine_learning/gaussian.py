import matplotlib
import numpy as np
from scipy.stats import multivariate_normal
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
import matplotlib.cm as cm
from mpl_toolkits.mplot3d import Axes3D # support 3d projection

# delta = 0.025
# matplotlib.rcParams['xtick.direction'] = 'out'
# matplotlib.rcParams['ytick.direction'] = 'out'

def sin_2d():
	# theta1, theta2 = np.meshgrid(np.linspace(0, 2*np.pi), np.linspace(0, 2*np.pi), sparse=True)
	theta1, theta2 = np.meshgrid(np.linspace(0, 2*np.pi), np.linspace(0, 2*np.pi))
	s = np.sin(theta1+theta2) # meshgrid is very useful to evaluate functions on a grid
	ax = plt.figure().gca(projection='3d')
	ax.plot_wireframe(theta1, theta2, s)
	x = theta1.flatten('C')  # ‘C’ means to flatten in row-major (C-language-style) order
	y = theta2.flatten('C')
	z = s.flatten('C')
	ax.scatter(x, y, z, c='r', marker='o')
	plt.show()

def cos_2d():
	theta1, theta2 = np.meshgrid(np.linspace(0, 2*np.pi), np.linspace(0, 2*np.pi))
	s = np.cos(theta1+theta2) # meshgrid is very useful to evaluate functions on a grid
	ax = plt.figure().gca(projection='3d')
	ax.plot_wireframe(theta1, theta2, s)
	x = theta1.flatten('C')  # ‘C’ means to flatten in row-major (C-language-style) order
	y = theta2.flatten('C')
	z = s.flatten('C')
	ax.scatter(x, y, z, c='r', marker='o')
	plt.show()

def univariate_gaussian(x, mu, sigma):
	# return (np.exp(-np.power(x - mu, 2.) / (2 * np.power(sigma, 2.)))) / (np.sqrt(2*np.pi)*sigma)
	return multivariate_normal.pdf(x, mu, sigma)

def plot_univariate_gaussian(mu=0, sigma=1, n_sample=100):
	x = np.linspace(mu-4*sigma, mu+4*sigma, n_sample)
	plt.plot(x, univariate_gaussian(x, mu, sigma))
	plt.show()

def mixture_univariate_gaussian(x, mu_list, sigma_list, coeff_list, precision=0.000000001):
	if np.sum(coeff_list) < (1-precision) or np.sum(coeff_list) > (1+precision):
		raise Exception('coeff sum should be equal to one')
	p = np.zeros(len(x))
	for mu, sigma, coeff in zip(mu_list, sigma_list, coeff_list):
		p += coeff*univariate_gaussian(x, mu, sigma)
	return p

def plot_mixture_univariate_gaussian(mu_list=None, sigma_list=None, coeff_list=None, n_mixture=2, n_sample=100, precision=0.0000000001):
	if mu_list is None:
		mu_list = (np.random.rand(n_mixture) - 0.5) * 100 # -50~50
	if sigma_list is None:
		sigma_list = (np.random.rand(n_mixture) * 10) + 2 # 2~12
	if coeff_list is None:
		coeff_list = np.array([1/n_mixture]*n_mixture)
	max_sigma = max(sigma_list)
	x = np.linspace(min(mu_list)-4*max_sigma, max(mu_list)+4*max_sigma, n_sample)
	# for mu, sigma in zip(mu_list, sigma_list):
	# 	plt.plot(x, univariate_gaussian(x, mu, sigma))
	plt.plot(x, mixture_univariate_gaussian(x, mu_list, sigma_list, coeff_list))
	plt.show()

def multivariate_gaussian(x, mu, cov=None):
	# covariance matrix must be positive define matrix
	if cov is None:
		cov = np.identity(len(mu))
	g = multivariate_normal(mu, cov)
	return g.pdf(x)

def plot_bivariate_gaussian(mu=None, cov=None, n_sample=100):
	if mu is None:
		mu = np.zeros(2)
	if cov is None:
		cov = np.identity(2)
	x, y = np.meshgrid(
		np.linspace(mu[0]-4*cov[0][0], mu[0]+4*cov[0][0], n_sample), 
		np.linspace(mu[1]-4*cov[1][1], mu[1]+4*cov[1][1], n_sample)
	)
	z = multivariate_gaussian(np.dstack((x, y)), mu, cov)
	ax = plt.figure().gca(projection='3d')
	# ax.scatter(x, y, z, c='r', marker='o')
	ax.plot_wireframe(x, y, z)
	plt.show()

def mixture_multivariate_gaussian(x, mu_list, cov_list, coeff_list, precision=0.0000000001):
	# covariance matrix must be positive define matrix
	if np.sum(coeff_list) < (1-precision) or np.sum(coeff_list) > (1+precision):
		raise Exception('coeff sum should be equal to one')

	p = np.zeros(len(x))
	for mu, cov, coeff in zip(mu_list, cov_list, coeff_list):
		p += coeff*multivariate_gaussian(x, mu, cov)
	return p

def plot_mixture_bivariate_gaussian(mu_list=None, cov_list=None, coeff_list=None, n_mixture=3, n_sample=100, precision=0.0000000001):
	if mu_list is None and coeff_list is None:
		mu_list = (np.random.rand(n_mixture, 2) - 0.5) * 100 # -50 ~ 50
		# covariance matrix must be positive define matrix
		cov_list = np.array([np.identity(2)*np.random.random_integers(5, 10) for i in range(n_mixture)])  # 5 ~ 10
	
	if coeff_list is None:
		coeff_list = np.array([1/n_mixture]*n_mixture)

	max_mu = np.amax(mu_list, axis=0)
	min_mu = np.amin(mu_list, axis=0)
	diagonal = np.vstack((cov.diagonal() for cov in cov_list))
	max_sigma = np.amax(diagonal, axis=0)
	x, y = np.meshgrid(
		np.linspace(min_mu[0]-4*max_sigma[0], max_mu[0]+4*max_sigma[0], n_sample), 
		np.linspace(min_mu[1]-4*max_sigma[1], max_mu[1]+4*max_sigma[1], n_sample)
	)
	x = x.flatten('C')
	y = y.flatten('C')
	z = mixture_multivariate_gaussian(np.array(list(zip(x, y))), mu_list, cov_list, coeff_list)
	ax = plt.figure().gca(projection='3d')
	ax.plot_wireframe(x, y, z)
	plt.show()



# plot_univariate_gaussian()
# plot_univariate_gaussian(5, 7)
# plot_mixture_univariate_gaussian(n_mixture=5)
# plot_bivariate_gaussian()
# plot_bivariate_gaussian(np.array([0, 0]), np.identity(2)*5)
# plot_mixture_bivariate_gaussian()
# plot_mixture_bivariate_gaussian(np.array([[-3, -3], [3, 3]]), np.array([np.identity(2)*5, np.identity(2)*5]), np.array([0.5, 0.5]), 2)
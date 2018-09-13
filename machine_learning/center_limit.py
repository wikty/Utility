import numpy as np
from scipy.stats import norm
import matplotlib.pyplot as plt

def plot(n=10):
	n_sample = 1000
	data = np.zeros(n_sample)
	for i in range(n):
		mu = np.random.randint(-100, 100)
		sigma = np.random.randint(1, 10) + np.random.random()
		data += np.random.normal(mu, sigma, n_sample)
	plt.hist(data, bins='auto', normed=True, alpha=0.6, color='g')
	xmin, xmax = plt.xlim()
	mu, std = norm.fit(data)
	x = np.linspace(xmin, xmax, 100)
	p = norm.pdf(x, mu, std)
	plt.plot(x, p, 'k', linewidth=2)
	title = "Fit Normal: mu = %.2f,  std = %.2f" % (mu, std)
	plt.title(title)
	plt.show()

plot(100)
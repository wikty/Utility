import numpy as np
from scipy.stats import norm
import matplotlib.pyplot as plt


def plot(data, with_hist=True):
	if with_hist:
		plt.hist(data, bins='auto', normed=True, alpha=0.6, color='g')
		xmin, xmax = plt.xlim()
	else:
		xmin, xmax = np.min(data)-10, np.max(data)+10
	mu, std = norm.fit(data)
	x = np.linspace(xmin, xmax, 100)
	p = norm.pdf(x, mu, std)
	plt.plot(x, p, 'k', linewidth=2)
	title = "Fit Normal: mu = %.2f,  std = %.2f" % (mu, std)
	plt.title(title)
	plt.show()
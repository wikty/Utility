#!/usr/bin/env python

import sys
import logging

import numpy as np

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plot

from sklearn.cluster import estimate_bandwidth, MeanShift, get_bin_seeds
from sklearn.datasets.samples_generator import make_blobs


def test_mean_shift():
    logging.debug('Generating mixture')
    count = 5000
    blocks = 7
    std_error = 0.5
    mixture, clusters = make_blobs(n_samples=count, centers=blocks, cluster_std=std_error)

    logging.debug('Measuring bendwith')
    bandwidth = estimate_bandwidth(mixture)
    logging.debug('Bandwidth: %r' % bandwidth)

    mean_shift = MeanShift(bandwidth=bandwidth)

    logging.debug('Clustering')
    mean_shift.fit(mixture)

    shifted = mean_shift.cluster_centers_
    guess = mean_shift.labels_

    logging.debug('Centers: %r' % shifted)

    def draw_mixture(mixture, clusters, output='mixture.png'):
        plot.clf()
        plot.scatter(mixture[:, 0], mixture[:, 1],
                     c=clusters,
                     cmap=plot.cm.coolwarm)
        plot.savefig(output)

    def draw_mixture_shifted(mixture, shifted, output='mixture_shifted.png'):
        plot.clf()
        plot.scatter(mixture[:, 0], mixture[:, 1], c='r')
        plot.scatter(shifted[:, 0], shifted[:, 1], c='b')
        plot.savefig(output)

    logging.debug('Drawing')
    draw_mixture_shifted(mixture, shifted)
    draw_mixture(mixture, guess)


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)

    test_mean_shift()
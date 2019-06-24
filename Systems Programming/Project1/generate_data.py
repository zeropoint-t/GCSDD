#! /usr/bin/env python3

import argparse
import cmath
import itertools
import math
import random
import sys


parser = argparse.ArgumentParser(
    description="Generate randomly-distributed points in a ring around the origin",
    formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('--distribution', type=str, default='ring', help='probability distribution: one of {uniform, ring, gauss}')
parser.add_argument('--inner', type=float, default=0.0, help='inner radius')
parser.add_argument('--outer', type=float, default=1.0, help='outer radius')
parser.add_argument('--mean', type=float, default=0.0, help='mean (gaussian)')
parser.add_argument('--deviation', type=float, default=0.25, help='standard deviation (gaussian)')
parser.add_argument('n', type=int, help='number of points to generate')
args = parser.parse_args()
n = args.n
distribution = args.distribution
inner = args.inner
outer = args.outer
ring = outer - inner
mean = args.mean
deviation = args.deviation


def counter(n=None):
    if n is None:
        return itertools.count()
    else:
        return range(n)


def gaussian(n=None):
    gen = lambda: min(1.0, max(-1.0, random.gauss(mean, deviation)))
    for _ in counter(n):
        yield gen(), gen()


def uniform(n=None):
    for _ in counter(n):
        x = (2 * random.random() - 1.0)
        y = (2 * random.random() - 1.0)
        yield x, y


def ring(inner, outer, n=None):
    ring = outer - inner
    for x, y in uniform(n):
        # (x, y) is a random point on the unit circle
        z = complex(x, y)
        r, phi = abs(z), cmath.phase(z)
        r = inner + r * (ring)
        z = cmath.rect(r, phi)
        yield z.real, z.imag


if distribution == 'gauss':
    generator = gaussian(n)
elif distribution == 'uniform':
    generator = uniform(n)
elif distribution == 'ring':
    generator = ring(inner, outer, n)
else:
    sys.stderr.write("invalid distribution: {}\n".format(distribution))
    sys.exit(1)

for x, y in generator:
    print("{}\t{}".format(x, y))

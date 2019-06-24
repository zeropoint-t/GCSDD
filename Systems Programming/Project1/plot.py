#! /usr/bin/env python3

import sys

PROLOG = """<svg width="{dimension}" height="{dimension}" xmlns="http://www.w3.org/2000/svg" >
<rect width="100%" height="100%" fill="white" />
"""

POINT = '<circle cx="{x}" cy="{y}" r="{width}" fill="{color}" />\n'

POLY= """<polyline
   points="{data}"
   stroke="{color}" stroke-width="{width}" fill="none" />
"""

GRID="""
   <defs>
      <pattern id="smallGrid" width="10" height="10" patternUnits="userSpaceOnUse">
        <path d="M 10 0 L 0 0 0 10" fill="none" stroke="gray" stroke-width="0.5"/>
      </pattern>
      <pattern id="grid" width="100" height="100" patternUnits="userSpaceOnUse">
        <rect width="100" height="100" fill="url(#smallGrid)"/>
        <path d="M 100 0 L 0 0 0 100" fill="none" stroke="gray" stroke-width="1"/>
      </pattern>
    </defs>
    <rect width="100%" height="100%" fill="url(#grid)" />
"""


def load(f):
    return [(float(x), float(y)) for x, y in (l.strip().split() for l in f)]


class Svg(object):
    def __init__(self, out=sys.stdout):
        self.dimension = 1600
        self.scale = self.dimension / 2.0
        self.out = out
        self.prolog()

    def close(self):
        self.epilog()
        self.out.close()

    def prolog(self):
        self.out.write(PROLOG.format(dimension=self.dimension))
        self.out.write(GRID)

    def epilog(self):
        self.out.write('</svg>\n')

    def transform_x(self, x):
        return int(self.scale * (1.0 + x))

    def transform_y(self, y):
        return int(self.scale * (1.0 - y))

    def transform(self, p):
        return (self.transform_x(p[0]), self.transform_y(p[1]))

    def emit_point(self, p, color='black', width=2):
        x, y = self.transform(p)
        self.out.write(POINT.format(color=color, x=x, y=y, width=width))

    def emit_points(self, points, color='black', width=4):
        for p in points: self.emit_point(p, color, width)

    def emit_polyline(self, points, color='blue', width=4):
        data = ' '.join('{} {}'.format(x, y) for x, y in (self.transform(p) for p in points))
        self.out.write(POLY.format(data=data, color=color, width=width))

    def emit_polygon(self, points, color='blue', width=4):
        self.emit_polyline(points + [points[0]], color=color, width=width)


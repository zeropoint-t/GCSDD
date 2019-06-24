// graham.cc
//
// Implement Graham scan algorithm.  If compiled with -DSVG, emit
// image frames at critical points for animating the algorithm.
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#include <algorithm>
#include <cassert>
#include <iostream>

#include "point.h"
#include "svg.h"

using namespace std;


void Points::graham_scan(Points& hull) {
  assert(points.size() > 0);
  assert(hull.points.size() == 0);

#ifdef SVG
  Svg svg;
#endif

  sort();

  if (points.size() <= 3) {
    hull.points = points;
    return;
  }

  hull.points.push_back(points[0]);
  hull.points.push_back(points[1]);

  int n = points.size();
  int m = 2;

  for (int i = 2; i < n; ++i) {
    Point p = points[i];

#ifdef SVG
    svg.frame("green", p, hull.points, points);
#endif

    //cerr << "adding " << p << endl;
    while (m >= 2 && !Point::is_left_turn(hull.points[m - 2], hull.points[m - 1], p)) {
      //cerr << "dropping " << points[m - 1] << endl;

#ifdef SVG
      svg.frame("red", p, hull.points, points);
#endif

      hull.points.pop_back();
      --m;
    }

#ifdef SVG
    svg.frame("blue", p, hull.points, points);
#endif

    hull.points.push_back(p);
    ++m;
  }

  while (!Point::is_left_turn(hull.points[m - 2], hull.points[m - 1], hull.points[0])) {
#ifdef SVG
    svg.frame("green", hull.points[0], hull.points, points);
    svg.frame("red", hull.points[0], hull.points, points);
#endif
    hull.points.pop_back();
    --m;
  }

#ifdef SVG
  svg.frame("blue", hull.points[0], hull.points, points);
  svg.frame("blue", hull.points[0], hull.points, points);
  svg.frame("blue", hull.points[0], hull.points, points);
  svg.frame("blue", hull.points[0], hull.points, points);
#endif
}

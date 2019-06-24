// slice.cc
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#include "slice.h"

#include <algorithm>
#include <cfloat>
#include <cstdlib>
#include <utility>

using namespace std;


// Sort points counterclockwise wrt the lowest, leftmost point.
void Slice::sort() {
  // Find lowest, leftmost point.
  Point p_min = Point(0.0, DBL_MAX);
  for (std::size_t i = 0; i < size; ++i) {
    if (points[i] < p_min) {
      p_min = points[i];
    }
  }

  // Heavy lifting is done by the standard library.
  ::sort(points, points + size, Point_Comparator{p_min});
}

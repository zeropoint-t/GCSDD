// slice.h
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#if !defined(SLICE_H)
#define SLICE_H

#include "point.h"

#include <iostream>


// Pointer into contiguous range of points within a Points object.
//
// The subrange can be sorted or the convex hull may be computed using
// Divide and Conquer.
class Slice {
public:
  Slice(Points& points):
    size(points.points.size()), points(&points.points[0]) {}
  Slice(Points& points, std::size_t start, std::size_t size):
    size(size), points(&points.points[start]) {}
  Slice(Point* start, std::size_t size): size(size), points(start) {}

  void sort();

  void graham(Points& hull);

  void divide_and_conquer(int n_processors, Points& hull);

  void dump(std::ostream& out=std::cerr) const {
    //out << "   size: " << size << std::endl;
    for (const Point* p = points; p != points + size; ++p) {
      out << "   " << *p << std::endl;
    }
  }

private:
  friend class Points;

  const std::size_t size;
  Point* points;
};


#endif

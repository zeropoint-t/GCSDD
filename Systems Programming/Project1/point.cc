//
// point.cc
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#include "point.h"
#include "slice.h"

#include <cassert>
#include <cstdlib>
#include <cfloat>
#include <limits>
#include <random>

#include <unistd.h>

using namespace std;


ostream& operator<<(ostream& out, const Point& p) {
  return out <<  p.x << "\t" << p.y;
}


// Generate n uniformly distributed random points inside the unit circle.
Points::Points(int nPoints) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  for (int i = 0; i < nPoints; ++i) {
    points.push_back(Point(distribution(generator), distribution(generator)));
  }
}


Points::Points(istream& in) {
  double x;
  while (in >> x) {
    double y;
    in >> y;
    points.push_back(Point(x, y));
  }
}


void Points::read_raw(int fd) {
  int rc;
  size_t n;
  Point p;

  rc = read(fd, reinterpret_cast<char *>(&n), sizeof(size_t));
  if (rc < static_cast<int>(sizeof(size_t))) {
    perror("reading size");
    exit(EXIT_FAILURE);
  }

  while(n-- > 0) {
    rc = read(fd, &p.x, sizeof(double));
    if (rc < static_cast<int>(sizeof(double))) {
      perror("reading data");
      exit(EXIT_FAILURE);
    }
    rc = read(fd, &p.y, sizeof(double));
    if (rc < static_cast<int>(sizeof(double))) {
      perror("reading data");
      exit(EXIT_FAILURE);
    }
    points.push_back(p);
  }
}

void Points::write_raw(int fd) {
  int rc;
  size_t n = points.size();

  rc = write(fd, &n, sizeof(size_t));
  if (rc < static_cast<int>(sizeof(n))) {
    perror("writing size");
    exit(EXIT_FAILURE);
  }

  for (Point* p = &points[0]; n-- > 0; ++p) {
    rc = write(fd, &(p->x), sizeof(double));
    if (rc < static_cast<int>(sizeof(double))) {
      perror("writing data");
      exit(EXIT_FAILURE);
    }
    rc = write(fd, &(p->y), sizeof(double));
    if (rc < static_cast<int>(sizeof(double))) {
      perror("writing data");
      exit(EXIT_FAILURE);
    }
  }
}


void Points::sort() {
  Slice slice{*this};
  slice.sort();
}

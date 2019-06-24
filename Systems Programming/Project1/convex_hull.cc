//
// convex_hull.cc
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#include "point.h"

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;


// Compute time difference as a string.
string delta_t(struct timespec *end, struct timespec *start) {
  time_t sec = end->tv_sec - start->tv_sec;
  long ns = end->tv_nsec - start->tv_nsec;
  if (ns < 0) {
    ns += 1000000000; // 1e9
    sec--;
  }
  double elapsed = double(sec) + double(ns) / 1e9;

  const size_t BUF_SIZE = 32;
  char buf[BUF_SIZE];
  snprintf(buf, BUF_SIZE, "%0.6f", elapsed);
  return string{buf};
}


// Print message indicating elapsed time for operation.
void elapsed(const char *message, timespec *end, timespec *start) {
  cerr << message << ": "
       << delta_t(end, start)
       << " seconds elapsed"
       << endl;
}


void graham(Points *points) {
  struct timespec start;
  clock_gettime(CLOCK_REALTIME, &start);

  Points hull;
  points->graham_scan(hull);

  struct timespec end;
  clock_gettime(CLOCK_REALTIME, &end);
  elapsed("Graham scan", &end, &start);

  cout << "@@@@@@@@@@@@@@@@@@@@@@@@> convex hull (Graham scan):" << endl;
  hull.dump(cout);
}


void divide_and_conquer(int n_processors, Points *points) {
  struct timespec start;
  clock_gettime(CLOCK_REALTIME, &start);

  Points hull;
  points->divide_and_conquer(n_processors, hull);

  struct timespec end;
  clock_gettime(CLOCK_REALTIME, &end);
  elapsed("divide and conquer", &end, &start);

  cout << "@@@@@@@@@@@@@@@@@@@@@@@@> convex hull (divide & conquer):" << endl;
  hull.dump(cout);
}


void usage(const char* name) {
  cerr << "Usage: " << name << " [nProcessors [nPoints]]" << endl;
  cerr << "If [nPoints] is not provided, points are read from standard input." << endl;
  cerr << "If [nProcessors] is not provided, assume a single processor." << endl;
  exit(EXIT_FAILURE);
}

int main(int argc, const char* argv[]) {
  if (argc > 3) {
    usage(argv[0]);
  }

  struct timespec resolution;
  clock_getres(CLOCK_REALTIME, &resolution);
  cerr << "timer resolution: " << resolution.tv_nsec << " ns" << endl;

  struct timespec start_load;
  clock_gettime(CLOCK_REALTIME, &start_load);

  int n_processors = 1;
  if (argc >= 2) {
    n_processors = atoi(argv[1]);
  }

  Points* points1;
#if 0
  if (argc >= 3) {
    int n = atoi(argv[2]);
    // Randomly-generated set of n points.
    points1 = new Points {n};
  } else {
    // Read data from standard input.
    points1 = new Points {cin};
  }
#else
  // Always read data from standard input.
  points1 = new Points {cin};
#endif

  // Make a copy of the original points because graham scan modifies
  // points in-place (sorts by polar coordinate around lowest,
  // leftmost point).
  Points* points2 = new Points(*points1);

  struct timespec end_load;
  clock_gettime(CLOCK_REALTIME, &end_load);
  elapsed("Loading: ", &end_load, &start_load);

  // Since we always read from standard input, no need to output data.
#if 0
  cout << "@@@@@@@@@@@@@@@@@@@@@@@@> initial points:" << endl;
  points1->dump(cout);
#endif


#if defined(GRAHAM)
  // Pure Graham scan solution for comparison.
  graham(points1);
#endif

  divide_and_conquer(n_processors, points2);

  return EXIT_SUCCESS;
}

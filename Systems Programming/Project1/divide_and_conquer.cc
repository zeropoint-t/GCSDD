//
// divide_and_conquer.cc
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#include "point.h"
#include "slice.h"

#include <cassert>
#include <iostream>
#include <utility>

#if defined(FORK)
// CSS503
// Add whatever header files required to implement your solution.
#include "solution/fork-includes.h"
#endif


using namespace std;

// Compute the convex hull of the slice and return results in hull.
//
// Assumes points are already sorted
void Slice::graham(Points& hull) {
#if defined(VERBOSE) && 0
  cerr << "########################> slice: " << this <<  " begin Graham scan" << endl;
  dump(cerr);
#endif

  // Trivial case: shouldn't happen.
  if (size <= 3) {
    for(size_t i = 0; i < size; ++i) {
      hull.points.push_back(points[i]);
    }
    return;
  }

  hull.points.push_back(points[0]);
  hull.points.push_back(points[1]);
  size_t m = 2;			// Current number of points in hull.
  for (size_t j = 2; j < size; ++j) {
    Point p = points[j];	// TODO: use iterator.
    while (m >= 2 &&  !Point::is_left_turn(hull.points[m - 2], hull.points[m - 1], p)) {
      hull.points.pop_back();
      --m;
    }
    hull.points.push_back(p);
    ++m;
  }
  while (!Point::is_left_turn(hull.points[m - 2], hull.points[m - 1], hull.points[0])) {
    hull.points.pop_back();
    --m;
  }
#if defined(VERBOSE) && 0
  cerr << "########################> hull: " <<  &hull << " (Graham scan of " << this << ")" << endl;
  hull.dump(cerr);
#endif
}

// Compute the convex hull by dividing input into two parts,
// computing the hull of each part recursively, and merging the
// hulls.  Switch to straight Graham scan when down to a single
// processor or the number of points is small.  Save results in hull
// parameter.
void Slice::divide_and_conquer(int n_processors, Points& hull) {
  assert(size > 0);
  assert(n_processors > 0);

#if defined(VERBOSE) && 0
  cerr << "########################> divide and conquer: " <<  this << endl;
  dump(cerr);
#endif

  // Base case: down to a single processor or small number of
  // points. "Small" was chosen arbitrarily.
  if (n_processors <= 1  || size <= 8) {
    // sorting is O(size log size); graham scan is linear
    sort();
    graham(hull);
    return;
  }

#if defined(FORK)

  // CSS503
  // Compile with -DFORK for multi-process solution

  // Compute hulls of both halves of the input points by opening a
  // pipe and forking the process.  The child process should compute
  // the hull of half the points and pipe the result back to the
  // parent process.  The parent process should compute the hull of
  // other half and read first hull from the child process.
  //
  // The parent process should terminate with error status if it fails
  // to fork a child or if the child terminates abnormally.
  //
  // Don't leak open files.

  // Put your solution here.
#include "solution/fork.inc"

#else

  // Recursively compute hulls of both halves of the input points.

  Slice slice1{points, (size + 1) / 2};
  Points hull1;
  slice1.divide_and_conquer((n_processors + 1) / 2, hull1);

  Slice slice2{points + (size + 1) / 2, size / 2};
  Points hull2;
  slice2.divide_and_conquer(n_processors / 2, hull2);

#endif

  if (hull1.points[0] < hull2.points[0]) {
    hull.merge(hull1, hull2);
  } else {
    hull.merge(hull2, hull1);
  }
}


// Merge two convex hulls. Points in the hulls are ordered clockwise
// with lowest, leftmost point first.
void Points::merge(Points& hull1, Points& hull2) {
  assert(hull1.points.size() > 0);
  assert(hull2.points.size() > 0);
  assert(!(hull2.points[0] < hull1.points[0]));

#if defined(VERBOSE)
  cerr << "########################> " << this << ": merging: " <<  &hull1 << " and " << &hull2 << endl;
  cerr << "$$$$$$$$$$$$$$$$$$$$$$$$> hull 1 " << &hull1 << endl;
  hull1.dump(cerr);
  cerr << "$$$$$$$$$$$$$$$$$$$$$$$$> hull 2 " << &hull2 << endl;
  hull2.dump(cerr);
#endif

  Point p_min = hull1.points[0];

  Point_Comparator cmp{p_min};

  // Scan hull2 to find outer wedge.

  size_t lo = 0;
  if (hull2.points[lo] == hull1.points[0]) {
    // discard duplicated point: it can't be on the merged hull.
    ++lo;
  }

  // Add "first" point to back end so that we'll be able to test the
  // last point against the "next" point.  Okay to modify hull2
  // because hull2 will be discarded immediately afterwards.
  size_t n = hull2.points.size();
  hull2.points.push_back(hull2.points[lo]);

  for(;; ++lo) {
    assert(lo < n);
    if (cmp(hull2.points[lo], hull2.points[lo + 1])) {
      // Found minimum-angle point: start of upper bridge.
      break;
    }
  }
  size_t hi = lo + 1;
  for (; hi < n; ++hi) {
    if (!cmp(hull2.points[hi], hull2.points[hi + 1])) {
      // found maximum-angle point: end of upper bridge.
      break;
    }
  }

  // Merge points of hull1 and the outer wedge slice of hull2

  Slice hull1_slice{hull1};
  Slice hull2_slice{hull2, lo, hi + 1 - lo};

#if defined(VERBOSE)
  cerr << "$$$$$$$$$$$$$$$$$$$$$$$$> hull 2 slice" << endl;
  hull2_slice.dump(cerr);
#endif

  Points merged;
  merged.push_back(hull1_slice.points[0]);
  size_t i1 = 1;
  size_t i2 = 0;

  //cerr << "+++ merge: hull 1 size(" << hull1_slice.size << ") hull size (" << hull2_slice.size << ")" << endl;
  while (i1 < hull1_slice.size && i2 < hull2_slice.size) {
    //cerr << "+++ merge: i1(" << i1 << ") i2(" << i2 << ")" << endl;
    if (cmp(hull1_slice.points[i1], hull2_slice.points[i2])) {
      //cerr << "+++ merge: adding i1 to merge " << hull1_slice.points[i1] << endl;
      merged.points.push_back(hull1_slice.points[i1++]);
    } else if (cmp(hull2_slice.points[i2], hull1_slice.points[i1])) {
      //cerr << "+++ merge: adding i1 to merge " << hull2_slice.points[i2] << endl;
      merged.points.push_back(hull2_slice.points[i2++]);
    } else {
      //cerr << "+++ merge: equal points "
      //     << hull1_slice.points[i1]
      //     << " and "
      //     << hull2_slice.points[i2]
      //     << endl;
      // Equal points: discard one.
      merged.points.push_back(hull1_slice.points[i1++]);
      ++i2;
    }
  }
  //cerr << "+++ merge: break from loop i1(" << i1 << ") i2(" << i2 << ")" << endl;
  while (i1 < hull1_slice.size) {
    //cerr << "+++ merge: adding  i1(" << i1 << ")" << endl;
    merged.points.push_back(hull1_slice.points[i1++]);
  }
  while (i2 < hull2_slice.size) {
    //cerr << "+++ merge: adding i2(" << i2 << ")" << endl;
    merged.points.push_back(hull2_slice.points[i2++]);
  }

  //cerr << "+++ merge: size of merged points: " << merged.points.size() << endl;

#if defined(VERBOSE)
  cerr << "$$$$$$$$$$$$$$$$$$$$$$$$> " << this << " merged points" << endl;
  merged.dump(cerr);
#endif

  Slice merged_slice(merged);
  merged_slice.graham(*this);

#if defined(VERBOSE)
  cerr << "$$$$$$$$$$$$$$$$$$$$$$$$> " << this << " merged hull" << endl;
  dump(cerr);
  cerr << "$$$$$$$$$$$$$$$$$$$$$$$$> " << endl;
#endif
}


void Points::divide_and_conquer(int n_processors, Points& hull) {
  assert(hull.points.size() == 0);

  if (points.size() <= 8) {
    graham_scan(hull);
    return;
  }

  Slice s{*this, 0, points.size()};
  s.divide_and_conquer(n_processors, hull);
}

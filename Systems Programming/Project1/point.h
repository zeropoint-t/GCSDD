// point.h
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#if !defined(POINT_H)
#define POINT_H

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

struct Point;

std::ostream& operator<<(std::ostream& out, const Point& p);


struct Point {
  Point(): x(0.0), y(0.0) {}
  Point(double x, double y) : x(x), y(y) {}

  Point& operator=(const Point&) = default;

  bool operator==(const Point& other) const {return x == other.x && y == other.y;}

  // Point is lower or same and lefter than other.
  bool operator<(const Point& other) const {
    return (y < other.y) || ((y == other.y) && (x < other.x));
  }

  Point operator+(const Point& other) const {return Point(x + other.x, y + other.y);}

  Point operator-(const Point& other) const {return Point(x - other.x, y - other.y);}

  static double slope(const Point& p1, const Point& p2) {
    return (p2.y - p1.y) / (p2.x - p1.x);
  }

  static double length_sq(const Point& p1, const Point& p2) {
    double x = p2.x - p1.x;
    double y = p2.y - p1.y;
    return (x * x) + (y * y);
  }

  static double cross_z(const Point& a, const Point& b) {
    double z = a.x * b.y - a.y * b.x;
    //cerr << "cross_z: " << z << endl;
    return z;
  }

  static bool is_left_turn(const Point& p1, const Point& p2, const Point& p3) {
    //cerr << "is_left_turn " << p1 << p2 << p3 << endl;
    return cross_z(p2 - p1, p3 - p1) > 0;
  }

  static int compare_slopes(const Point& a, const Point& b) {
    // Given two vectors defined by endpoints, return -1, 0, 1 if v1
    // is  shallower, same, or steeper than v2.
    return -(a.x * b.y - a.y * b.x);
  }

  double x;
  double y;
};


// Less-than operator for points: all points are above p_min. Two
// points are compared for their polar angle theta around p_min.
// Theta is in the range of [0, pi).  Use a proxy for theta to avoid
// expensive trig/square-root operations.
class Point_Comparator{
public:
  Point_Comparator(const Point& p_min) : p_min(p_min) {}

  bool operator()(const Point& p1, const Point& p2) const {
    if (p2 == p_min) {
      return false;
    }
    if (p1 == p_min) {
      return true;
    }
    double z =  Point::cross_z(p1 - p_min, p2 - p_min);
    if (z == 0) {
      // colinear points: "smaller" is closer to y_min.
      return p1.y < p2.y;
    }
    return z > 0;
  }

private:
  const Point p_min;
};


class Slice;


class Points {
public:
  Points() {}
  Points(int nPoints);
  Points(std::istream& in);
  Points(const Points& old): points{old.points} {}
  void push_back(const Point& p) {points.push_back(p);}

  void read_raw(int fd);
  void write_raw(int fd);

  void sort();

  void graham_scan(Points& hull);
  void divide_and_conquer(int n_processors, Points& hull);

  void dump(std::ostream& out=std::cerr) const {
    for (auto& p: points) {out << p << std::endl;}
  }

private:
  void merge(Points& hull1, Points& hull2);
  void merge1(const Points& hull1, const Points& hull2);

  friend class Slice;

  std::vector<Point> points;
};


#endif

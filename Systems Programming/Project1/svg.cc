// svg.cc
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "svg.h"

using namespace std;


#define FILENAME_BUF_SIZE 1023

static const int dimension = 800;
static const double shift = dimension / 2.0;


static int transform_x(double x) {
  return static_cast<int> (shift * (1.0 + x));
}

static int transform_y(double y) {
  return static_cast<int> (shift * (1.0 - y));
}


void Svg::frame(const char* color,
                const Point& p,
                const vector<Point>& hull,
                const vector<Point>& data) {

  ++frame_number;
  char filename[FILENAME_BUF_SIZE + 1];
  snprintf(filename, FILENAME_BUF_SIZE, "frame-%06d.svg", frame_number);
  ofstream out {filename};

  out << "<svg"
      << " width=\"" << dimension << "\""
      << " height=\"" << dimension << "\""
    //<< " style=\"stroke-width: 0 background: white\""
      << " >"
      << endl;

  out << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>" << endl;

  for(auto p: data) {
    out << "<circle"
        << " cx=\"" << transform_x(p.x) << "\""
        << " cy=\"" << transform_y(p.y) << "\""
        << " r = \"6\""
        << " fill=\"black\""
        << " />"
        << endl;
  }

  out << "<polyline" << endl;
  out << "    points=\"";
  for (auto it = &(hull[0]); it != &(hull.back()); ++it) {
    out << " " << transform_x(it->x) << " "  << transform_y(it->y) << " ";
  }
  out << "\"" << endl;
  out << "    stroke=\"blue\" stroke-width=\"1\" fill=\"none\" />" << endl;

  const Point& p0 = *(&hull.back() - 1);
  const Point& p1 = hull.back();
  out << "<polyline"
      << " points=\""
      << " " << transform_x(p0.x) << " " << transform_y(p0.y)
      << " " << transform_x(p1.x) << " " << transform_y(p1.y)
      << " " << transform_x(p.x) << " " << transform_y(p.y)
      << "\""
      << " stroke=\"" << color << "\""
      << " stroke-width=\"1\" fill=\"none\" />"
      << endl;


  out << "</svg>" << endl;
}

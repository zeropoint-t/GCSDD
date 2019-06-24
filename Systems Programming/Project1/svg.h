// svg.h
//
// Author: Morris Bernstein
// Copyright 2019, Systems Deployment, LLC.

#if !defined(SVG_H)
#define SVG_H

#include <vector>

#include "point.h"

class Svg {
public:
  Svg() : frame_number(0) {}

  void frame(const char* color,
	     const Point& p,
	     const std::vector<Point>& hull,
	     const std::vector<Point>& data);

private:
  int frame_number;
};

#endif

#pragma once
#include <iostream>
#include <math.h>
// Contains structs and functions for collision detection
struct AABB {
  uint32_t id;
  float x;
  float y;
  int w;
  int h;
};

uint32_t uid() {
  static uint32_t id = 0;
  id++;
  return id;
}

inline bool intersect(AABB A, AABB B) {
  return (abs(A.x - B.x) * 2 < (A.w + B.w)) &&
         (abs(A.y - B.y) * 2 < (A.h + B.h));
}

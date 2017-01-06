#pragma once
#include <stdint.h>
#include <vector>
#include <array>
#include "aabb.h"

enum { NW, NE, SW, SE };

struct search_result {
  std::vector<AABB> neighbours;
  int checks;
};

struct bool_result {
  bool success;
  int checks;
};

struct update_result {
  bool delete_success;
  bool insert_success;
  int checks;
};

class QuadTree {
 private:
  const int capacity = 8;
  int level = 1;
  AABB bounds;
  std::array<QuadTree*, 4> children{};  // Order: NW, NE, SW, SE
  std::vector<AABB> objects;
  bool inrange(AABB obj);

 public:
  QuadTree(float x, float y, int w, int h);
  ~QuadTree();
  void Destroy();
  bool_result Insert(AABB obj);
  bool_result Delete(AABB obj);
  update_result Update(AABB old, AABB new_);
  void Split();
  search_result Search(AABB range);
};
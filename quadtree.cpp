#include "quadtree.h"

QuadTree::QuadTree(float x, float y, int w, int h) {
  // set bounds
  bounds = AABB{uid(), x, y, w, h};
  // init to null
  for (int i = 0; i < 4; ++i) {
    children[i] = nullptr;
  }
}

QuadTree::~QuadTree() {
  if (!(children[0] == nullptr)) {
    for (int i = 0; i < 4; ++i) {
      children[i]->Destroy();
    }
    Destroy();
  }
}

void QuadTree::Destroy() {
  // check if children exist
  if (children[0] != nullptr) {
    for (int i = 0; i < 4; ++i) {
      // free
      delete children[i];
      // set it back to nullptr
      children[i] = nullptr;
    }
  }
}

bool QuadTree::inrange(AABB obj) { return intersect(bounds, obj); }

bool_result QuadTree::Insert(AABB obj) {
  bool_result result = {false, 0};
  // If not in range don't insert
  if (!inrange(obj)) {
    return result;
  }
  // Only insert if capacity isn't reached
  if (objects.size() < capacity) {
    objects.push_back(obj);
    result.checks++;
    result.success = true;
    return result;
  }
  // Init if no children
  if ((children[0] == nullptr)) {
    Split();
  }
  // Insert into atleast one child
  for (int i = 0; i < 4; i++) {
    bool_result child_result = children[i]->Insert(obj);
    if (child_result.success) {
      result.checks += child_result.checks;
      result.success = true;
      return result;
    }
    result.checks++;
  }

  return result;
}

bool_result QuadTree::Delete(AABB obj) {
  bool_result result = {false, 0};
  // If not in range don't bother
  if (!inrange(obj)) {
    return result;
  }

  // Check at levels
  if (!objects.empty()) {
    int i = 0;
    result.checks = 8;
    for (AABB& lvl_obj : objects) {
      // find same id
      if (lvl_obj.id == obj.id) {
        objects.erase(objects.begin() + i);
        result.success = true;
        return result;
      }
      i++;
    }
  }

  // If it does have children
  if (!(children[0] == nullptr)) {
    for (int i = 0; i < 4; i++) {
      bool_result child_result = children[i]->Delete(obj);
      result.checks += child_result.checks;
      if (child_result.success) {
        result.success = true;
        return result;
      }
    }
  }
  return result;
}

update_result QuadTree::Update(AABB old, AABB new_) {
  // Delete old entry
  bool_result delete_result = Delete(old);
  // Insert new entry
  bool_result insert_result = Insert(new_);

  return update_result{delete_result.success, insert_result.success,
                       delete_result.checks + insert_result.checks};
}

void QuadTree::Split() {
  // Increase level
  level++;
  // sub width and height
  int subw = bounds.w / level;
  int subh = bounds.h / level;
  // Init children
  children[NW] = new QuadTree(bounds.x, bounds.y, subw, subh);
  children[NE] = new QuadTree(bounds.x, bounds.y + subh, subw, subh);
  children[SW] = new QuadTree(bounds.x + subw, bounds.y, subw, subh);
  children[SE] = new QuadTree(bounds.x + subw, bounds.y + subh, subw, subh);
}

search_result QuadTree::Search(AABB range) {
  search_result result;
  result.checks = 0;
  // Terminate if not in range
  if (!inrange(range)) {
    return result;
  }

  // Check at current level
  for (AABB obj : objects) {
    if (intersect(range, obj)) {
      result.neighbours.push_back(obj);
    }
    result.checks++;
  }
  // Terminate if no children
  if ((children[0] == nullptr)) {
    return result;
  }

  for (int j = 0; j < 4; j++) {
    // Query children for neighbours
    search_result child_result = children[j]->Search(range);
    // Add neighbours it gives to our array
    for (AABB n : child_result.neighbours) {
      result.neighbours.push_back(n);
    }
    result.checks += child_result.checks;
  }

  return result;
}
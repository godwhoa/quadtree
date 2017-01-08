#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <quadtree.hpp>
#include <aabb.hpp>

void print_aabb(AABB obj) {
  printf("x: %f y:%f w: %d h: %d\n", obj.x, obj.y, obj.w, obj.h);
}

AABB rand_aabb(int ww, int wh, int w, int h) {
  return AABB{uid(), (float)(rand() % ww), (float)(rand() % wh), rand() % 50,
              rand() % 50};
}

int main(int argc, char const* argv[]) {
  srand(time(NULL));
  long long int N = 1000;
  QuadTree tree(0.0, 0.0, N / 2, N / 2);

  // Test insert
  double insert_avg;
  int insert_cavg;
  for (int i = 0; i < N; ++i) {
    AABB r_aabb = rand_aabb(N / 2, N / 2, 50, 50);

    clock_t start = clock();  // stopwatch start
    bool_result insert_r = tree.Insert(r_aabb);
    clock_t stop = clock();  // stopwatch stop

    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;  // conv.
    insert_avg += elapsed;
    insert_cavg += insert_r.checks;
  }

  // inital data
  AABB m_AABB = AABB{uid(), 0, 0, 20, 30};
  tree.Insert(m_AABB);

  // Test update
  double update_avg = 0.0;
  int update_cavg = 0;
  AABB n_AABB;
  for (int i = 0; i < N; ++i) {
    n_AABB = rand_aabb(N / 2, N / 2, N / 50, N / 10);
    n_AABB.id = m_AABB.id;

    clock_t start = clock();  // stopwatch start
    update_result update_r = tree.Update(m_AABB, n_AABB);
    clock_t stop = clock();  // stopwatch stop
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;  // conv.

    update_avg += elapsed;
    update_cavg += update_r.checks;
  }

  // Test delete
  double delete_avg = 0.0;
  int delete_cavg = 0;
  for (int i = 0; i < N; ++i) {
    AABB r_aabb = rand_aabb(N / 2, N / 2, 50, 50);
    bool_result insert_r = tree.Insert(r_aabb);

    clock_t start = clock();  // stopwatch start
    bool_result delete_r = tree.Delete(r_aabb);
    clock_t stop = clock();  // stopwatch stop
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;  // conv.

    delete_avg += elapsed;
    delete_cavg += delete_r.checks;
  }

  // Test Search
  double search_avg = 0.0;
  int search_cavg = 0;
  search_result search_r;
  for (int i = 0; i < N; ++i) {
    AABB r_aabb = rand_aabb(N / 2, N / 2, 50, 50);
    bool_result insert_r = tree.Insert(r_aabb);

    clock_t start = clock();  // stopwatch start
    search_r = tree.Search(r_aabb);
    clock_t stop = clock();  // stopwatch stop
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;  // conv.

    bool_result delete_r = tree.Delete(r_aabb);

    search_avg += elapsed;
    search_cavg += search_r.checks;
  }

  // search_result search_r = tree.Search(AABB{uid(), 0, 0, 100, 100});

  printf("\n\n\nAll objects in range: x:%f y:%f w: %d, h:%d\n", 0.0, 0.0, 100,
         100);
  for (auto& n : search_r.neighbours) {
    print_aabb(n);
  }
  printf("\nNumber of objects: %d\n", N);
  printf("Avg. insert time and checks: %fms %dchecks\n", insert_avg / N,
         insert_cavg / N);
  printf("Avg. delete time and checks: %fms %dchecks\n", delete_avg / N,
         delete_cavg / N);
  printf("Avg. update time and checks: %fms %dchecks\n", update_avg / N,
         update_cavg / N);
  printf("Avg. search time and checks: %fms %dchecks\n", search_avg / N,
         search_cavg / N);

  printf("update+search for %d objs:%fms\n", N,
         ((update_avg / N) + (search_avg / N)) * N);

  printf("Quadtree search took %d checks\n", search_r.checks);
  printf("Brute-force(n*(n-1)) would be %d checks\n", N * (N - 1));
  return 0;
}
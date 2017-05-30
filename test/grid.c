/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */

#include <cute.h>
#include <string.h>

#include "hexc.h"

CUTEST_DATA {
  hexc_cell_t grid[HEXC_GSIZE][HEXC_GSIZE];
};

CUTEST_SETUP {
  hexc_grid_ctor(self->grid);
}
CUTEST_TEARDOWN {}

CUTEST(grid, neighbor) {
  hexc_cell_t *cells[6];
  unsigned count;

  hexc_grid_neighbor_cells(self->grid, 5, 5, cells, &count);
  ASSERT(memcmp(&self->grid[4][5], cells[0], sizeof(hexc_cell_t)) == 0);
  ASSERT(memcmp(&self->grid[4][6], cells[1], sizeof(hexc_cell_t)) == 0);
  ASSERT(memcmp(&self->grid[5][6], cells[2], sizeof(hexc_cell_t)) == 0);
  ASSERT(memcmp(&self->grid[5][4], cells[3], sizeof(hexc_cell_t)) == 0);
  ASSERT(memcmp(&self->grid[6][5], cells[4], sizeof(hexc_cell_t)) == 0);
  ASSERT(memcmp(&self->grid[6][4], cells[5], sizeof(hexc_cell_t)) == 0);
  return EXIT_SUCCESS;
}

int main() {
  CUTEST_DATA test = {0};

  CUTEST_PASS(grid, neighbor);
  return EXIT_SUCCESS;
}

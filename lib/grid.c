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

#include "hexc.h"

void hexc_grid_init(hexc_cell_t grid[14][14]) {
  int i, j;

  for (i = 0; i<14; ++i) {
    for (j = 0; j<14; ++j) {
      grid[i][j].color = HEXC_COLOR_WHITE;
      grid[i][j].x = i;
      grid[i][j].y = j;
    }
  }
}

bool hexc_grid_has_winner(hexc_cell_t grid[14][14], hexc_player_t *out) {
  return false;
}

void hexc_grid_neighbor_cells(hexc_cell_t grid[14][14], int x, int y, hexc_cell_t cells[6], unsigned *count) {
#define ADD_COUPLE(i, j) do if ((i)>=0&&(i)<14&&(j)>=0&&(j)<14) *(cells + (++*count, *count-1)) = grid[i][j]; while (0)
  *count = 0;
  ADD_COUPLE(x-1, y);
  ADD_COUPLE(x-1, y+1);
  ADD_COUPLE(x, y+1);
  ADD_COUPLE(x, y-1);
  ADD_COUPLE(x+1, y);
  ADD_COUPLE(x+1, y-1);
#undef ADD_COUPLE
}

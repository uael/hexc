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

#include <string.h>
#include "hexc.h"

void hexc_grid_ctor(hexc_cell_t grid[14][14]) {
  int i, j;

  for (i = 0; i<14; ++i) {
    for (j = 0; j<14; ++j) {
      grid[i][j].color = HEXC_COLOR_WHITE;
      grid[i][j].x = i;
      grid[i][j].y = j;
    }
  }
}

void hexc_grid_reset(hexc_cell_t grid[14][14]) {
  int i, j;

  for (i = 0; i<14; ++i) {
    for (j = 0; j<14; ++j) {
      grid[i][j].past = false;
    }
  }
}

void hexc_grid_print(hexc_cell_t grid[14][14], FILE *stream) {
  int i, j, k;
  hexc_cell_t cell;

  fputs(" / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\\n", stream);
  for (i = 0; i<14; ++i) {
    for (k = 0; k < (2*i); ++k) {
      fputc(' ', stream);
    }
    for (j = 0; j<14; ++j) {
      cell = grid[i][j];
      fputs("| ", stream);
      switch (cell.color) {
        case HEXC_COLOR_BLUE:
          fputc('O', stream);
          break;
        case HEXC_COLOR_RED:
          fputc('X', stream);
          break;
        default:
          fputc(' ', stream);
          break;
      }
      fputc(' ', stream);
    }
    fputs("|\n", stream);
    for (k = 0; k < (2*i); ++k) {
      fputc(' ', stream);
    }
    fputs(" \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\\n", stream);
  }
}

bool hexc_grid_search_winner(hexc_cell_t grid[14][14], hexc_cell_t *neighbor_cells[6], int neighbor_cells_n) {
  bool victory = false;
  hexc_cell_t *cell;
  hexc_cell_t *next, *cells[6];
  int i, j, count;

  if (neighbor_cells_n) {
    for (i = 0; i<neighbor_cells_n; ++i) {
      cell = neighbor_cells[i];
      hexc_grid_neighbor_cells(grid, cell->x, cell->y, cells, (unsigned int *) &count);
      for (j = 0; j<count; ++j) {
        next = cells[j];
        if (next->color == HEXC_COLOR_BLUE && next->x >= 14) {
          victory = true;
        }
        if (next->color == HEXC_COLOR_RED && next->y >= 14) {
          victory = true;
        }
      }
      if (!victory) {
        hexc_cell_t *nexts[6];
        memcpy(nexts, cells, sizeof(hexc_cell_t *) * 6);
        victory = hexc_grid_search_winner(grid, nexts, count);
      }
    }
  }
  return victory;
}

bool hexc_grid_search_victory(hexc_cell_t grid[14][14], int x, int y) {
  bool victory;
  hexc_cell_t *cell, *cells[6];
  int count;

  hexc_grid_reset(grid);
  cell = &grid[x][y];
  cell->past = true;
  hexc_grid_neighbor_cells(grid, cell->x, cell->y, cells, (unsigned int *) &count);
  victory = hexc_grid_search_winner(grid, cells, count);
  return victory;
}

void hexc_grid_neighbor_cells(hexc_cell_t grid[14][14], int x, int y, hexc_cell_t *cells[6], unsigned *count) {
  hexc_cell_t cell;

  cell = grid[x][y];
#define ADD_COUPLE(i, j) \
  do if ((i)>=0&&(i)<14&&(j)>=0&&(j)<14&&!grid[i][j].past&&grid[i][j].color==cell.color) \
    *(cells + (++*count, *count-1)) = (grid[i][j].past = true, &grid[i][j]); \
  while (0)

  *count = 0;
  ADD_COUPLE(x-1, y);
  ADD_COUPLE(x-1, y+1);
  ADD_COUPLE(x, y+1);
  ADD_COUPLE(x, y-1);
  ADD_COUPLE(x+1, y);
  ADD_COUPLE(x+1, y-1);
#undef ADD_COUPLE
}

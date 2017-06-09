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

#define MC_ITS 10000

HEX_MOVE(hex_ai_montecarlo) {
  uint8_t i, p, k, moves, free_nodes_count = board->freecells_c, nodes[free_nodes_count];
  uint16_t j, max_wins, state[HEX_GSIZE];
  hex_cell_t win_pos, free_nodes[free_nodes_count], free_nodes_copy[free_nodes_count];

  hex_board_print(board, stdout);
  memcpy(nodes, board->freecells, free_nodes_count * sizeof(uint8_t));
  for (i = 0; i < free_nodes_count; ++i) {
    uint8_t id = nodes[i], x, y;
    if (player->color) {
      x = (uint8_t) (id % HEX_GSIZE);
      y = (uint8_t) (id / HEX_GSIZE);
    } else {
      x = (uint8_t) (id / HEX_GSIZE);
      y = (uint8_t) (id % HEX_GSIZE);
    }
    free_nodes[i].x = x;
    free_nodes[i].y = y;
  }
  memcpy(free_nodes_copy, free_nodes, free_nodes_count * sizeof(hex_cell_t));
  max_wins = 0;
  win_pos = free_nodes_copy[0];
  moves = (uint8_t) ((free_nodes_count - 2) / 2 + player->color);
  for (p = 0; p < free_nodes_count; ++p) {
    uint16_t wins = 0, possible_wins = MC_ITS;
    hex_cell_t pos = free_nodes_copy[p];

    for (j = 0; j < MC_ITS; ++j) {
      memcpy(state, board->players[player->color].state, HEX_GSIZE * sizeof(uint16_t));
      hex_state_toggle(state, pos.x, pos.y);
      for (k = 0; k < moves; ++k) {
        uint8_t kpos;
        hex_cell_t id, tmp;

        kpos = (uint8_t) ((rand() % (free_nodes_count - 1 - k)) + 1);
        id = free_nodes[kpos];
        tmp = free_nodes[kpos];
        free_nodes[kpos] = free_nodes[free_nodes_count - 1 - k];
        free_nodes[free_nodes_count - 1 - k] = tmp;

        if (pos.v == id.v) {
          ++k;
          continue;
        }
        hex_state_toggle(state, id.x, id.y);
      }
      if (hex_state_win(state)) {
        ++wins;
      } else {
        possible_wins--;
      }
      if (possible_wins < max_wins) {
        goto end_loop;
      }
    }
    if (wins > max_wins) {
      win_pos = pos;
      max_wins = wins;
    }
    end_loop:;
  }
  if (player->color)
    return (hex_cell_t) {win_pos.y, win_pos.x};
  return (hex_cell_t) {win_pos.x, win_pos.y};
}

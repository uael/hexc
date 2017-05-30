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

#include <stdlib.h>
#include "hexc.h"

hexc_play_t hexc_ai_montecarlo(struct hexc_player *self, hexc_game_t *game) {
  uint32_t free_nodes_count = game->freecells_c;
  hexc_play_t free_nodes[free_nodes_count];
  hexc_play_t free_nodes_copy[free_nodes_count];

  uint16_t nodes[free_nodes_count];
  memcpy(nodes, game->freecells, free_nodes_count * sizeof(uint16_t));

  for (uint16_t i = 0; i < free_nodes_count; ++i) {
    uint32_t id = nodes[i];
    uint8_t row, col;
    if (self->color != 0u) {
      row = (uint8_t) (id % HEXC_GSIZE);
      col = (uint8_t) (id / HEXC_GSIZE);
    } else {
      row = (uint8_t) (id / HEXC_GSIZE);
      col = (uint8_t) (id % HEXC_GSIZE);
    }
    free_nodes[i].x = row;
    free_nodes[i].y = col;
  }

  memcpy(free_nodes_copy, free_nodes, free_nodes_count * sizeof(hexc_play_t));

  uint16_t b[HEXC_FSIZE];
  uint32_t max_wins = 0;
  hexc_play_t win_pos = free_nodes_copy[0];
  uint32_t moves = (free_nodes_count - 2) / 2 + self->color;

  for (uint32_t p = 0; p < free_nodes_count; ++p) {
    uint32_t wins = 0;
    uint32_t possible_wins = 2000;
    hexc_play_t pos = free_nodes_copy[p];

    for (uint32_t j = 0; j < 2000; ++j) {
      memcpy(b, game->players[self->color].state, HEXC_FSIZE * sizeof(uint16_t));
      b[pos.x] |= 1 << pos.y;

      for (uint32_t k = 0; k < moves; ++k) {
        uint32_t kpos = rand() % (((free_nodes_count - 1) - k)) + 1;;
        hexc_play_t id = free_nodes[kpos];
        hexc_play_t tmp = free_nodes[kpos];
        free_nodes[kpos] = free_nodes[(free_nodes_count - 1) - k];
        free_nodes[(free_nodes_count - 1) - k] = tmp;

        if (pos.v == id.v) {
          k++;
          continue;
        }

        b[id.x] |= 1 << id.y;
      }
      if (hexc_state_win(b)) {
        wins++;
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

  if (self->color != 0u)
    return (hexc_play_t) {win_pos.y, win_pos.x};

  return (hexc_play_t) {win_pos.x, win_pos.y};
}

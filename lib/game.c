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

void hexc_game_ctor(hexc_game_t *self, hexc_player_t *red, hexc_player_t *blue) {
  *self = (hexc_game_t) {
    .players = {
      *red, *blue
    }
  };
  hexc_grid_ctor(self->grid);
}

void hexc_game_dtor(hexc_game_t *self) {

}

bool hexc_game_console(hexc_game_t *self) {
  hexc_color_t color = HEXC_COLOR_RED;
  hexc_ai_t *ai;
  hexc_player_t *player;
  bool victory = false;
  hexc_play_t play;

  while (!victory) {
    player = &self->players[color];
    if (player->is_ai) {
      ai = (hexc_ai_t *) player;
      play = ai->play(ai, self);
    } else {
      hexc_grid_print(self->grid, stdout);
      printf("player %s, where to play ? <x> <y> :", hexc_color_tostring(color));
      scanf("%d %d", &play.x, &play.y);
    }
    victory = hexc_game_play(self, color, play.x, play.y);
    color = (hexc_color_t) !color;
  }
  return true;
}

bool hexc_game_play(hexc_game_t *self, hexc_color_t color, int x, int y) {
  hexc_cell_t *cell;

  cell = &self->grid[x][y];
  if (cell->color == HEXC_COLOR_WHITE) {
    cell->color = color;
    switch (color) {
      case HEXC_COLOR_BLUE:
        return hexc_grid_search_victory(self->grid, 0, 1);
      case HEXC_COLOR_RED:
        return hexc_grid_search_victory(self->grid, 1, 0);
      default:
        break;
    }
  }

  return false;
}

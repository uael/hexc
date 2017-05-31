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

void hex_game_ctor(hex_game_t *self, hex_move_t X, hex_move_t O) {
  hex_player_t first = {0}, second = {0};

  *self = (hex_game_t) {
    .current_color = HEXC_COLOR_X
  };
  hex_player_ctor(&first, HEXC_COLOR_X, X);
  hex_player_ctor(&second, HEXC_COLOR_O, O);
  hex_board_ctor(&self->board, &first, &second);
}

void hex_game_reset(hex_game_t *self) {
  self->current_color = HEXC_COLOR_X;
  hex_board_reset(&self->board);
}

bool hex_game_win(hex_game_t *self) {
  return hex_state_win(self->board.players[self->current_color].state);
}

bool hex_game_console(hex_game_t *self) {
  hex_player_t player;
  hex_cell_t cell;

  do {
    player = self->board.players[self->current_color];
    cell = player.move(&player, &self->board);
  } while(!hex_game_play(self, cell));
  return true;
}

bool hex_game_play(hex_game_t *self, hex_cell_t cell) {
  uint8_t pos;
  bool victory = false;

  pos = (uint8_t) (cell.x * HEX_GSIZE + cell.y);
  if (hex_board_is_toggled(&self->board, pos)) {
    fputs("This position is already occupied\n", stderr);
  } else {
    hex_board_toggle(&self->board, pos, self->current_color);
    victory = hex_game_win(self);
  }
  self->current_color ^= 1;

  return victory;
}

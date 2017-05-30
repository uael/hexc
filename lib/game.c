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
  hexc_game_reset(self);
}

void hexc_game_dtor(hexc_game_t *self) {

}

void hexc_game_reset(hexc_game_t *self) {
  uint16_t i;

  hexc_state_reset(self->players[0].state);
  hexc_state_reset(self->players[1].state);
  for (i = 0; i < HEXC_FSIZE; ++i) self->freecells_idx[i] = self->freecells[i] = i;
  self->freecells_c = HEXC_FSIZE;
}

bool hexc_game_is_toggled(const hexc_game_t *self, uint32_t id) {
  uint32_t row = id / HEXC_GSIZE;
  uint32_t col = id % HEXC_GSIZE;

  return (bool) (((self->players[0].state[row] >> col) & 1) | ((self->players[1].state[col] >> row) & 1));
}

void hexc_game_toggle(hexc_game_t *self, uint32_t id, hexc_color_t color) {
  if (!hexc_game_is_toggled(self, id)) {
    uint16_t __tmp;
    int __row, __col, __id;
    if (color != 0u) {
      __row = id % HEXC_GSIZE;
      __col = id / HEXC_GSIZE;
    }
    else {
      __row = id / HEXC_GSIZE;
      __col = id % HEXC_GSIZE;
    }
    self->players[color].state[__row] |= 1 << __col;
    __id = self->freecells_idx[id];
    __tmp = self->freecells[__id];
    self->freecells[__id] = self->freecells[--self->freecells_c];
    self->freecells[self->freecells_c] = __tmp;
    __tmp = self->freecells_idx[__id];
    self->freecells_idx[__id] = self->freecells_idx[self->freecells_c];
    self->freecells_idx[self->freecells_c] = __tmp;
  }
}

bool hexc_is_end_game(hexc_game_t *self, hexc_color_t color) {
  return hexc_player_win(&self->players[color]);
}

bool hexc_game_console(hexc_game_t *self) {
  hexc_color_t color = HEXC_COLOR_RED;
  hexc_player_t *player;
  bool victory = false;
  hexc_play_t play;

  while (!victory) {
    player = &self->players[color];
    play = player->play(player, self);
    victory = hexc_game_play(self, color, play.x, play.y);
    color = (hexc_color_t) !color;
  }
  return true;
}

bool hexc_game_play(hexc_game_t *self, hexc_color_t color, int x, int y) {
  uint32_t pos;

  pos = (uint32_t) (x * HEXC_GSIZE + y);
  if (hexc_game_is_toggled(self, pos)) {
    fputs("This position is already occupied\n", stderr);
  } else {
    hexc_game_toggle(self, pos, color);
    return hexc_is_end_game(self, color);
  }

  return false;
}

void hexc_game_print(hexc_game_t b, FILE *stream) {
  fputc(' ', stream);
  for (uint32_t i = 0; i < HEXC_GSIZE; ++i) {
    fprintf(stream, " %c ", 'A' + i);
  }
  fputc('\n', stream);

  for (uint32_t i = 0; i < HEXC_GSIZE; ++i) {
    for (unsigned s = 0; s < i; ++s) {
      fputs("  ", stream);
    }
    fprintf(stream, "%d", i + 1);
    for (unsigned j = 0; j < HEXC_GSIZE; ++j) {
      fputc(' ', stream);
      if (b.players[0].state[i] >> j & 1) {
        fputc('X', stream);
      } else if (b.players[1].state[j] >> i & 1) {
        fputc('O', stream);
      } else {
        fputc('.', stream);
      }
      fputc(' ', stream);
    }
    fprintf(stream, "%d", i + 1);
    fputc('\n', stream);
  }

  fputc(' ', stream);
  for (int s = 0; s < HEXC_GSIZE; ++s) {
    fputs("  ", stream);
  }
  for (int i = 0; i < HEXC_GSIZE; ++i) {
    fprintf(stream, " %c ", 'A' + i);
  }
  fputc('\n', stream);
}

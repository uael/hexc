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

void hex_board_ctor(hex_board_t *self, hex_player_t *red, hex_player_t *blue) {
  *self = (hex_board_t) {
    .players = {
      *red, *blue
    }
  };
  hex_board_reset(self);
}

void hex_board_reset(hex_board_t *self) {
  uint8_t i;

  hex_state_reset(self->players[0].state);
  hex_state_reset(self->players[1].state);
  for (i = 0; i < HEX_FSIZE; ++i) self->freecells_idx[i] = self->freecells[i] = i;
  self->freecells_c = HEX_FSIZE;
}

void hex_board_cpy(hex_board_t *dest, hex_board_t *src) {
  *dest = (hex_board_t) {
    .freecells_c = src->freecells_c
  };
  memcpy(dest->freecells, src->freecells, src->freecells_c * sizeof(uint8_t));
  memcpy(dest->freecells_idx, src->freecells_idx, HEX_FSIZE * sizeof(uint8_t));
}

bool hex_board_is_toggled(const hex_board_t *self, uint8_t idx) {
  uint8_t row = (uint8_t) (idx / HEX_GSIZE);
  uint8_t col = (uint8_t) (idx % HEX_GSIZE);

  return (bool) (((self->players[0].state[row] >> col) & 1) | ((self->players[1].state[col] >> row) & 1));
}

void hex_board_toggle(hex_board_t *self, uint8_t idx, hex_color_t color) {
  if (!hex_board_is_toggled(self, idx)) {
    uint8_t tmp;
    unsigned x, y, i;

    if (color) {
      x = idx % HEX_GSIZE;
      y = idx / HEX_GSIZE;
    }
    else {
      x = idx / HEX_GSIZE;
      y = idx % HEX_GSIZE;
    }
    self->players[color].state[x] |= 1 << y;
    i = self->freecells_idx[idx];
    tmp = self->freecells[i];
    self->freecells[i] = self->freecells[--self->freecells_c];
    self->freecells[self->freecells_c] = tmp;
    tmp = self->freecells_idx[i];
    self->freecells_idx[i] = self->freecells_idx[self->freecells_c];
    self->freecells_idx[self->freecells_c] = tmp;
  }
}

void hex_board_print(hex_board_t *self, FILE *stream) {
  uint8_t i, j, s;

  for (i = 0; i < HEX_GSIZE; ++i) {
    fprintf(stream, " %c ", 'A' + i);
  }
  fputc('\n', stream);
  for (i = 0; i < HEX_GSIZE; ++i) {
    for (s = 0; s < i; ++s) {
      fputs("  ", stream);
    }
    fprintf(stream, "%d", i + 1);
    for (j = 0; j < HEX_GSIZE; ++j) {
      fputc(' ', stream);
      if (self->players[0].state[i] >> j & 1) {
        fputc('X', stream);
      } else if (self->players[1].state[j] >> i & 1) {
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
  for (s = 0; s < HEX_GSIZE; ++s) {
    fputs("  ", stream);
  }
  for (i = 0; i < HEX_GSIZE; ++i) {
    fprintf(stream, " %c ", 'A' + i);
  }
  fputc('\n', stream);
}

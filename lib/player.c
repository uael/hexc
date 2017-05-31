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

void hex_player_ctor(hex_player_t *self, hex_color_t color, hex_cell_t (*move)(hex_player_t*, hex_board_t*)) {
  *self = (hex_player_t) {
    .color = color,
    .move = move
  };
}

hex_cell_t hex_realplayer(hex_player_t *player, hex_board_t *game) {
  char *line;
  size_t line_size;

  hex_board_print(game, stdout);
  printf("player %s, where to play ?", hex_color_tostring(player->color));

  while (true) {
    getline(&line, &line_size, stdin);
    if (line_size >= 2) {
      int32_t row, col;
      col = line[0] - 'A';
      row = atoi(line + 1) - 1;
      if (col >= 0 && (uint32_t) col < HEX_GSIZE && row >= 0 && (uint32_t) row < HEX_GSIZE) {
        return (hex_cell_t) {(uint8_t) row, (uint8_t) col};
      }
    }
    fputs("Invalid position\n", stderr);
  }
}

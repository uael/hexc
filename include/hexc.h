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

#ifndef  HEXC_H__
# define HEXC_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HEX_GSIZE 9U
#define HEX_FSIZE HEX_GSIZE*HEX_GSIZE

#define HEX_MOVE(name) hex_cell_t name(hex_player_t *player, hex_board_t *board)

#ifndef bool
# define bool unsigned char
# define true  1U
# define false 0U
#endif

#define hex_state_reset(state) memset(state, 0, HEX_FSIZE * sizeof(uint16_t))

typedef const char *string_t;
typedef char hex_color_t;
typedef struct hex_player hex_player_t;
typedef union hex_cell hex_cell_t;
typedef struct hex_board hex_board_t;
typedef struct hex_game hex_game_t;
typedef hex_cell_t (*hex_move_t)(hex_player_t*, hex_board_t*);

union hex_cell {
  struct {
    uint8_t x;
    uint8_t y;
  };
  uint16_t v;
};

extern bool hex_state_win(uint16_t player_state[HEX_FSIZE]);
extern bool hex_state_is_toggled(uint16_t player_state[HEX_FSIZE], uint8_t idx);
extern bool hex_state_is_toggled_by(uint16_t player_state[HEX_FSIZE], uint8_t idx, hex_color_t color);
extern void hex_state_toggle(uint16_t player_state[HEX_FSIZE], uint8_t row, uint8_t col);
extern void hex_state_toggle_by(uint16_t player_state[HEX_FSIZE], uint8_t idx, hex_color_t color);

  enum hex_color {
  HEXC_COLOR_X = 0,
  HEXC_COLOR_O = 1
};

extern string_t hex_color_tostring(hex_color_t color);

struct hex_player {
  hex_color_t color;
  uint16_t state[HEX_FSIZE];
  hex_move_t move;
};

extern void hex_player_ctor(hex_player_t *self, hex_color_t color, hex_move_t move);

struct hex_board {
  uint8_t freecells_c, freecells_idx[HEX_FSIZE];
  uint8_t freecells[HEX_FSIZE];
  hex_player_t players[2];
};

extern void hex_board_ctor(hex_board_t *self, hex_player_t *red, hex_player_t *blue);
extern void hex_board_reset(hex_board_t *self);
extern void hex_board_cpy(hex_board_t *dest, hex_board_t *src);
extern bool hex_board_is_toggled(const hex_board_t *self, uint8_t idx);
extern void hex_board_toggle(hex_board_t *self, uint8_t idx, hex_color_t color);
extern void hex_board_print(hex_board_t *self, FILE *stream);

struct hex_game {
  hex_board_t board;
  hex_color_t current_color;
};

extern void hex_game_ctor(hex_game_t *self, hex_move_t X, hex_move_t O);
extern void hex_game_reset(hex_game_t *self);
extern bool hex_game_win(hex_game_t *self);
extern bool hex_game_console(hex_game_t *self);
extern bool hex_game_play(hex_game_t *self, hex_cell_t cell);

extern HEX_MOVE(hex_realplayer);
extern HEX_MOVE(hex_ai_montecarlo);

#endif /* HEXC_H__ */

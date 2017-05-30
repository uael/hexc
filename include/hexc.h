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

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define HEXC_GSIZE 9
#define HEXC_FSIZE HEXC_GSIZE*HEXC_GSIZE

#ifndef bool
# define bool unsigned char
# define true  1UL
# define false 0UL
#endif

#define hexc_state_reset(state) memset(state, 0, HEXC_GSIZE * sizeof(uint16_t))

typedef const char *string_t;
typedef char hexc_color_t;
typedef struct hexc_player hexc_player_t;
typedef union hexc_play hexc_play_t;
typedef struct hexc_game hexc_game_t;

extern bool hexc_state_win(uint16_t player_state[HEXC_FSIZE]);

enum hexc_color {
  HEXC_COLOR_WHITE = -1,
  HEXC_COLOR_RED = 0,
  HEXC_COLOR_BLUE = 1
};

extern string_t hexc_color_tostring(hexc_color_t color);

struct hexc_player {
  string_t name;
  hexc_color_t color;
  uint16_t state[HEXC_FSIZE];

  hexc_play_t (*play)(struct hexc_player*, hexc_game_t*);
};

extern void hexc_player_ctor(hexc_player_t *self, string_t name, hexc_color_t color, hexc_play_t (*play)(struct hexc_player*, hexc_game_t*));
extern void hexc_player_dtor(hexc_player_t *self);
extern bool hexc_player_win(hexc_player_t *self);

union hexc_play {
  struct {
    uint8_t x;
    uint8_t y;
  };
  uint16_t v;
};

struct hexc_game {
  uint16_t freecells_c, freecells[HEXC_FSIZE], freecells_idx[HEXC_FSIZE];
  hexc_player_t players[2];
};

extern void hexc_game_ctor(hexc_game_t *self, hexc_player_t *red, hexc_player_t *blue);
extern void hexc_game_dtor(hexc_game_t *self);
extern void hexc_game_reset(hexc_game_t *self);
extern bool hexc_game_is_toggled(const hexc_game_t *self, uint32_t id);
extern void hexc_game_toggle(hexc_game_t *self, uint32_t id, hexc_color_t color);
extern bool hexc_is_end_game(hexc_game_t *self, hexc_color_t color);
extern bool hexc_game_console(hexc_game_t *self);
extern bool hexc_game_play(hexc_game_t *self, hexc_color_t color, int x, int y);
extern void hexc_game_print(hexc_game_t b, FILE *stream);

extern hexc_play_t hexc_realplayer(struct hexc_player *self, hexc_game_t *game);
extern hexc_play_t hexc_ai_montecarlo(struct hexc_player *self, hexc_game_t *game);

#endif /* HEXC_H__ */

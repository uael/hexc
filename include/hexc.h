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

#ifndef bool
# define bool unsigned char
# define true  1UL
# define false 0UL
#endif

typedef const char *string_t;
typedef unsigned char hexc_color_t;
typedef struct hexc_cell hexc_cell_t;
typedef struct hexc_player hexc_player_t;
typedef struct hexc_ai hexc_ai_t;
typedef struct hexc_game hexc_game_t;

enum hexc_color {
  HEXC_COLOR_WHITE = -1,
  HEXC_COLOR_RED = 0,
  HEXC_COLOR_BLUE = 1
};

struct hexc_cell {
  int x, y;
  hexc_color_t color;
  bool past;
};

extern void hexc_grid_init(hexc_cell_t grid[14][14]);
extern void hexc_grid_reset(hexc_cell_t grid[14][14]);
extern bool hexc_grid_has_winner(hexc_cell_t grid[14][14], hexc_player_t *out);
extern void hexc_grid_neighbor_cells(hexc_cell_t grid[14][14], int x, int y, hexc_cell_t *cells[6], unsigned *count);

struct hexc_player {
  string_t name;
  hexc_color_t color;
  bool is_ai;
};

extern void hexc_player_ctor(hexc_player_t *self, string_t name, hexc_color_t color);
extern void hexc_player_dtor(hexc_player_t *self);

struct hexc_ai {
  string_t name;
  hexc_color_t color;
  bool is_ai;
  hexc_cell_t vue[14][14];

  void (*play)(struct hexc_ai *self, hexc_game_t *game);
};

extern void hexc_ai_ctor(hexc_ai_t *self, string_t name, hexc_color_t color, void (*play)(hexc_ai_t *, hexc_game_t *));
extern void hexc_ai_dtor(hexc_ai_t *self);

struct hexc_game {
  hexc_cell_t grid[14][14];
  hexc_player_t players[2];
};

extern void hexc_game_ctor(hexc_game_t *self, hexc_player_t *red, hexc_player_t *blue);
extern void hexc_game_dtor(hexc_game_t *self);
extern bool hexc_game_play(hexc_game_t *self);

#endif /* HEXC_H__ */

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

bool hex_state_win(uint16_t player_state[HEX_FSIZE]) {
  enum State {
    LinkDown,
    LinkUp,
    Capture
  };

  uint16_t board[HEX_FSIZE] = {0}, shadow_board[HEX_FSIZE] = {0};
  memcpy(board, player_state, HEX_FSIZE * sizeof(uint16_t));

  uint32_t shadow_row = board[0];
  uint32_t row = 0;

  shadow_board[0] = board[0];
  board[0] = 0;

  int state = LinkDown;
  uint32_t row_num = 1;

  uint32_t connections = 0;
  while (true) {
    switch (state) {
      case LinkDown: {
        row = board[row_num];
        connections = (row & shadow_row) | ((shadow_row >> 1) & row);

        if (connections) {
          if (row_num == HEX_GSIZE - 1)
            return true;

          state = Capture;
        } else {
          shadow_row = shadow_board[row_num];
          if (shadow_row) {
            row_num++;
          } else {
            return false;
          }
        }
        break;
      }
      case LinkUp: {
        row = board[row_num];
        connections = (row & shadow_row) | ((shadow_row << 1) & row);
        if (connections) {
          state = Capture;
        } else {
          shadow_row = shadow_board[row_num];
          row_num++;
          state = LinkDown;
        }
        break;
      }
      case Capture: {
        uint32_t capture = 0;
        capture = connections;

        board[row_num] ^= capture;
        shadow_board[row_num] |= capture;

        // capture right bits
        do {
          capture >>= 1;
          capture &= board[row_num];
          board[row_num] ^= capture;
          shadow_board[row_num] |= capture;
        } while (capture);

        // capture left bits
        capture = connections;
        do {
          capture <<= 1;
          capture &= board[row_num];
          board[row_num] ^= capture;
          shadow_board[row_num] |= capture;
        } while (capture);

        shadow_row = shadow_board[row_num];

        if (board[row_num - 1]) {
          row_num--;
          state = LinkUp;
        } else {
          row_num++;
          state = LinkDown;
        }
        break;
      }
    }
  }
}

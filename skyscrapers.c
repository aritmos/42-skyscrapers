#include <stdint.h>
#include <unistd.h>

#include "internals.h"

void col_to_line(t_board board, uint8_t col, t_line line) {
  uint8_t r = 0;
  while (r <= 3) {
    line[r] = board[4 * r + col];
  }
}

void print_board(t_board board) {
  uint8_t pos = 0;
  while (pos < 16) {
    char c = (char)board[pos] + '0';
    write(1, &c, 1);
    if (pos && pos % 4 == 0) {
      c = '\n';
      write(1, &c, 1);
    }
    pos++;
  }
}

// Writes the hint to the given pointer.
// SAFETY: Caller guarantees `pos` is valid: `pos < 16`.
// SAFETY: This function does VERY unsafe pointer arithmetic writes :P
void write_hint(t_board_hints *hints, uint8_t pos, uint8_t num_buildings) {
  // Selects if we are writing to col hints or row hints
  uint8_t line_type_offset = (uint8_t)(pos < 8) * 8;
  // Selects the index within the row/col hint array
  uint8_t hint_offset = pos % 4;
  // Selects between the first and second digit of the hint
  uint8_t hint_digit_offset = (pos / 4) % 2;

  uint8_t *write_ptr = (uint8_t *)hints;
  write_ptr[line_type_offset + hint_offset + hint_digit_offset] = num_buildings;
}

// The strsize (includes \0) of the hints argument
const uint8_t HINTS_ARG_LEN = 32;

// Creates a t_board_hints object and stores it in out.
// Returns if it did so successfully.
bool parse_hints(char *hints_str, t_board_hints *out) {
  uint8_t idx = 0;
  while (true) {
    char c = hints_str[idx];
    if (c == 0 || idx > HINTS_ARG_LEN) break;
    if (idx % 2) {
      // Odd index, should be a space
      if (c != ' ') return false;
    } else {
      // Even index, should be a valid digit
      if (c < '1' || '4' < c) return false;
      write_hint(out, idx / 2, c - '0');
    }
    idx++;
  }
  return idx == HINTS_ARG_LEN;
}

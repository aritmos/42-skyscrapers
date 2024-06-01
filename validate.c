#include "internals.h"

// Checks if the given number doesn't already exist in its row (only checks to
// the left)
inline bool is_valid_in_row(uint8_t board[static 16], uint8_t pos) {
  uint8_t col = pos % 4;
  uint8_t dc = 1;
  while (dc <= col) {
    if (board[pos - dc] == board[pos]) return false;
    dc--;
  }
  return true;
}

// Checks if the given number doesn't already exist in its row (only checks
// upwards)
inline bool is_valid_in_col(uint8_t board[static 16], uint8_t pos) {
  uint8_t row = pos / 4;
  uint8_t dr = 1;
  while (dr <= row) {
    if (board[pos - 4 * dr] == board[pos]) return false;
    dr--;
  }
  return true;
}

// Checks if the given row satisfies the puzzles hints
inline bool is_valid_in_row_hint(t_skyscrapers *skyscrapers, uint8_t row) {
  t_hint hint = skyscrapers->hints.row_hints[row];
  // SAFETY: This cast requires `row` to be a valid row index: 0..=3.
  t_line *line_ptr = (t_line *)&skyscrapers->board[4 * row];
  return hint_is_valid(*line_ptr, hint);
}

// Checks if the given column satisfies the puzzles hints
inline bool is_valid_in_col_hint(t_skyscrapers *skyscrapers, uint8_t col) {
  t_hint hint = skyscrapers->hints.col_hints[col];
  t_line line;
  col_to_line(skyscrapers->board, col, line);
  return hint_is_valid(line, hint);
}

bool is_valid_cell(t_skyscrapers *skyscrapers, uint8_t pos) {
  uint8_t row = pos / 4;
  uint8_t col = pos % 4;

  // check for clashes with already assigned cells
  if (!is_valid_in_row(skyscrapers->board, pos)) return false;
  if (!is_valid_in_col(skyscrapers->board, pos)) return false;

  // check for clashes in the hints
  if (col == 3) {
    if (!is_valid_in_row_hint(skyscrapers, row)) return false;
  }
  if (row == 3) {
    if (!is_valid_in_col_hint(skyscrapers, col)) return false;
  }

  return true;
}

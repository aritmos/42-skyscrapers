#pragma once

/*
 * ===============
 * STDLIB INCLUDES
 * ===============
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * =====
 * TYPES
 * =====
 */

// A `hint` is a collection of two numbers given in the puzzle,
// namely the ones on either side of a line of buildings.
//
// The order is [top, bottom] or [left, right]
typedef struct s_hint {
  uint8_t forward;
  uint8_t backward;
} t_hint;

// The collection of hints given in the puzzle,
// grouped into those relating to columns and rows respectively.
typedef struct s_board_hints {
  t_hint col_hints[4];
  t_hint row_hints[4];
} t_board_hints;

// The skyscrapers object contains the working board
// along with the provided hints
typedef struct skyscrapers {
  uint8_t board[16];
  t_board_hints hints;
} t_skyscrapers;

// The 4x4 grid of building heights, stored in a linear array.
typedef uint8_t t_board[16];

// A sequence of 4 building heights.
typedef uint8_t t_line[4];

/*
 * =========
 * FUNCTIONS
 * =========
 */

/*
 * Hints
 */

// Checks if a given line agrees with a hint.
bool hint_is_valid(t_line line, t_hint hint);

/*
 * Skyscrapers
 */

// Parses the given hints (as given by the args) into the given pointer.
// Returns if it was successful.
bool parse_hints(char *hints_str, t_board_hints *hints);

// Prints the board to stdout
void print_board(t_board board);

// Creates a line from the given column of the board
void col_to_line(t_board board, uint8_t col, t_line out);

// Creates the skyscrapers object using hints encoded into a string
// Returns if the creation was successful.
bool create_skyscrapers(char *hints_str, t_skyscrapers *out);

/*
 * Validate
 */

// Checks if a cell is valid in the grid (only looks backwards)
bool is_valid_cell(t_skyscrapers *skyscrapers, uint8_t pos);

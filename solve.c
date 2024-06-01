#include "internals.h"

typedef enum e_result : uint8_t {
  Done,
  ArgErr,
  ParseErr,
  NoSolution,
} t_result;

void backtrack(t_skyscrapers *skyscrapers, uint8_t pos, bool *finished) {
  if (pos == 16) {
    print_board(skyscrapers->board);
    *finished = true;
    return;
  }

  uint8_t n = 1;
  while (n <= 4) {
    skyscrapers->board[pos] = n;
    if (!*finished && is_valid_cell(skyscrapers, pos))
      backtrack(skyscrapers, pos + 1, finished);
    n++;
  }
}

// Solves the skyscrapers puzzle.
// Uses a backtracking algorithm.
t_result solve(char *hints_str) {
  t_skyscrapers skyscrapers;
  bool ok = parse_hints(hints_str, &skyscrapers.hints);
  if (!ok) return ParseErr;

  bool finished = false;
  backtrack(&skyscrapers, 0, &finished);
  return finished ? Done : NoSolution;
}

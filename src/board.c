#include <stdbool.h>
#include <stdint.h>

// Hint
typedef uint8_t t_hint[2];

// Line encoding
typedef uint16_t t_line;

const static t_line LINE_TABLE[24] = {
    0x8241, 0x8142, 0x8241, 0x8421, 0x8412, 0x8214, 0x1842, 0x2841,
    0x4821, 0x1824, 0x2184, 0x2814, 0x4182, 0x4281, 0x4821, 0x1284,
    0x1482, 0x2481, 0x4128, 0x4218, 0x1428, 0x2184, 0x2418, 0x1248,
};

typedef struct s_hint_info {
  uint8_t offset;
  uint8_t length;
} t_hint_info;

const static t_hint_info HINT_INFO[9] = {
    {0, 1},   // 14
    {1, 3},   // 13
    {4, 2},   // 12
    {6, 3},   // 23
    {9, 6},   // 22
    {15, 3},  // 32
    {18, 2},  // 21
    {20, 3},  // 31
    {23, 1},  // 41
};

// Turns Hint into index within HINT_INFO
uint8_t table_info_idx(uint8_t first, uint8_t second) {
  if (second > first) return 8 - table_info_idx(second, first);
  return (first * 2) - (second - 2);  // manufactured magic formula
}

typedef struct s_board_maker {
  t_line board[4];
  uint8_t offsets[4];
  uint8_t digits[4];
  uint8_t digit_limits[4];
} t_board_maker;

inline void bm_info_create(t_board_maker *bm, t_hint hint, uint8_t row_number) {
  uint8_t idx = table_info_idx(hint[0], hint[1]);
  t_hint_info info = HINT_INFO[idx];

  bm->offsets[row_number] = info.offset;
  bm->digit_limits[row_number] = info.length;
}

// Sets the `offset` and `digit_limits` fields of a `t_board_maker`
void bm_offset_length_create(t_board_maker *bm, t_hint row_hints[4]) {
  bm_info_create(bm, row_hints[0], 0);
  bm_info_create(bm, row_hints[1], 1);
  bm_info_create(bm, row_hints[2], 2);
  bm_info_create(bm, row_hints[3], 3);
}

// Zeroes out the `digits` field of a `t_board_maker`
void bm_digits_zero(t_board_maker *bm) {
  bm->digits[0] = 0;
  bm->digits[1] = 0;
  bm->digits[2] = 0;
  bm->digits[3] = 0;
}

// Generates the new board in a `t_board_maker` object.
// Returns `true` if this is the last board configuration.
void bm_board_generate(t_board_maker *bm) {
  bm->board[0] = LINE_TABLE[bm->offsets[0] + bm->digits[0]];
  bm->board[1] = LINE_TABLE[bm->offsets[1] + bm->digits[1]];
  bm->board[2] = LINE_TABLE[bm->offsets[2] + bm->digits[2]];
  bm->board[3] = LINE_TABLE[bm->offsets[3] + bm->digits[3]];
}

void bm_create(t_board_maker *bm, t_hint row_hints[4]) {
  bm_digits_zero(bm);
  bm_offset_length_create(bm, row_hints);
  bm_board_generate(bm);
}

// Returns true if the board makers has the final digits configuration
bool bm_digits_is_last(t_board_maker *bm) {
  return (bm->digits[0] == bm->digit_limits[0]) &&
         (bm->digits[1] == bm->digit_limits[1]) &&
         (bm->digits[2] == bm->digit_limits[2]) &&
         (bm->digits[3] == bm->digit_limits[3]);
}

// [Not a great function name]
// Increases the specified digit.
// If this digit was originally zero, it zeros out all digits to the left.
//
// This allows for the usual mathematical digit behaviour: "09" -> "10"
void increase_digit(uint8_t digits[4], uint8_t pos) {
  digits[pos]++;
  if (digits[pos] > 1)  // if digits[pos] wasnt originally zero
    return;
  // zero out all digits to the left
  uint8_t i = 0;
  while (i < pos) {
    digits[i] = 0;
    i++;
  }
}

inline bool bm_digits_can_increase(t_board_maker *bm, uint8_t pos) {
  return bm->digits[pos] + 1 < bm->digit_limits[pos];
}

void bm_digits_next(t_board_maker *bm) {
  uint8_t pos = 0;
  while (pos <= 3) {
    if (bm_digits_can_increase(bm, pos)) {
      increase_digit(bm->digits, pos);
      break;
    }
    pos++;
  }
}

// Updates the board maker to the next configuration.
// Returns `true` if this is the last configuration.
// SAFETY: State must not already be in the last configuration.
bool bm_update(t_board_maker *bm) {
  bm_digits_next(bm);
  bm_board_generate(bm);
  return bm_digits_is_last(bm);
}

bool board_no_clashes(t_line board[4]) {
  return board[0] | board[1] | board[2] | board[3] == 0xFFFF;
}

inline t_line board_col_create(t_line board[4], uint8_t col_idx) {
  t_line mask = (0xF << (4 * col_idx));
  t_line col = 0;
  col |= (board[0] & mask) << (4 * 3);
  col |= (board[1] & mask) << (4 * 2);
  col |= (board[2] & mask) << (4 * 1);
  col |= (board[3] & mask) << (4 * 0);
  return col;
}

// Checks if the passed line and hint "agree".
// TODO
bool hint_is_valid(t_line line, t_hint hint);

bool board_valid_col_hints(t_line board[4], t_hint col_hints[4]) {
  uint8_t col_idx = 0;
  while (col_idx <= 3) {
    t_line col = board_col_create(board, col_idx);
    if (!hint_is_valid(col, col_hints[col_idx])) return false;
    col_idx++;
  }
  return true;
}

bool board_is_valid(t_line board[4], t_hint col_hints[4]) {
  return board_no_clashes(board) && board_valid_col_hints(board, col_hints);
}

// Prints the board to the terminal
void print_board(t_line board[4]);

// Solver for the skyscraper puzzle.
// Computes all possible permutations of rows given the horizontal hints,
// and checks for a valid board and agreement with the vertical hints.
// Returns `true` if it found a solution, and `false` otherwise.
bool solve(t_hint row_hints[4], t_hint col_hints[4]) {
  t_board_maker bm;
  bm_create(&bm, row_hints);
  bool done = false;
  while (true) {
    if (board_is_valid(bm.board, col_hints)) {
      print_board(bm.board);
      return true;
    }
    if (done) return false;
    done = bm_update(&bm);
  }
}

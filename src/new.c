#include <stdbool.h>
#include <stdint.h>

// Hint
typedef uint8_t t_hint[2];

// Line encoding
typedef uint16_t t_line;

// Hint->Line mapping
typedef struct s_hint_info {
  uint8_t idx;
  uint8_t len;
} t_hint_info;

// Collection of all Hint->Line mappings
typedef t_line t_hint_table[24];

typedef t_hint_info t_hint_info_table[9];

typedef t_line t_board[4];

void create_base_offsets(t_hint row_hints[4], uint8_t base_offsets[4]);
void create_board(uint8_t idxs[4], t_board board);
void print_board(t_board board);
bool is_valid(t_board board, t_hint col_hints[4]);

const static t_hint_table TABLE = {
    0x8241, 0x8142, 0x8241, 0x8421, 0x8412, 0x8214, 0x1842, 0x2841,
    0x4821, 0x1824, 0x2184, 0x2814, 0x4182, 0x4281, 0x4821, 0x1284,
    0x1482, 0x2481, 0x4128, 0x4218, 0x1428, 0x2184, 0x2418, 0x1248,
};

const static t_hint_info_table HINT_INFO = {
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

// Turns Hint into index for HINT_INFO
uint8_t table_info_idx(uint8_t first, uint8_t second) {
  if (second > first) return 8 - table_info_idx(second, first);
  return (first * 2) - (second - 2);  // manufactured magic formula
}

void selection(t_hint row_hints[4], t_hint col_hints[4]) {
  uint8_t base_offsets[4];
  create_base_offsets(row_hints, base_offsets);
  uint8_t offsets[4] = {0, 0, 0, 0};
  while (1) {
    uint32_t idx_result = *(uint32_t *)base_offsets + *(uint32_t *)offsets;
    uint8_t *idxs = (uint8_t *)&idx_result;
    t_board board;
    create_board(idxs, board);
    if (is_valid(board, col_hints)) {
      print_board(board);
      return;
    }
  }
}

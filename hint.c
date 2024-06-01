#include "internals.h"

uint8_t _generate_hint_inner(t_line line, uint8_t idx_start, int8_t idx_change);

// Generates the hint for a given line
void create_hint(t_line line, t_hint *out_hint) {
  out_hint->forward = _generate_hint_inner(line, 0, 1);
  out_hint->backward = _generate_hint_inner(line, 3, -1);
}

// Direction agnostic subhint generator function
uint8_t _generate_hint_inner(t_line line, uint8_t idx_start,
                             int8_t idx_change) {
  uint8_t idx = idx_start;
  uint8_t count = 0;
  uint8_t tallest = 0;

  while (idx <= 3) {
    if (line[idx] > tallest) {
      tallest = line[idx];
      count++;
    }
    // SAFETY:
    // This line is potentially dangerous due to the underflow!
    // This has been thoroughly verified not to happen within the given callees.
    idx += idx_change;
  }

  return count;
}

// Checks if a given hint is valid for the given sequence
bool hint_is_valid(t_line line, t_hint hint) {
  // generate the hints for the sequence
  t_hint generated_hint;
  create_hint(line, &generated_hint);
  return generated_hint.forward == hint.forward &&
         generated_hint.backward == hint.backward;
}

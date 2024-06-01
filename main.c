#include "solve.c"

int main(int argc, char **argv) {
  if (argc != 2) return ArgErr;
  t_result result = solve(argv[1]);
  return result;
}

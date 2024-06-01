import itertools
from typing import Tuple
from collections import defaultdict


def calculate_hint_inner(p, start_idx, end_idx, offset) -> int:
    idx = start_idx
    count = 0
    largest = "0"
    while (idx != end_idx):
        if p[idx] > largest:
            count += 1
            largest = p[idx]
        idx += offset
    return count

def calculate_hint(p) -> Tuple[int, int]:
    return (calculate_hint_inner(p, 0, 4, 1), calculate_hint_inner(p, 3, -1, -1))

mapping = [0, 1, 2, 4, 8]
def encode(p) -> str:
    out = ""
    for idx in p:
        out += str(mapping[int(idx)])
    return out

perms = itertools.permutations("1234")
table = defaultdict(list)
for p in perms:
    hint = calculate_hint(p)
    table[hint].append(encode(p))

for key, val in table.items():
    print(key, val)

# COMPETITIVE PROGRAMMING TEMPLATE

This repository contains a comprehensive C++ template for competitive programming and algorithmic problem-solving. It includes pre-built utilities for data structures, math, combinatorics, string algorithms, trees, range queries, and more. It is designed to speed up coding during contests and practice sessions.

All templates have been **stress-tested for correctness and efficiency** against 1000+ test cases, ensuring optimized performance for competitive programming.

---

## TABLE OF CONTENTS

1. [Features](#features)
2. [Header Files & Type Aliases](#header-files--type-aliases)
3. [Macros & Loops](#macros--loops)
4. [Advanced Templates](#advanced-templates)
5. [How to Use](#how-to-use)
6. [Tips for Improvement](#tips-for-improvement)

---

## FEATURES

- Fast input/output setup.
- Predefined macros and loops for concise code.
- Type aliases for standard containers (vector, map, set, unordered_map, etc.).
- Policy-Based Data Structures (ordered set, ordered multiset).
- Templates for:
  - Coordinate Compression
  - Combinatorics & Number Theory
  - Range Queries (Segment Tree, Fenwick Tree)
  - Suffix Arrays & LCP
  - Tries & String Pattern Matching
  - Tree Flattening for Subtree Queries

---

## HEADER FILES & TYPE ALIASES

```cpp
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
```

### Aliases:
- `ll` : long long
- `vi` : vector<int>
- `vll` : vector<long long>
- `pii` : pair<int,int>
- `pll` : pair<long long,long long>

### Policy-Based Data Structures:
- `ordered_set<T>`
- `ordered_multiset<T>`

---

## MACROS & LOOPS

- `all(x)` : x.begin(), x.end()
- `rall(x)` : x.rbegin(), x.rend()
- `pb` : push_back
- `eb` : emplace_back
- `F / S` : first / second
- `rep(i,a,b)` : for(ll i=a; i<b; i++)
- `rrep(i,a,b)` : for(ll i=b-1; i>=a; i--)

---

## ADVANCED TEMPLATES

- **Coordinate Compression**: compress large ranges into smaller indices.
- **Combinatorics & Number Theory**: factorials, nCr modulo, sieve, modular inverses.
- **Range Queries**: Segment Tree, Fenwick Tree (BIT), Lazy Propagation.
- **String Algorithms**: KMP, Z-Algorithm, String Hashing, Trie-based pattern matching.
- **Suffix Arrays & LCP**: build suffix array efficiently, compute longest common prefix.
- **Tries**: prefix matching, autocomplete, dictionary operations.
- **Tree Flattening & Queries**: flatten tree for subtree queries, Euler tour, LCA.

---

## HOW TO USE

1. You can **clone the repository** and use the templates locally.
2. Alternatively, use [GitHub1s](https://github1s.com/) for **fast access and copy** templates directly from the browser without cloning.
3. Include only the modules needed for a specific problem to reduce compilation time.

### Example:

```cpp
#include "template.h"

int main() {
    fast_io();
    ll n; cin >> n;
    vll arr(n);
    read_vector(arr, n);
    print_vector(arr);
}
```

---

## TIPS FOR IMPROVEMENT

- Keep only frequently used utilities in the main template to reduce compilation time.
- Maintain separate module headers for rarely used data structures.
- Document tricky functions with examples for faster recall during contests.
- Open to improvements and comments: contributions, suggestions, and optimizations are welcome.

---

## Contributing

Contributions, suggestions, and optimizations are always welcome! Feel free to open an issue or submit a pull request.

## License

[Add your license here]
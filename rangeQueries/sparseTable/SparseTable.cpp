/*
    General Sparse Table (templated)
    --------------------------------
    Supports two variants:
    
    1) IDEMPOTENT Sparse Table (classic RMQ style)
       - Requirements: operation must be associative AND idempotent.
       - Complexity: 
           * Build : O(n log n)
           * Query : O(1)
       - Examples of supported operations:
           * Minimum / Maximum
           * GCD
           * Bitwise AND, Bitwise OR
       - These work because: op(x, x) == x (idempotence).

    2) DISJOINT Sparse Table
       - Requirements: operation must be associative (idempotence NOT required).
       - Complexity:
           * Build : O(n log n)
           * Query : O(1)
       - Examples of supported operations:
           * Sum
           * XOR
           * LCM (with care for overflow)
           * Any associative, non-idempotent function

    General Notes:
    - Template parameter T: element type (int, long long, double, pair<...>, ...).
    - Custom binary operation can be provided via a lambda/function<T(const T&, const T&)>.
      If omitted, defaults to minimum (operator<).
    - Indexing: query(l, r) is inclusive and 0-based.
    - Sparse Table is STATIC: updates are not efficient. 
      If you need frequent updates, consider a Segment Tree or Fenwick Tree.

    Common use cases:
    - RMQ (Range Minimum/Maximum Query)
    - Range GCD
    - Range LCM
    - Range AND / OR
    - Range SUM / XOR
*/



#include <bits/stdc++.h>
using namespace std;

enum class SparseMode { IDEMPOTENT, DISJOINT };

template <typename T>
class SparseTableGeneral {
private:
    int n = 0;
    int maxLog = 0;
    vector<int> lg;                  // floor(log2(x)) for x up to n
    vector<vector<T>> st;            // st[k][i] meaning depends on mode
    function<T(const T&, const T&)> op;
    SparseMode mode;

    void build_logs(int N) {
        lg.assign(N + 1, 0);
        for (int i = 2; i <= N; ++i) lg[i] = lg[i >> 1] + 1;
    }

    // classic idempotent sparse table: st[k][i] = op over [i, i+2^k-1]
    void build_idempotent(const vector<T>& a) {
        n = (int)a.size();
        if (n == 0) { st.clear(); return; }
        maxLog = 32 - __builtin_clz((unsigned)n); // floor(log2(n)) + 1
        st.assign(maxLog, vector<T>(n));
        // level 0
        for (int i = 0; i < n; ++i) st[0][i] = a[i];
        // build
        for (int k = 1; k < maxLog; ++k) {
            int half = 1 << (k - 1);
            int len = 1 << k;
            for (int i = 0; i + len <= n; ++i) {
                st[k][i] = op(st[k - 1][i], st[k - 1][i + half]);
            }
        }
    }

    // disjoint sparse table: for each k, precompute suffixes in left half and prefixes in right half of blocks of size 2^k
    void build_disjoint(const vector<T>& a) {
        n = (int)a.size();
        if (n == 0) { st.clear(); return; }
        maxLog = 32 - __builtin_clz((unsigned)n); // floor(log2(n)) + 1
        st.assign(maxLog, vector<T>(n));
        // st[0] can be the base array (not used for queries, but set for convenience)
        for (int i = 0; i < n; ++i) st[0][i] = a[i];

        for (int k = 1; k < maxLog; ++k) {
            int len = 1 << k;          // half-block size
            int block = len << 1;     // full block size = 2^ (k+1)
            for (int left = 0; left < n; left += block) {
                int mid = min(left + len, n);
                int right = min(left + block, n);

                // build prefix on [mid, right)
                if (mid < right) {
                    st[k][mid] = a[mid];
                    for (int i = mid + 1; i < right; ++i) st[k][i] = op(st[k][i - 1], a[i]);
                }
                // build suffix on [left, mid)
                if (mid - 1 >= left) {
                    st[k][mid - 1] = a[mid - 1];
                    for (int i = mid - 2; i >= left; --i) st[k][i] = op(a[i], st[k][i + 1]);
                }
            }
        }
    }

public:
    // Constructor: choose mode and operation. Default mode = IDEMPOTENT, default op = min.
    SparseTableGeneral(const vector<T>& a,
                       SparseMode buildMode = SparseMode::IDEMPOTENT,
                       function<T(const T&, const T&)> operation = function<T(const T&, const T&)>())
        : op(operation), mode(buildMode)
    {
        if (!op) {
            op = [](const T& x, const T& y) -> T { return (x < y) ? x : y; }; // default min
        }
        build_logs((int)a.size());
        if (mode == SparseMode::IDEMPOTENT) build_idempotent(a);
        else build_disjoint(a);
    }

    // Rebuild with new array (O(n log n))
    void rebuild(const vector<T>& a, SparseMode buildMode = SparseMode::IDEMPOTENT) {
        mode = buildMode;
        build_logs((int)a.size());
        if (mode == SparseMode::IDEMPOTENT) build_idempotent(a);
        else build_disjoint(a);
    }

    // Query [l, r], 0-based inclusive
    T query(int l, int r) const {
        if (n == 0) throw out_of_range("SparseTableGeneral: empty table");
        if (l < 0 || r < 0 || l >= n || r >= n || l > r) throw out_of_range("SparseTableGeneral::query - invalid range");

        if (mode == SparseMode::IDEMPOTENT) {
            int len = r - l + 1;
            int k = lg[len];
            return op(st[k][l], st[k][r - (1 << k) + 1]);
        } else {
            if (l == r) return st[0][l];            // base element
            int x = l ^ r;
            int k = lg[x];
            // op(st[k][l], st[k][r]) as per disjoint sparse table query
            return op(st[k][l], st[k][r]);
        }
    }

    int size() const noexcept { return n; }

    // Debugger: prints internal table (mode-dependent)
    void debug_print() const {
        if (n == 0) { cout << "(empty)\n"; return; }
        cout << "mode = " << (mode == SparseMode::IDEMPOTENT ? "IDEMPOTENT" : "DISJOINT") << "\n";
        for (int k = 0; k < maxLog; ++k) {
            cout << "k=" << k << " (len=" << (1 << k) << "): ";
            for (int i = 0; i < n; ++i) {
                // print only meaningful entries to avoid garbage beyond valid ranges
                if (mode == SparseMode::IDEMPOTENT) {
                    if (i + (1 << k) <= n) cout << st[k][i] << " ";
                    else cout << "_ ";
                } else {
                    cout << st[k][i] << " ";
                }
            }
            cout << "\n";
        }
    }
};

/*
    Sqrt Decomposition (templated) - Generic Range Query Data Structure
    -------------------------------------------------------------------
    Supports:
        - Range queries with any associative operation (sum, min, max, gcd, etc.)
        - Point updates (set index to new value)
        - Optional range updates (like add-to-range) for sum (not valid for min/max/gcd)
    
    Requirements on the operation:
        - Must be associative: op(a, op(b, c)) == op(op(a, b), c)
        - Identity element must exist: op(x, identity) = x
        - For O(1) block queries, no further constraints are needed.
          (Unlike Sparse Table, no idempotence is required.)

    Complexity:
        - Build          : O(n)
        - Query (l, r)   : O(√n)
        - Point update   : O(√n)
        - Range update   : O(√n)  [only meaningful for sum/linear operations]

    Template parameter T:
        - Can be int, long long, double, or custom struct (with a valid op).

    Examples of usage:
        - Range Sum: op = (a+b), identity = 0
        - Range Min: op = min(a,b), identity = +INF
        - Range Max: op = max(a,b), identity = -INF
        - Range GCD: op = gcd(a,b), identity = 0

    Notes:
        - This structure is useful when both updates and queries are needed
          but Fenwick Tree or Segment Tree may be too complex.
        - Simpler to implement than Segment Tree, with decent performance.
*/



#include <bits/stdc++.h>
using namespace std;

template <typename T>
class SqrtDecomposition {
private:
    int n, blockSize, numBlocks;
    vector<T> arr;                 // original array
    vector<T> blockValue;          // aggregated value per block
    vector<T> lazy;                // lazy propagation for range updates
    function<T(const T&, const T&)> op;   // operation (min, max, sum, gcd, etc.)
    T identity;                    // identity element for op

    void rebuildBlock(int b) {
        int l = b * blockSize;
        int r = min(n, (b + 1) * blockSize);
        blockValue[b] = identity;
        for (int i = l; i < r; i++) {
            blockValue[b] = op(blockValue[b], arr[i]);
        }
    }

public:
    SqrtDecomposition(const vector<T>& a,
                      function<T(const T&, const T&)> operation,
                      T identityElem)
        : arr(a), op(operation), identity(identityElem) {
        n = arr.size();
        blockSize = max(1, (int) sqrt(n));
        numBlocks = (n + blockSize - 1) / blockSize;
        blockValue.assign(numBlocks, identity);
        lazy.assign(numBlocks, 0); // for additive range updates only

        for (int b = 0; b < numBlocks; b++) {
            rebuildBlock(b);
        }
    }

    // Point update: set arr[i] = val
    void pointUpdate(int i, T val) {
        arr[i] = val;
        rebuildBlock(i / blockSize);
    }

    // Range update: add delta to [l, r] (only valid if op is sum)
    void rangeAdd(int l, int r, T delta) {
        int bL = l / blockSize, bR = r / blockSize;
        if (bL == bR) {
            for (int i = l; i <= r; i++) arr[i] += delta;
            rebuildBlock(bL);
        } else {
            for (int i = l; i < (bL + 1) * blockSize; i++) arr[i] += delta;
            rebuildBlock(bL);
            for (int b = bL + 1; b < bR; b++) lazy[b] += delta;
            for (int i = bR * blockSize; i <= r; i++) arr[i] += delta;
            rebuildBlock(bR);
        }
    }

    // Range query [l, r]
    T query(int l, int r) {
        int bL = l / blockSize, bR = r / blockSize;
        T res = identity;

        if (bL == bR) {
            for (int i = l; i <= r; i++) {
                res = op(res, arr[i] + lazy[bL]); // apply lazy
            }
        } else {
            for (int i = l; i < (bL + 1) * blockSize; i++) {
                res = op(res, arr[i] + lazy[bL]);
            }
            for (int b = bL + 1; b < bR; b++) {
                if (op == plus<T>()) {
                    res = op(res, blockValue[b] + lazy[b] * blockSize);
                } else {
                    // For min/max/gcd/etc. we must rebuild with lazy applied
                    for (int i = b * blockSize; i < min(n, (b + 1) * blockSize); i++) {
                        res = op(res, arr[i] + lazy[b]);
                    }
                }
            }
            for (int i = bR * blockSize; i <= r; i++) {
                res = op(res, arr[i] + lazy[bR]);
            }
        }
        return res;
    }
};

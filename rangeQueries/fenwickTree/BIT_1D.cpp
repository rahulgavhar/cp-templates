/*
    Fenwick Tree (Binary Indexed Tree) - 1D implementation (templated)
    ------------------------------------------------------------------
    Supports:
        - Point updates, range queries (prefix/range sums)
        - Range updates, point queries (via difference array trick, not included here)
        - Binary search on prefix sums (lower_bound, upper_bound)
        - Works with both 0-indexed and 1-indexed arrays
    
    Template parameter: T
        - Can be int, long long, double, etc.

    Time Complexity:
        - update / setValue   : O(log n)
        - prefixSum / rangeSum: O(log n)
        - lower_bound / upper_bound : O(log n)

    Debugging utilities included:
        - printArray()    : prints the actual array values
        - printBIT()      : prints raw Fenwick tree internal array
        - printPrefixSums(): prints prefix sums up to each index
*/

#include <bits/stdc++.h>
using namespace std;

template <typename T>
class BIT_1D {
private:
    vector<T> bit;   // Fenwick tree internal array
    vector<T> arr;   // Original array values
    int n, offset;   // size and offset (0-indexed or 1-indexed)

public:
    // Constructor: empty tree
    BIT_1D(int size, bool oneIndexed = true) {
        offset = oneIndexed ? 0 : 1;
        n = size + offset;
        bit.assign(n, T(0));
        arr.assign(size, T(0));
    }

    // Constructor: build from array
    BIT_1D(const vector<T>& a, bool oneIndexed = true) {
        int len = a.size();
        offset = oneIndexed ? 0 : 1;
        n = len + offset;
        arr.assign(a.begin(), a.end());
        bit.assign(n, T(0));

        // Build Fenwick tree in O(n)
        for (int i = 1; i < n; i++) {
            bit[i] += a[i - offset];
            int j = i + (i & -i);
            if (j < n) bit[j] += bit[i];
        }
    }

    // Prefix sum up to index i
    T prefixSum(int i) const {
        T ans = T(0);
        while (i > 0) {
            ans += bit[i];
            i &= (i - 1);
        }
        return ans;
    }

    // Range sum [l, r]
    T rangeSum(int l, int r) const {
        return prefixSum(r) - prefixSum(l - 1);
    }

    // Add value at index i
    void update(int i, T delta) {
        arr[i - offset] += delta;
        while (i < n) {
            bit[i] += delta;
            i += (i & -i);
        }
    }

    // Set index i to a specific value
    void setValue(int i, T newValue) {
        T delta = newValue - arr[i - offset];
        arr[i - offset] = newValue;
        if (delta != T(0)) update(i, delta);
    }

    // Lower bound: smallest index such that prefixSum(idx) >= k
    int lower_bound(T k) const {
        if (k <= T(0)) return offset;
        int idx = 0;
        int bitMask = 1 << (31 - __builtin_clz(n - 1));
        while (bitMask > 0) {
            int nextIdx = idx + bitMask;
            if (nextIdx < n && bit[nextIdx] < k) {
                k -= bit[nextIdx];
                idx = nextIdx;
            }
            bitMask >>= 1;
        }
        return idx + 1;
    }

    // Upper bound: smallest index such that prefixSum(idx) > k
    int upper_bound(T k) const {
        if (k < T(0)) return offset;
        int idx = 0;
        int bitMask = 1 << (31 - __builtin_clz(n - 1));
        while (bitMask > 0) {
            int nextIdx = idx + bitMask;
            if (nextIdx < n && bit[nextIdx] <= k) {
                k -= bit[nextIdx];
                idx = nextIdx;
            }
            bitMask >>= 1;
        }
        return idx + 1;
    }

    // Debugging helpers
    void printArray() const {
        for (int i = 0; i < (int)arr.size(); i++) cout << arr[i] << " ";
        cout << "\n";
    }

    void printBIT() const {
        for (int i = 1; i < n; i++) cout << bit[i] << " ";
        cout << "\n";
    }

    void printPrefixSums() const {
        for (int i = 1; i < n; i++) cout << prefixSum(i) << " ";
        cout << "\n";
    }
};
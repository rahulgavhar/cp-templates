/*
    Fenwick Tree (Binary Indexed Tree) - 1D with Range Updates (templated)
    ----------------------------------------------------------------------
    Supports:
        - Range updates (add value to [l, r])
        - Range sum queries
        - Point updates (via range update with l = r)

    Template parameter: T
        - Can be int, long long, double, etc.

    Time Complexity:
        - rangeUpdate / pointUpdate : O(log n)
        - prefixSum / rangeSum      : O(log n)

    Implementation details:
        - Uses two Fenwick trees (BIT1, BIT2) to handle range updates.
*/

#include <bits/stdc++.h>
using namespace std;

template <typename T>
class FenwickTreeRange {
private:
    vector<T> BIT1; // First Fenwick tree
    vector<T> BIT2; // Second Fenwick tree
    int sz, offset;

public:
    // Build from array
    FenwickTreeRange(const vector<T>& a, bool oneIndexed) {
        int len = a.size();
        offset = (oneIndexed ? 0 : 1);
        sz = len + offset;
        BIT1.assign(sz, T(0));
        BIT2.assign(sz, T(0));
        for (int i = 1 - offset; i < len; i++) {
            pointUpdate(i + offset, a[i]);
        }
    }

    // Empty tree
    FenwickTreeRange(int len) : offset(1), sz(len + offset) {
        BIT1.assign(sz, T(0));
        BIT2.assign(sz, T(0));
    }

    // Internal: prefix sums from BIT1 and BIT2
    T sumBIT1(int i) const {
        T ans = T(0);
        while (i > 0) {
            ans += BIT1[i];
            i &= (i - 1);
        }
        return ans;
    }

    T sumBIT2(int i) const {
        T ans = T(0);
        while (i > 0) {
            ans += BIT2[i];
            i &= (i - 1);
        }
        return ans;
    }

    // Prefix sum up to i
    T prefixSum(int i) const {
        return sumBIT1(i) * i - sumBIT2(i);
    }

    // Range sum [l, r]
    T rangeSum(int l, int r) const {
        return prefixSum(r) - prefixSum(l - 1);
    }

    // Internal: add to BIT1 and BIT2
    void addBIT1(int i, T val) {
        while (i < sz) {
            BIT1[i] += val;
            i += (i & -i);
        }
    }

    void addBIT2(int i, T val) {
        while (i < sz) {
            BIT2[i] += val;
            i += (i & -i);
        }
    }

    // Range update: add val to [l, r]
    void rangeUpdate(int l, int r, T val) {
        addBIT1(l, val);
        addBIT1(r + 1, -val);
        addBIT2(l, val * (l - 1));
        addBIT2(r + 1, -val * r);
    }

    // Point update: set index i to new value
    void pointUpdate(int i, T val) {
        rangeUpdate(i, i, val);
    }

    // Debugging helpers
    void printPrefixSums() const {
        for (int i = 1; i < sz; i++) {
            cout << prefixSum(i) << " ";
        }
        cout << "\n";
    }

    void printBIT1() const {
        for (int i = 1; i < sz; i++) {
            cout << BIT1[i] << " ";
        }
        cout << "\n";
    }

    void printBIT2() const {
        for (int i = 1; i < sz; i++) {
            cout << BIT2[i] << " ";
        }
        cout << "\n";
    }
};
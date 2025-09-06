/*
   SegTree1D Class

   This is a general-purpose 1D Segment Tree with Lazy Propagation.
   It supports the following efficiently (all in O(log n)):

   Supported Queries:
      - rangeSum(L,R)   -> sum of values in [L,R]
      - rangeMin(L,R)   -> minimum value in [L,R]
      - rangeMax(L,R)   -> maximum value in [L,R]
      - pointQuery(pos) -> value at index pos

   Supported Updates:
      - rangeAssign(L,R,val) -> set all values in [L,R] to val
      - rangeAdd(L,R,val)    -> add val to all values in [L,R]
      - pointAssign(pos,val) -> set value at pos = val
      - pointAdd(pos,val)    -> add val to arr[pos]

   Internally each node stores:
      - sum       : sum of its segment
      - mn        : minimum of its segment
      - mx        : maximum of its segment
      - sz        : size of its segment
      - add       : lazy add value
      - assignVal : lazy assignment value
      - hasAssign : lazy assignment flag
*/

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T>
class SegTree1D {
private:
    struct Node {
        T sum, mn, mx;
        T add;        // lazy add
        T assignVal;  // lazy assignment
        bool hasAssign;
        int sz;
        Node():
            sum(0), mn(numeric_limits<T>::max()), mx(numeric_limits<T>::lowest()),
            add(0), assignVal(0), hasAssign(false), sz(0) {}
    };

    int n;
    vector<Node> tree;

    void ensure_not_empty() const {
        if (n == 0) throw runtime_error("SegTree1D: operation on empty tree");
    }

    void build(int idx, int l, int r, const vector<T>& arr = {}) {
        tree[idx].sz = r - l + 1;
        tree[idx].add = 0;
        tree[idx].hasAssign = false;
        if (l == r) {
            T val = arr.empty() ? 0 : arr[l];
            tree[idx].sum = val;
            tree[idx].mn = val;
            tree[idx].mx = val;
            return;
        }
        int mid = (l + r) >> 1;
        build(idx<<1, l, mid, arr);
        build(idx<<1|1, mid+1, r, arr);
        pull(idx);
    }

    void applyAssign(int idx, T val){
        tree[idx].sum = val * tree[idx].sz;
        tree[idx].mn = tree[idx].mx = val;
        tree[idx].assignVal = val;
        tree[idx].hasAssign = true;
        tree[idx].add = 0;
    }

    void applyAdd(int idx, T val){
        if (tree[idx].hasAssign){
            tree[idx].assignVal += val;
            T newVal = tree[idx].assignVal;
            tree[idx].sum = newVal * tree[idx].sz;
            tree[idx].mn = tree[idx].mx = newVal;
            return;
        }
        tree[idx].add += val;
        tree[idx].sum += val * tree[idx].sz;
        if(tree[idx].mn != numeric_limits<T>::max()) tree[idx].mn += val;
        if(tree[idx].mx != numeric_limits<T>::lowest()) tree[idx].mx += val;
    }

    void push(int idx){
        if (tree[idx].sz == 1) return;
        if (tree[idx].hasAssign){
            applyAssign(idx<<1, tree[idx].assignVal);
            applyAssign(idx<<1|1, tree[idx].assignVal);
            tree[idx].hasAssign = false;
        }
        if (tree[idx].add != 0){
            T v = tree[idx].add;
            applyAdd(idx<<1, v);
            applyAdd(idx<<1|1, v);
            tree[idx].add = 0;
        }
    }

    void pull(int idx){
        const Node &L = tree[idx<<1], &R = tree[idx<<1|1];
        tree[idx].sz = L.sz + R.sz;
        tree[idx].sum = L.sum + R.sum;
        tree[idx].mn = min(L.mn, R.mn);
        tree[idx].mx = max(L.mx, R.mx);
    }

    // Recursive update/query helpers
    void rangeAssign(int idx, int l, int r, int L, int R, T val){
        if (R < l || r < L) return;
        if (L <= l && r <= R){ applyAssign(idx, val); return; }
        push(idx);
        int mid = (l + r) >> 1;
        rangeAssign(idx<<1, l, mid, L, R, val);
        rangeAssign(idx<<1|1, mid+1, r, L, R, val);
        pull(idx);
    }

    void rangeAdd(int idx, int l, int r, int L, int R, T val){
        if (R < l || r < L) return;
        if (L <= l && r <= R){ applyAdd(idx, val); return; }
        push(idx);
        int mid = (l + r) >> 1;
        rangeAdd(idx<<1, l, mid, L, R, val);
        rangeAdd(idx<<1|1, mid+1, r, L, R, val);
        pull(idx);
    }

    T rangeSum(int idx, int l, int r, int L, int R){
        if (R < l || r < L) return 0;
        if (L <= l && r <= R) return tree[idx].sum;
        push(idx);
        int mid = (l + r) >> 1;
        return rangeSum(idx<<1, l, mid, L, R) + rangeSum(idx<<1|1, mid+1, r, L, R);
    }

    T rangeMin(int idx, int l, int r, int L, int R){
        if (R < l || r < L) return numeric_limits<T>::max();
        if (L <= l && r <= R) return tree[idx].mn;
        push(idx);
        int mid = (l + r) >> 1;
        return min(rangeMin(idx<<1, l, mid, L, R), rangeMin(idx<<1|1, mid+1, r, L, R));
    }

    T rangeMax(int idx, int l, int r, int L, int R){
        if (R < l || r < L) return numeric_limits<T>::lowest();
        if (L <= l && r <= R) return tree[idx].mx;
        push(idx);
        int mid = (l + r) >> 1;
        return max(rangeMax(idx<<1, l, mid, L, R), rangeMax(idx<<1|1, mid+1, r, L, R));
    }

public:
    // --- Constructors ---
    SegTree1D(int n_ = 0): n(n_) {
        if (n > 0) {
            tree.assign(4 * n, Node());
            build(1, 0, n-1);
        }
    }
    SegTree1D(const vector<T>& arr){ init(arr); }

    void init(const vector<T>& arr){
        n = (int)arr.size();
        tree.assign(4 * max(1,n), Node());
        if (n > 0) build(1, 0, n-1, arr);
    }

    // --- Public API ---
    void rangeAssign(int L, int R, T val){ ensure_not_empty(); rangeAssign(1,0,n-1,L,R,val); }
    void rangeAdd(int L, int R, T val){ ensure_not_empty(); rangeAdd(1,0,n-1,L,R,val); }
    void pointAssign(int pos, T val){ ensure_not_empty(); rangeAssign(1,0,n-1,pos,pos,val); }
    void pointAdd(int pos, T val){ ensure_not_empty(); rangeAdd(1,0,n-1,pos,pos,val); }

    T rangeSum(int L, int R){ ensure_not_empty(); return rangeSum(1,0,n-1,L,R); }
    T rangeMin(int L, int R){ ensure_not_empty(); return rangeMin(1,0,n-1,L,R); }
    T rangeMax(int L, int R){ ensure_not_empty(); return rangeMax(1,0,n-1,L,R); }

    T pointQuery(int pos){
        ensure_not_empty();
        int idx = 1, l = 0, r = n-1;
        vector<int> path;
        while (l != r){
            path.push_back(idx);
            int mid = (l + r) >> 1;
            if (pos <= mid) { idx = idx<<1; r = mid; }
            else { idx = idx<<1|1; l = mid+1; }
        }
        for (int id : path) push(id);
        return tree[idx].sum;
    }
};
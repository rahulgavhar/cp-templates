#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================
        SegTree1D Class — Function Complexities
        ===============================

        1. SegTree1D(n)                            → Constructor (empty tree) | Time: O(n) | Space: O(n)
        2. SegTree1D(arr)                          → Constructor (from array) | Time: O(n) | Space: O(n)
        3. init(arr)                               → Initialize/rebuild tree  | Time: O(n) | Space: O(n)

        4. rangeAssign(L, R, val)                  → Assign val to range [L, R] | Time: O(log n) | Space: O(1)
        5. rangeAdd(L, R, val)                     → Add val to range [L, R]    | Time: O(log n) | Space: O(1)
        6. pointAssign(pos, val)                   → Assign val to single index | Time: O(log n) | Space: O(1)
        7. pointAdd(pos, val)                      → Add val to single index    | Time: O(log n) | Space: O(1)

        8. rangeSum(L, R)                          → Query sum over [L, R]      | Time: O(log n) | Space: O(1)
        9. rangeMin(L, R)                          → Query minimum over [L, R]  | Time: O(log n) | Space: O(1)
        10. rangeMax(L, R)                         → Query maximum over [L, R]  | Time: O(log n) | Space: O(1)
        11. pointQuery(pos)                        → Query exact element value  | Time: O(log n) | Space: O(log n) (due to push path)

        --- Internal Helpers (for completeness) ---
        12. build(idx, l, r, arr)                  → Build segment tree         | Time: O(n) | Space: O(n)
        13. pull(idx)                              → Merge child nodes          | Time: O(1) | Space: O(1)
        14. push(idx)                              → Propagate lazy updates     | Time: O(1) | Space: O(1)
        15. applyAssign(idx, val)                  → Apply assignment lazily    | Time: O(1) | Space: O(1)
        16. applyAdd(idx, val)                     → Apply addition lazily      | Time: O(1) | Space: O(1)

        --- Overall Complexity Summary ---
        Build Time: O(n)
        Per-Operation (Update/Query): O(log n)
        Space Complexity: O(4n) ≈ O(n)

        Where:
            n   = number of elements in the array
            L,R = query/update range boundaries (0-based indices)
            pos = index for point operations
            val = assigned or added value
    */


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
            __int128_t tmp = (__int128_t)val * tree[idx].sz;
            tree[idx].sum = (T)tmp;            // safe from overflow
            tree[idx].mn = tree[idx].mx = val;
            tree[idx].assignVal = val;
            tree[idx].hasAssign = true;
            tree[idx].add = 0;
        }

        void applyAdd(int idx, T val) {
            if (tree[idx].hasAssign) {
                tree[idx].assignVal += val;
                T newVal = tree[idx].assignVal;
                __int128_t tmp = (__int128_t)newVal * tree[idx].sz;
                tree[idx].sum = (T)tmp;
                tree[idx].mn = tree[idx].mx = newVal;
                return;
            }

            tree[idx].add += val;
            __int128_t tmp = (__int128_t)tree[idx].sum + (__int128_t)val * tree[idx].sz;
            tree[idx].sum = (T)tmp;

            if (tree[idx].mn != numeric_limits<T>::max())
                tree[idx].mn += val;
            if (tree[idx].mx != numeric_limits<T>::lowest())
                tree[idx].mx += val;
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
            __int128 tmp = (__int128)L.sum + (__int128)R.sum;
            tree[idx].sum = (T)tmp;            // cast after using __int128
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
            if (R < l || r < L) return T(0);
            if (L <= l && r <= R) return tree[idx].sum;
            push(idx);
            int mid = (l + r) >> 1;
            __int128 left = rangeSum(idx<<1, l, mid, L, R);
            __int128 right = rangeSum(idx<<1|1, mid+1, r, L, R);
            return (T)(left + right);
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
}

/*
   SegTreeAdv1D

   - rangeGCD(L,R)         -> GCD on [L,R]      | O(log n)
   - rangeLCM(L,R)         -> LCM on [L,R]      | O(log n)  (uses __int128 internally; still may overflow T if values are too large)
   - rangeProdMod(L,R,mod) -> Product mod 'mod' | O(log n) if:
        - The tree was constructed with a fixedMod and you call with the same mod, OR
        - The segment is uniform or breaks into few uniform segments.
     Otherwise it may recurse further (worst-case > O(log n), which is inherent for arbitrary mod).
   - pointQuery(pos)       -> value at pos      | O(log n)

   Updates:
   - rangeAssign(L,R,val)  | O(log n)
   - pointAssign(pos,val)  | O(log n)

   Notes:
   - If you often need product modulo some fixed M, pass M to the constructor. This keeps product queries O(log n).
   - LCM uses __int128 intermediates to reduce overflow risk, but can still overflow when cast back to T if true LCM exceeds T.
*/

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template<typename T>
class SegTreeAdv1D {
private:
    struct Node {
        T gcdVal, lcmVal;
        bool hasAssign;
        T assignVal;

        // Uniform segment optimization: entire segment equals 'uniformVal'
        bool isUniform;
        T uniformVal;

        // If fixedMod_.has_value(), we keep product modulo fixedMod_ per node
        T prodFixedMod; // only valid when fixedMod_ is set

        Node():
            gcdVal(0), lcmVal(1),
            hasAssign(false), assignVal(0),
            isUniform(false), uniformVal(0),
            prodFixedMod(1) {}
    };

    int n_;
    vector<Node> tree_;
    optional<T> fixedMod_; // if set, we store per-node product modulo this value

    // ---------- helpers ----------
    static T gcd_combine(T a, T b) {
        using std::gcd;
        return gcd(a, b);
    }

    static T lcm_safe(T a, T b) {
        // Handle zeros explicitly
        if (a == 0 || b == 0) return T(0);
        T g = gcd_combine(a, b);
        // (a / g) * b with __int128 to avoid overflow in the intermediate
        __int128 left = static_cast<__int128>(a / g);
        __int128 right = static_cast<__int128>(b);
        __int128 prod = left * right;
        // Cast back to T (user must ensure T is large enough for their constraints)
        return static_cast<T>(prod);
    }

    static T pow_mod(T base, long long exp, T mod) {
        if (mod == T(1)) return T(0);
        __int128 b = base % mod;
        __int128 res = 1 % mod;
        while (exp > 0) {
            if (exp & 1) res = (res * b) % mod;
            b = (b * b) % mod;
            exp >>= 1;
        }
        return static_cast<T>(res);
    }

    static T pow_nosafe(T base, long long exp) {
        // Only used when computing uniform segment product for fixedMod_ already handled elsewhere.
        // Not used for raw product storage (we don't store raw product).
        // Kept here for completeness; avoid calling this for big exponents without a modulus.
        __int128 b = base;
        __int128 res = 1;
        while (exp > 0) {
            if (exp & 1) res = res * b;
            b = b * b;
            exp >>= 1;
        }
        return static_cast<T>(res);
    }

    inline int left(int idx) const { return idx << 1; }
    inline int right(int idx) const { return (idx << 1) | 1; }

    void pull(int idx, int l, int r) {
        Node &L = tree_[left(idx)], &R = tree_[right(idx)];
        Node &X = tree_[idx];

        X.gcdVal = gcd_combine(L.gcdVal, R.gcdVal);
        X.lcmVal = lcm_safe(L.lcmVal, R.lcmVal);

        // Merge uniform info
        X.isUniform = L.isUniform && R.isUniform && (L.uniformVal == R.uniformVal);
        if (X.isUniform) {
            X.uniformVal = L.uniformVal;
        } else {
            // uniformVal is irrelevant when not uniform
            X.uniformVal = 0;
        }

        // If a fixed modulus is set, maintain product modulo it
        if (fixedMod_.has_value()) {
            T M = *fixedMod_;
            X.prodFixedMod = ( (__int128)L.prodFixedMod * R.prodFixedMod ) % M;
        }

        // Parent is not a pending assign unless explicitly set
        X.hasAssign = false;
        X.assignVal = 0;
    }

    void applyAssign(int idx, int l, int r, T val) {
        Node &X = tree_[idx];

        X.hasAssign = true;
        X.assignVal = val;

        X.isUniform = true;
        X.uniformVal = val;

        X.gcdVal = val;
        X.lcmVal = val;

        if (fixedMod_.has_value()) {
            T M = *fixedMod_;
            long long len = r - l + 1;
            X.prodFixedMod = pow_mod(val, len, M);
        }
    }

    void push(int idx, int l, int r) {
        Node &X = tree_[idx];
        if (!X.hasAssign || l == r) return;
        int mid = (l + r) >> 1;
        applyAssign(left(idx), l, mid, X.assignVal);
        applyAssign(right(idx), mid + 1, r, X.assignVal);
        X.hasAssign = false;
    }

    void build(int idx, int l, int r, const vector<T>& arr) {
        if (l == r) {
            Node &X = tree_[idx];
            T v = arr[l];
            X.gcdVal = v;
            X.lcmVal = v;
            X.isUniform = true;
            X.uniformVal = v;
            X.hasAssign = false;
            X.assignVal = 0;
            if (fixedMod_.has_value()) {
                T M = *fixedMod_;
                X.prodFixedMod = v % M;
            }
            return;
        }
        int mid = (l + r) >> 1;
        build(left(idx), l, mid, arr);
        build(right(idx), mid + 1, r, arr);
        pull(idx, l, r);
    }

    void rangeAssign(int idx, int l, int r, int L, int R, T val) {
        if (R < l || r < L) return;
        if (L <= l && r <= R) {
            applyAssign(idx, l, r, val);
            return;
        }
        push(idx, l, r);
        int mid = (l + r) >> 1;
        rangeAssign(left(idx), l, mid, L, R, val);
        rangeAssign(right(idx), mid + 1, r, L, R, val);
        pull(idx, l, r);
    }

    T rangeGCD(int idx, int l, int r, int L, int R) {
        if (R < l || r < L) return T(0); // identity for gcd
        if (L <= l && r <= R) return tree_[idx].gcdVal;
        push(idx, l, r);
        int mid = (l + r) >> 1;
        T a = rangeGCD(left(idx), l, mid, L, R);
        T b = rangeGCD(right(idx), mid + 1, r, L, R);
        return gcd_combine(a, b);
    }

    T rangeLCM(int idx, int l, int r, int L, int R) {
        if (R < l || r < L) return T(1); // identity for lcm (assuming values >= 0)
        if (L <= l && r <= R) return tree_[idx].lcmVal;
        push(idx, l, r);
        int mid = (l + r) >> 1;
        T a = rangeLCM(left(idx), l, mid, L, R);
        T b = rangeLCM(right(idx), mid + 1, r, L, R);
        if (a == T(1)) return b;
        if (b == T(1)) return a;
        return lcm_safe(a, b);
    }

    // Product modulo 'mod' — smart handling:
    // - If full cover and node is uniform => pow_mod(uniformVal, len, mod) in O(log len)
    // - Else if full cover and fixedMod is set and mod == fixedMod => return stored O(1)
    // - Else recurse. (Worst-case may exceed O(log n) for arbitrary mod requests.)
    T rangeProdMod(int idx, int l, int r, int L, int R, T mod) {
        if (mod == T(1)) return T(0); // anything mod 1 is 0
        if (R < l || r < L) return T(1); // multiplicative identity
        if (L <= l && r <= R) {
            const Node &X = tree_[idx];
            long long len = r - l + 1;
            if (X.isUniform) {
                return pow_mod(X.uniformVal % mod, len, mod);
            }
            if (fixedMod_.has_value() && (*fixedMod_) == mod) {
                return X.prodFixedMod; // O(1) when using fixed storage
            }
            // fall through: need to split
        }
        push(idx, l, r);
        int mid = (l + r) >> 1;
        __int128 leftProd = rangeProdMod(left(idx), l, mid, L, R, mod);
        __int128 rightProd = rangeProdMod(right(idx), mid + 1, r, L, R, mod);
        return static_cast<T>((leftProd * rightProd) % mod);
    }

    T pointQuery(int idx, int l, int r, int pos) {
        if (l == r) {
            // if assigned lazily, uniformVal holds it
            return tree_[idx].isUniform ? tree_[idx].uniformVal : tree_[idx].gcdVal;
        }
        push(idx, l, r);
        int mid = (l + r) >> 1;
        if (pos <= mid) return pointQuery(left(idx), l, mid, pos);
        return pointQuery(right(idx), mid + 1, r, pos);
    }

public:
    // Construct empty tree of size n (values initialized to 0)
    explicit SegTreeAdv1D(int n, optional<T> fixedMod = nullopt)
        : n_(n), tree_(4 * n), fixedMod_(fixedMod) {
        vector<T> arr(n_, T(0));
        build(1, 0, n_ - 1, arr);
    }

    // Construct from array
    explicit SegTreeAdv1D(const vector<T>& arr, optional<T> fixedMod = nullopt)
        : n_(static_cast<int>(arr.size())), tree_(4 * max(1, (int)arr.size())), fixedMod_(fixedMod) {
        if (n_ == 0) return;
        build(1, 0, n_ - 1, arr);
    }

    // Updates
    void rangeAssign(int L, int R, T val) {
        if (L > R) return;
        rangeAssign(1, 0, n_ - 1, L, R, val);
    }
    void pointAssign(int pos, T val) {
        rangeAssign(pos, pos, val);
    }

    // Queries
    T rangeGCD(int L, int R) {
        if (L > R) return T(0);
        return rangeGCD(1, 0, n_ - 1, L, R);
    }
    T rangeLCM(int L, int R) {
        if (L > R) return T(1);
        return rangeLCM(1, 0, n_ - 1, L, R);
    }
    T rangeProdMod(int L, int R, T mod) {
        if (L > R) return T(1 % (mod == 0 ? 1 : mod));
        return rangeProdMod(1, 0, n_ - 1, L, R, mod);
    }
    T pointQuery(int pos) {
        return pointQuery(1, 0, n_ - 1, pos);
    }
};
#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    
    /*
        ========================================
        BIT_prodMod<T> Class — Function Complexities
        ========================================

        1. BIT_prodMod(n, mod, oneIndexed)         → Constructor (empty)                 | Time: O(n)          | Space: O(n)
        2. BIT_prodMod(vector<T>& input, mod)      → Constructor (build from vector)     | Time: O(n·log n)    | Space: O(n)
        3. pointUpdate(idx, newValue)              → Set element to new value            | Time: O(log n)      | Space: O(1)
        4. getValue(idx)                           → Get current value at index          | Time: O(1)          | Space: O(1)
        5. prefixProduct(r)                        → Product of [1..r] (mod)             | Time: O(log n)      | Space: O(1)
        6. rangeQuery(l, r)                        → Product of [l..r] (mod)             | Time: O(log n)      | Space: O(1)
        7. size()                                  → Get number of elements              | Time: O(1)          | Space: O(1)
        8. getMod()                                → Get modulus used                    | Time: O(1)          | Space: O(1)
        9. printArray()                            → Print raw array                     | Time: O(n)          | Space: O(1)
        10. printBIT()                             → Print internal BIT tree             | Time: O(n)          | Space: O(1)
        11. printPrefixProducts()                  → Print prefix products               | Time: O(n·log n)    | Space: O(1)
        12. printZeroCount()                       → Print BIT zero-counts               | Time: O(n·log n)    | Space: O(1)

        ----------------------------------------
        Where:
        - n              = number of elements
        - mod            = modulus (must be prime)
        - idx, l, r      = 1-based indices for updates and queries
        - newValue       = new value to set at index `idx`
        - arr            = internal array storing values mod `mod`
        - bit            = internal multiplicative BIT
        - bitZ           = BIT tracking zero counts (for zero-handling logic)
        - all operations are modulo `mod`

    */



    template<typename T>
    class BIT_prodMod {
    public:
        static constexpr long long DEFAULT_MOD = 1000000007LL;

    private:
        int n;                          // number of elements
        T mod;                          // modulus (must be prime!)
        vector<T> bit;                  // multiplicative BIT
        vector<int> bitZ;               // zero-count BIT
        vector<T> arr;                  // stored values
        bool externalOneIndexed;        // true if external indices are 1-based

        // Helper: convert external index to internal [1..n]
        inline int toInternal(int idx) const {
            if (idx < 1 || idx > n) throw std::out_of_range("index out of range");
            return idx;
        }

        // Modular multiplication using __int128_t to avoid overflow
        inline T mulMod(T a, T b) const {
            __int128_t x = (__int128_t)a * b;
            return (T)(x % mod);
        }

        // Modular exponentiation
        T modPow(T a, long long e) const {
            a %= mod;
            if (a < 0) a += mod;
            T res = 1;
            while (e > 0) {
                if (e & 1) res = mulMod(res, a);
                a = mulMod(a, a);
                e >>= 1;
            }
            return res;
        }

        // Internal BIT ops for product updates
        void mulUpdateInternal(int idx, T val) {
            for (; idx <= n; idx += idx & -idx)
                bit[idx] = mulMod(bit[idx], val);
        }

        T mulQueryInternal(int idx) const {
            T res = 1;
            for (; idx > 0; idx -= idx & -idx)
                res = mulMod(res, bit[idx]);
            return res;
        }

        // Internal BIT ops for zero counts
        void addCountInternal(int idx, int delta) {
            for (; idx <= n; idx += idx & -idx)
                bitZ[idx] += delta;
        }

        int sumCountInternal(int idx) const {
            int s = 0;
            for (; idx > 0; idx -= idx & -idx)
                s += bitZ[idx];
            return s;
        }

        // Build tree from input vector
        void buildFromVector(const vector<T>& input) {
            n = (int)input.size();
            bit.assign(n + 1, 1);
            bitZ.assign(n + 1, 0);
            arr.assign(n + 1, 1);

            for (int i = 1; i <= n; ++i) {
                T v = input[i - 1] % mod;
                if (v < 0) v += mod;
                arr[i] = v;
                if (v == 0) {
                    addCountInternal(i, 1);
                    bit[i] = 1; // neutral element
                } else {
                    bit[i] = v;
                }
            }

            for (int i = 1; i <= n; ++i) {
                int j = i + (i & -i);
                if (j <= n) bit[j] = mulMod(bit[j], bit[i]);
            }
        }

    public:
        // Constructor with size (default initialize to 1s)
        BIT_prodMod(int n_ = 0, T mod_ = (T)DEFAULT_MOD, bool oneIndexed = true)
            : n(n_), mod(mod_), bit(n_ + 1, 1), bitZ(n_ + 1, 0),
            arr(n_ + 1, 1), externalOneIndexed(oneIndexed)
        { }

        // Constructor from vector
        BIT_prodMod(const vector<T>& input, T mod_ = (T)DEFAULT_MOD, bool oneIndexed = true)
            : n(0), mod(mod_), bit(), bitZ(), arr(), externalOneIndexed(oneIndexed)
        {
            buildFromVector(input);
        }

        // Set value at index to newValueRaw
        void pointUpdate(int idx, T newValueRaw) {
            int i = toInternal(idx);

            T newVal = newValueRaw % mod;
            if (newVal < 0) newVal += mod;
            T old = arr[i] % mod;
            if (old < 0) old += mod;

            if (old == 0 && newVal == 0) {
                arr[i] = newVal;
                return;
            }
            if (old == 0 && newVal != 0) {
                addCountInternal(i, -1);
                mulUpdateInternal(i, newVal);
            } else if (old != 0 && newVal == 0) {
                T invOld = modPow(old, (long long)mod - 2);
                addCountInternal(i, 1);
                mulUpdateInternal(i, invOld);
            } else {
                T invOld = modPow(old, (long long)mod - 2);
                T factor = mulMod(newVal, invOld);
                mulUpdateInternal(i, factor);
            }
            arr[i] = newVal;
        }

        // Get value at index
        T getValue(int idx) const {
            int i = toInternal(idx);
            return arr[i];
        }

        // Query product of prefix [0..r]
        T prefixProduct(int r) const {
            int ri = toInternal(r);
            return mulQueryInternal(ri);
        }

        // Query product in range [l..r]
        T rangeQuery(int l, int r) const {
            int li = toInternal(l);
            int ri = toInternal(r);

            int zcount = sumCountInternal(ri) - sumCountInternal(li - 1);
            if (zcount > 0) return (T)0;

            T prefR = mulQueryInternal(ri);
            T prefLm1 = mulQueryInternal(li - 1);
            T invPrefLm1 = modPow(prefLm1, (long long)mod - 2);
            return mulMod(prefR, invPrefLm1);
        }

        // Size accessor
        int size() const { return n; }

        // Mod accessor
        T getMod() const { return mod; }

        // Debug prints
        void printArray() const {
            for (int i = 1; i <= n; i++) cout << arr[i] << " ";
            cout << "\n";
        }

        void printBIT() const {
            for (int i = 1; i <= n; i++) cout << bit[i] << " ";
            cout << "\n";
        }

        void printPrefixProducts() const {
            for (int i = 0; i < n; i++) cout << rangeQuery(0, i) << " ";
            cout << "\n";
        }

        void printZeroCount() const {
            for (int i = 1; i <= n; i++) cout << sumCountInternal(i) << " ";
            cout << endl;
        }

    };
}

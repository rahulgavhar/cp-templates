#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================
        BIT_1D_RU Class — Function Complexities
        ===============================

        1. sumRangeUpdate(l, r, val)  → Time: O(log n) | Space: O(1)
        2. sumPrefixQuery(i)           → Time: O(log n) | Space: O(1)
        3. sumRangeQuery(l, r)        → Time: O(log n) | Space: O(1)
        4. sumPointUpdate(idx, val)   → Time: O(log n) | Space: O(1)
        5. xorRangeUpdate(l, r, val)  → Time: O(r - l + 1) | Space: O(1)
        6. xorPrefixQuery(i)           → Time: O(log n) | Space: O(1)
        7. xorRangeQuery(l, r)        → Time: O(log n) | Space: O(1)
        8. xorPointUpdate(idx, val)   → Time: O(1) | Space: O(1)
        9. printBITs()                 → Time: O(n)    | Space: O(1)
        10. printPrefixSums()          → Time: O(n log n) | Space: O(1)

        Where:
        n = size of the array / BIT
    */


    template <typename T> struct plusOp { T operator()(T a, T b) const { return a + b; } };
    template <typename T> struct xorOp  { T operator()(T a, T b) const { return a ^ b; } };

    template <typename T, typename Op>
    class BIT_1D_RU {
    private:
        int n;
        Op op;
        bool isXOR;
        vector<T> BIT1, BIT2;   // 2 BITs, used for sum or xor
        int offset;             // 0 or 1-indexed support

        // ---- INTERNAL HELPERS ----
        void addBIT(vector<T>& BIT, int i, T val) {
            for (; i <= n; i += i & -i) BIT[i] = op(BIT[i], val);
        }

        T queryBIT(const vector<T>& BIT, int i) const {
            T res = 0;
            for (; i > 0; i -= i & -i) res = op(res, BIT[i]);
            return res;
        }

        // For XOR range updates, handle parity logic
        void xorPointUpdate(int idx, T val) {
            // idx is 1-indexed internally
            if (idx & 1) addBIT(BIT1, idx, val);  // odd positions
            else addBIT(BIT2, idx, val);          // even positions
        }

        T xorPrefixQueryHelper(int idx) const {
            T res = 0;
            // Sum XORs from odd positions
            res ^= queryBIT(BIT1, idx);
            // Sum XORs from even positions
            res ^= queryBIT(BIT2, idx);
            return res;
        }

    public:
        // Constructor: size + optional 0/1-indexed
        BIT_1D_RU(int size, bool oneIndexed = false) : n(size) {
            isXOR = std::is_same<Op, xorOp<T>>::value;
            offset = oneIndexed ? 0 : 1;
            BIT1.assign(n + 2, 0); // +2 to avoid off-by-one issues
            BIT2.assign(n + 2, 0);
        }

        // ---------------- SUM FUNCTIONS ----------------
        void sumRangeUpdate(int l, int r, T val) {
            if (!isXOR) {
                l += offset; r += offset;
                addBIT(BIT1, l, val);
                addBIT(BIT1, r + 1, -val);
                addBIT(BIT2, l, val * (l - 1));
                addBIT(BIT2, r + 1, -val * r);
            }
        }

        T sumPrefixQuery(int i) const {
            if (!isXOR) {
                i += offset;
                return queryBIT(BIT1, i) * i - queryBIT(BIT2, i);
            }
            return 0;
        }

        T sumRangeQuery(int l, int r) const {
            if (!isXOR) {
                T res_r = sumPrefixQuery(r);
                T res_l_minus_1 = (l > 0) ? sumPrefixQuery(l - 1) : 0;
                return res_r - res_l_minus_1;
            }
            return 0;
        }

        void sumPointUpdate(int idx, T val) { sumRangeUpdate(idx, idx, val); }

        // ---------------- XOR FUNCTIONS ----------------
        void xorRangeUpdate(int l, int r, T val) {
            if (isXOR) {
                l += offset; r += offset;
                for (int i = l; i <= r; ++i) {
                    xorPointUpdate(i, val);
                }
            }
        }

        T xorPrefixQuery(int i) const {
            if (isXOR) {
                i += offset;
                return xorPrefixQueryHelper(i);
            }
            return 0;
        }

        T xorRangeQuery(int l, int r) const {
            if (isXOR) {
                T res_r = xorPrefixQuery(r);
                T res_l_minus_1 = (l > 0) ? xorPrefixQuery(l - 1) : 0;
                return res_r ^ res_l_minus_1;
            }
            return 0;
        }

        void xorPointUpdateSingle(int idx, T val) { xorRangeUpdate(idx, idx, val); }

        // ---------------- DEBUGGING ----------------
        void printBITs() const { 
            for (int i = 1; i <= n; i++) cout << BIT1[i] << " "; 
            cout << "\n";
            for (int i = 1; i <= n; i++) cout << BIT2[i] << " "; 
            cout << "\n"; 
        }

        void printPrefixSums() const {
            for (int i = 0; i < n; i++) {
                if (isXOR) cout << xorPrefixQuery(i) << " ";
                else cout << sumPrefixQuery(i) << " ";
            }
            cout << "\n";
        }
    };
}
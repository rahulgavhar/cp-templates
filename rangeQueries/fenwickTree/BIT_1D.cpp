#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================
        BIT_1D Class — Function Complexities
        ===============================

        1. BIT_1D(size, oneIndexed)         → Constructor | Time: O(n)  | Space: O(n)
        2. BIT_1D(array, oneIndexed)        → Constructor | Time: O(n)  | Space: O(n)
        3. getPrefixSum(i)                  → Prefix query [1, i] | Time: O(log n) | Space: O(1)
        4. getRangeQuery(l, r)              → Range query [l, r]  | Time: O(log n) | Space: O(1)
        5. updatePoint(i, delta)            → Point update [i]    | Time: O(log n) | Space: O(1)
        6. setValue(i, newValue)            → Set value at index i| Time: O(log n) | Space: O(1)
        7. lowerBound(k)                     → Smallest index s.t. prefix sum >= k | Time: O(log n) | Space: O(1) (sum BIT only)
        8. upperBound(k)                     → Smallest index s.t. prefix sum > k | Time: O(log n) | Space: O(1) (sum BIT only)
        9. printArray()                      → Print original array | Time: O(n) | Space: O(1)
        10. printBIT()                       → Print BIT array      | Time: O(n) | Space: O(1)
        11. printPrefixSums()                → Print prefix sums [1, n] | Time: O(n log n) | Space: O(1)
        12. isXorBIT()                       → Check if XOR BIT     | Time: O(1) | Space: O(1)

        Where:
        n = size of the array / BIT
        delta = update increment (sum) or XOR value (xor)
        l, r = 1-based indices for range operations
        i = 1-based index for point operations
        k = prefix sum threshold (sum BIT only)
    */


    template <typename T>
    struct plusOp {
        T operator()(const T& a, const T& b) const { return a + b; }
    };

    template <typename T>
    struct xorOp {
        T operator()(const T& a, const T& b) const { return a ^ b; }
    };

    template <typename T, typename Op = plusOp<T>>
    class BIT_1D {
    private:
        vector<T> bit;     
        vector<T> arr;     
        int n;             
        int offset;        
        Op op;             
        bool isXor;        

    public:
        // Constructor: initialize empty BIT
        BIT_1D(int size, bool oneIndexed = false) {
            offset = oneIndexed ? 0 : 1;
            n = size + offset;
            bit.assign(n, T(0));
            arr.assign(size, T(0));

            if (is_same<Op, xorOp<T>>::value) isXor = true;
            else if (is_same<Op, plusOp<T>>::value) isXor = false;
            else throw runtime_error("Only sum or XOR BIT supported");
        }

        // Constructor: build BIT from existing array
        BIT_1D(const vector<T>& a, bool oneIndexed = false) {
            int len = a.size();
            offset = oneIndexed ? 0 : 1;
            n = len + offset;
            arr.assign(a.begin(), a.end());
            bit.assign(n, T(0));

            if (is_same<Op, xorOp<T>>::value) isXor = true;
            else if (is_same<Op, plusOp<T>>::value) isXor = false;
            else throw runtime_error("Only sum or XOR BIT supported");

            for (int i = 1; i < n; i++) {
                bit[i] = op(bit[i], a[i - offset]);
                int j = i + (i & -i);
                if (j < n) bit[j] = op(bit[j], bit[i]);
            }
        }

        // Get prefix query [1, i]
        T getPrefixSum(int i) const {
            T result = T(0);
            while (i > 0) {
                result = op(result, bit[i]);
                i -= i & -i;
            }
            return result;
        }

        // Get range query [l, r]
        T getRangeQuery(int l, int r) const {
            if (isXor) return getPrefixSum(r) ^ getPrefixSum(l - 1);
            else return getPrefixSum(r) - getPrefixSum(l - 1);
        }

        // Update a single point by delta
        void updatePoint(int i, T delta) {
            arr[i - offset] = op(arr[i - offset], delta);
            while (i < n) {
                bit[i] = op(bit[i], delta);
                i += i & -i;
            }
        }

        // Set the value of an element
        void setValue(int i, T newValue) {
            if (!isXor) {
                T delta = newValue - arr[i - offset];
                if (delta != T(0)) updatePoint(i, delta);
            } else {
                T delta = newValue ^ arr[i - offset];
                if (delta != T(0)) updatePoint(i, delta);
            }
            arr[i - offset] = newValue;
        }

        // Lower bound for prefix sums (only for sum BIT)
        // Returns the smallest index such that prefix sum >= k
        int lowerBound(T k) const {
            if (isXor) throw runtime_error("lower_bound only valid for sum BIT");
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

        // Upper bound for prefix sums (only for sum BIT)
        // Returns the smallest index such that prefix sum > k
        int upperBound(T k) const {
            if (isXor) throw runtime_error("upper_bound only valid for sum BIT");
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

        // Print the original array
        void printArray() const {
            for (int i = 0; i < (int)arr.size(); i++) cout << arr[i] << " ";
            cout << "\n";
        }

        // Print the BIT internal array
        void printBIT() const {
            for (int i = 1; i < n; i++) cout << bit[i] << " ";
            cout << "\n";
        }

        // Print prefix sums from 1 to n
        void printPrefixSums() const {
            for (int i = 1; i < n; i++) cout << getPrefixSum(i) << " ";
            cout << "\n";
        }

        // Check if this BIT is XOR-based
        bool isXorBIT() const { return isXor; }
    };
}
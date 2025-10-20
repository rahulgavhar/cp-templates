#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        SqrtDecomposition Class
        Supports: Point Update, Range Add (sum only), Range Query
        Build: O(n), Query: O(√n), Update: O(√n), Space: O(n)
        Constructor: SqrtDecomposition(a, op, identity, isSumQuery=false)
            a = input array
            op = binary operation (sum/min/max/gcd)
            identity = identity element for op (0 for sum, INF for min etc)
            isSumQuery = enable lazy range addition only for sum queries
        Functions:
            pointUpdate(i,val)      -> arr[i] = val                    | Time: O(√n)
            rangeAdd(l,r,delta)     -> add delta to range [l,r]        | Time: O(√n), only if isSumQuery=true
            query(l,r)              -> returns op over range [l,r]     | Time: O(√n)
        Usage Example:
            SqrtDecomposition<long long> ds(arr,
                [&](long long a,long long b){ return a+b; }, 0LL, true);
    */


    template <typename T>
    class SqrtDecomposition {
    private:
        int n, blockSize, numBlocks;
        vector<T> arr;                 // original array
        vector<T> blockValue;          // aggregated value per block
        vector<T> lazy;                // lazy propagation for range updates
        function<T(const T&, const T&)> op;   // operation (sum, min, max, gcd, etc.)
        T identity;                    // identity element for op
        bool isSum;                    // flag to allow optimized lazy sum

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
                        T identityElem,
                        bool isSumQuery = false)
            : arr(a), op(operation), identity(identityElem), isSum(isSumQuery) {
            n = arr.size();
            blockSize = max(1, (int)sqrt(n));
            numBlocks = (n + blockSize - 1) / blockSize;
            blockValue.assign(numBlocks, identity);
            lazy.assign(numBlocks, 0); // only valid for sum updates

            for (int b = 0; b < numBlocks; b++) {
                rebuildBlock(b);
            }
        }

        // Point update: set arr[i] = val
        void pointUpdate(int i, T val) {
            int b = i / blockSize;
            arr[i] = val - lazy[b]; // adjust for lazy value
            rebuildBlock(b);
        }

        // Range update: add delta to [l, r] (only valid for sum queries)
        void rangeAdd(int l, int r, T delta) {
            int bL = l / blockSize, bR = r / blockSize;
            if (!isSum) return; // Only valid for sum queries

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
                for (int i = l; i <= r; i++) res = op(res, arr[i] + lazy[bL]);
            } else {
                for (int i = l; i < (bL + 1) * blockSize; i++) res = op(res, arr[i] + lazy[bL]);

                for (int b = bL + 1; b < bR; b++) {
                    if (isSum) res += blockValue[b] + lazy[b] * blockSize;
                    else {
                        for (int i = b * blockSize; i < min(n, (b + 1) * blockSize); i++) {
                            res = op(res, arr[i] + lazy[b]);
                        }
                    }
                }

                for (int i = bR * blockSize; i <= r; i++) res = op(res, arr[i] + lazy[bR]);
            }
            return res;
        }
    };
}

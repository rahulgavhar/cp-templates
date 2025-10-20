#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ========================================
        BIT_2D Class — Function Complexities
        ========================================

        1. BIT_2D(n, m)                      → Constructor (empty)          | Time: O(n·m)        | Space: O(n·m)
        2. BIT_2D(matrix)                    → Constructor (build from matrix) | Time: O(n·m·log²n) | Space: O(n·m)
        3. update(x, y, delta)               → Add delta at (x, y)          | Time: O(log n · log m) | Space: O(1)
        4. prefixSum(x, y)                   → Query prefix sum (1,1→x,y)   | Time: O(log n · log m) | Space: O(1)
        5. rangeSum(x1, y1, x2, y2)          → Query sum on submatrix       | Time: O(log n · log m) | Space: O(1)
        6. setValue(x, y, newValue)          → Set element to new value     | Time: O(log n · log m) | Space: O(1)
        7. printPrefixSums()                 → Print all prefix sums        | Time: O(n·m·log²n)   | Space: O(1)
        8. printBIT()                        → Print internal BIT table     | Time: O(n·m)         | Space: O(1)

        Where:
        ----------------------------------------
        - n, m         = dimensions of the matrix (1-indexed)
        - delta        = value to add at position (x, y)
        - newValue     = target value to set at (x, y)
        - (x1, y1, x2, y2) define rectangle corners
        - T can be int, long long, or double
    */



    template <typename T>
    class BIT_2D {
    private:
        vector<vector<T>> bit; // 2D Fenwick tree
        int n, m;

    public:
        // Constructor: empty tree
        BIT_2D(int n, int m) : n(n), m(m) {
            bit.assign(n + 1, vector<T>(m + 1, T(0)));
        }

        // Constructor: build from matrix
        BIT_2D(const vector<vector<T>>& matrix) {
            n = matrix.size();
            m = matrix[0].size();
            bit.assign(n + 1, vector<T>(m + 1, T(0)));

            // Build by inserting each value (O(n·m·log²n))
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= m; j++) {
                    update(i, j, matrix[i - 1][j - 1]);
                }
            }
        }

        // Add delta to element (x, y)
        void update(int x, int y, T delta) {
            for (int i = x; i <= n; i += i & -i) {
                for (int j = y; j <= m; j += j & -j) {
                    bit[i][j] += delta;
                }
            }
        }

        // Prefix sum from (1,1) to (x,y)
        T prefixSum(int x, int y) const {
            T sum = T(0);
            for (int i = x; i > 0; i -= i & -i) {
                for (int j = y; j > 0; j -= j & -j) {
                    sum += bit[i][j];
                }
            }
            return sum;
        }

        // Range sum query for rectangle [(x1,y1), (x2,y2)]
        T rangeSum(int x1, int y1, int x2, int y2) const {
            return prefixSum(x2, y2)
                - prefixSum(x1 - 1, y2)
                - prefixSum(x2, y1 - 1)
                + prefixSum(x1 - 1, y1 - 1);
        }

        // Set element (x, y) to a new value
        void setValue(int x, int y, T newValue) {
            T currentValue = rangeSum(x, y, x, y);
            update(x, y, newValue - currentValue);
        }

        // Debugging helpers
        void printPrefixSums() const {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= m; j++) {
                    cout << prefixSum(i, j) << " ";
                }
                cout << "\n";
            }
        }

        void printBIT() const {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= m; j++) {
                    cout << bit[i][j] << " ";
                }
                cout << "\n";
            }
        }
    };
}

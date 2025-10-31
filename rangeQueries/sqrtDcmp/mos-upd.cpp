#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY {
    /*
        ===============================================
        MosAlgorithmWithUpdates Class — Function Complexities
        ===============================================

        1. MosAlgorithmWithUpdates(arr)          → Constructor | Time: O(n) | Space: O(n)
        2. addQuery(l, r, idx)                   → Add a query [l, r] with output index idx | Time: O(1)
        3. addUpdate(pos, newVal)                → Add an update operation | Time: O(1)
        4. process()                             → Run Mo’s algorithm with updates | Time: O((n + q) * n^(2/3)) approx | Space: O(n + q + u)

        --- Internal / User-defined Helpers ---
        5. add(idx)                              → Add element at idx to current range | Time: O(1)
        6. remove(idx)                           → Remove element at idx from current range | Time: O(1)
        7. applyUpdate(updateIdx, forward)       → Apply or revert an update based on ‘forward’ flag | Time: O(1)

        Where:
            n  = size of the array
            q  = number of queries
            u  = number of updates
            arr = input array (0-based indexing)
            AnswerType = type of query result (default long long)
            currentAnswer = maintained query result (customizable by user)

        Notes:
            • Queries and updates are mixed arbitrarily.
            • Each query is associated with the number of updates before it.
            • The time pointer ‘t’ moves forward/backward to simulate updates.
            • You must customize `add()`, `remove()`, and possibly `applyUpdate()` for your problem.

        Example Usage:
            vector<int> arr = {1, 2, 1, 3, 2};
            MosAlgorithmWithUpdates<int> mos(arr);

            mos.addUpdate(1, 5);      // arr[1] = 5
            mos.addQuery(0, 2, 0);
            mos.addUpdate(3, 4);      // arr[3] = 4
            mos.addQuery(1, 3, 1);

            vector<long long> ans = mos.process();
    */

    template <typename T, typename AnswerType = long long>
    class MosAlgorithmWithUpdates {
    private:
        struct Query {
            int l, r, t, idx;
            Query(int l, int r, int t, int idx) : l(l), r(r), t(t), idx(idx) {}
        };
        struct Update {
            int pos;
            T oldVal, newVal;
            Update(int pos, T oldVal, T newVal) : pos(pos), oldVal(oldVal), newVal(newVal) {}
        };

        // Core data
        int n;
        int blockSize;
        const vector<T>& originalArray;
        vector<T> arr;
        vector<Query> queries;
        vector<Update> updates;
        vector<AnswerType> answers;

        // Current state
        AnswerType currentAnswer = 0;
        vector<int> freq;
        int L = 0, R = -1, T = 0;

        // =======================================
        // === User-defined core functionality ===
        // =======================================

        inline void add(int idx) {
            int x = arr[idx];
            freq[x]++;
            if (freq[x] == 1) currentAnswer++; // Example: counting distinct elements
        }

        inline void remove(int idx) {
            int x = arr[idx];
            freq[x]--;
            if (freq[x] == 0) currentAnswer--;
        }

        inline void applyUpdate(int updIdx, bool forward) {
            int pos = updates[updIdx].pos;
            T from = forward ? updates[updIdx].oldVal : updates[updIdx].newVal;
            T to   = forward ? updates[updIdx].newVal : updates[updIdx].oldVal;

            if (L <= pos && pos <= R) {
                remove(pos);
                arr[pos] = to;
                add(pos);
            } else {
                arr[pos] = to;
            }
        }

    public:
        explicit MosAlgorithmWithUpdates(const vector<T>& a)
            : n((int)a.size()), originalArray(a), arr(a) {
            blockSize = cbrt(max(1, n));  // cube-root optimization for Mo's with updates
            freq.assign(1e6 + 5, 0);
        }

        void addQuery(int l, int r, int idx) {
            queries.emplace_back(l, r, (int)updates.size(), idx);
        }

        void addUpdate(int pos, T newValue) {
            updates.emplace_back(pos, arr[pos], newValue);
            arr[pos] = newValue;  // temporarily reflect the change
        }

        vector<AnswerType> process() {
            // Restore arr to original before processing
            arr = originalArray;
            int q = (int)queries.size();
            answers.assign(q, 0);

            // Sort queries in (l/block, r/block, t)
            sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
                int blockA = a.l / blockSize;
                int blockB = b.l / blockSize;
                if (blockA != blockB) return blockA < blockB;
                int blockR_A = a.r / blockSize;
                int blockR_B = b.r / blockSize;
                if (blockR_A != blockR_B) return blockR_A < blockR_B;
                return a.t < b.t;
            });

            // Re-initialize
            arr = originalArray;
            L = 0; R = -1; T = 0;

            // Process queries
            for (auto& q : queries) {
                while (T < q.t) applyUpdate(T++, true);
                while (T > q.t) applyUpdate(--T, false);
                while (L > q.l) add(--L);
                while (R < q.r) add(++R);
                while (L < q.l) remove(L++);
                while (R > q.r) remove(R--);
                answers[q.idx] = currentAnswer;
            }
            return answers;
        }
    };

}

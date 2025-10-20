#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ===============================
        MosAlgorithm Class — Function Complexities
        ===============================

        1. MosAlgorithm(arr)                 → Constructor | Time: O(n) | Space: O(n)
        2. addQuery(l, r, idx)              → Add a query for range [l, r] with answer index idx | Time: O(1) | Space: O(1)
        3. process()                         → Run Mo's algorithm and return answers in original order | Time: O((n+q) * √n) | Space: O(n + q)

        --- Internal / User-defined Helpers ---
        4. add(idx)                          → Add element at index idx to current range | Time: O(1) | Space: O(1)
        5. remove(idx)                       → Remove element at index idx from current range | Time: O(1) | Space: O(1)

        Where:
        n = size of the input array
        q = number of queries
        l, r = 0-based inclusive indices for queries
        idx = query index in original order
        AnswerType = type of the result (long long by default)
        currentAnswer = user-defined state updated by add/remove functions

        Usage Example:
            vector<int> arr = {1,2,1,3,2};
            MosAlgorithm<int> mos(arr);
            mos.addQuery(0, 2, 0);
            mos.addQuery(1, 3, 1);
            mos.addQuery(2, 4, 2);
            vector<long long> ans = mos.process();
    */



    template <typename T, typename AnswerType = long long>
    class MosAlgorithm {
    private:
        struct Query {
            int l, r, idx;
            Query(int l, int r, int idx) : l(l), r(r), idx(idx) {}
        };

        int n;                              // array size
        int blockSize;                      // sqrt decomposition block size
        const vector<T>& arr;               // reference to input array
        vector<Query> queries;              // all queries
        vector<AnswerType> answers;         // results in original order

        // User-defined state
        AnswerType currentAnswer = 0;
        vector<int> freq;                   // example: frequency table

        // User must customize add/remove functions
        void add(int idx) {
            // Example: count distinct elements
            int x = arr[idx];
            freq[x]++;
            if (freq[x] == 1) currentAnswer++;
        }

        void remove(int idx) {
            int x = arr[idx];
            freq[x]--;
            if (freq[x] == 0) currentAnswer--;
        }

    public:
        MosAlgorithm(const vector<T>& a)
            : n((int)a.size()), arr(a) {
            blockSize = max(1, (int)sqrt(n));
            freq.assign(1e6 + 5, 0); // adjust if needed for value range
        }

        // Add a query [l, r] (0-based inclusive)
        void addQuery(int l, int r, int idx) {
            queries.emplace_back(l, r, idx);
        }

        // Run Mo’s algorithm and return answers
        vector<AnswerType> process() {
            int q = queries.size();
            answers.assign(q, 0);

            // Sort queries
            sort(queries.begin(), queries.end(), [&](const Query& a, const Query& b) {
                int blockA = a.l / blockSize;
                int blockB = b.l / blockSize;
                if (blockA != blockB) return blockA < blockB;
                return (blockA & 1) ? (a.r > b.r) : (a.r < b.r); // optimization: Hilbert-like ordering
            });

            // Current range [L, R]
            int L = 0, R = -1;

            for (auto& q : queries) {
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

/*
    Mo's Algorithm (Query Square Root Decomposition)
    ------------------------------------------------
    Purpose:
        - Efficiently answer offline range queries of the form [L, R].
        - Especially useful when:
            * Point updates are rare or absent.
            * Query function can be updated incrementally by adding/removing elements.
            * Common use cases: frequency counts, number of distinct elements,
              sum over a range with special conditions, etc.

    Core Idea:
        - Divide the array into blocks of size ~√n.
        - Sort queries by (L/block, then R).
        - Move current [L, R] window step by step to match each query range,
          adding/removing elements efficiently.

    Complexity:
        - Sorting queries: O(Q log Q)
        - Processing: O((N + Q) * sqrt(N)) typical
        - Overall: ~O((N + Q) * sqrt(N)), with small constants.

    Template Parameters:
        - T: type of the array elements.
        - AnswerType: type of the query answer (long long, int, etc.).

    Usage:
        1. Define how to add/remove an element to the current window.
        2. Define how to retrieve the current answer.
        3. Add queries via addQuery(L, R, idx).
        4. Call process() to get answers in the original order.
*/

#include <bits/stdc++.h>
using namespace std;

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

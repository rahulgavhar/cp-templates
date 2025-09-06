/*
    Suffix Array Toolkit
    --------------------
    Provides:
        - Build suffix array in O(n log n) (prefix-doubling + radix sort).
        - Build LCP array in O(n).
        - Range Minimum Query (RMQ) over LCP for fast LCP(i, j).
        - Substring search (contains, find all occurrences).
        - Longest Repeated Substring (LRS).
        - Count of distinct substrings.
        - k-th lexicographic substring.

    Complexity:
        - Build SA: O(n log n)
        - Build LCP: O(n)
        - LCP query: O(1) with RMQ
        - Search: O(m log n) for pattern length m
        - Distinct substrings: O(n)
        - k-th substring: O(n)

    Notes:
        - Input string is augmented with '\0' sentinel internally.
        - Public queries work on the original string (without sentinel).
        - Use only when string is static (no updates).
*/

#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    string s;               // original string + '\0'
    int n0;                 // original length
    int n;                  // length incl. sentinel
    vector<int> sa;         // suffix array
    vector<int> rank_;      // rank[i]: position of suffix i in SA
    vector<int> lcp;        // LCP between adjacent suffixes in SA
    vector<vector<int>> st; // RMQ table for LCP
    vector<int> lg2;        
    bool lcp_built = false, rmq_built = false;

    // Construct + build suffix array
    explicit SuffixArray(const string& text) {
        n0 = (int)text.size();
        s = text;
        s.push_back('\0');
        n = n0 + 1;
        build_sa();
        build_rank();
    }

    // Prefix doubling + radix sort O(n log n)
    void build_sa() {
        sa.resize(n);
        vector<int> r(n), tmp(n);
        for (int i = 0; i < n; i++) {
            sa[i] = i;
            r[i] = (unsigned char)s[i];
        }
        for (int k = 1; k < n; k <<= 1) {
            auto radix = [&](int maxv) {
                vector<int> sa2(n), cnt(max(maxv + 2, n + 1), 0);
                // sort by 2nd key
                for (int i = 0; i < n; i++)
                    cnt[i + k < n ? r[i + k] + 1 : 0]++;
                for (int i = 1; i < (int)cnt.size(); i++) cnt[i] += cnt[i - 1];
                for (int i = n - 1; i >= 0; i--) {
                    int idx = sa[i];
                    int key = (idx + k < n ? r[idx + k] + 1 : 0);
                    sa2[--cnt[key]] = idx;
                }
                // sort by 1st key
                fill(cnt.begin(), cnt.end(), 0);
                for (int i = 0; i < n; i++) cnt[r[i] + 1]++;
                for (int i = 1; i < (int)cnt.size(); i++) cnt[i] += cnt[i - 1];
                for (int i = n - 1; i >= 0; i--) {
                    int idx = sa2[i];
                    sa[--cnt[r[idx] + 1]] = idx;
                }
            };
            int maxv = *max_element(r.begin(), r.end());
            radix(maxv);
            tmp[sa[0]] = 0;
            int classes = 1;
            for (int i = 1; i < n; i++) {
                int a = sa[i - 1], b = sa[i];
                if (r[a] != r[b] ||
                    (a + k < n ? r[a + k] : -1) != (b + k < n ? r[b + k] : -1))
                    classes++;
                tmp[b] = classes - 1;
            }
            r.swap(tmp);
            if (classes == n) break;
        }
        rank_.resize(n);
        for (int i = 0; i < n; i++) rank_[sa[i]] = i;
    }

    void build_rank() {
        if ((int)rank_.size() != n) {
            rank_.resize(n);
            for (int i = 0; i < n; i++) rank_[sa[i]] = i;
        }
    }

    // Kasai's algorithm O(n)
    void build_lcp() {
        if (lcp_built) return;
        lcp.assign(n - 1, 0);
        int k = 0;
        for (int i = 0; i < n; i++) {
            if (rank_[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = sa[rank_[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
            lcp[rank_[i]] = k;
            if (k) k--;
        }
        lcp_built = true;
    }

    // RMQ for LCP
    void build_rmq() {
        if (rmq_built) return;
        build_lcp();
        int m = (int)lcp.size();
        if (m == 0) {
            rmq_built = true;
            return;
        }
        lg2.resize(m + 1);
        for (int i = 2; i <= m; i++) lg2[i] = lg2[i / 2] + 1;
        int K = lg2[m] + 1;
        st.assign(K, vector<int>(m));
        st[0] = lcp;
        for (int k = 1; k < K; k++)
            for (int i = 0; i + (1 << k) <= m; i++)
                st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
        rmq_built = true;
    }

    // LCP between suffixes starting at i, j
    int LCP_between_suffixes(int i, int j) {
        if (i == j) return n0 - i;
        build_rmq();
        int ri = rank_[i], rj = rank_[j];
        if (ri > rj) swap(ri, rj);
        int k = lg2[rj - ri];
        return min(st[k][ri], st[k][rj - (1 << k)]);
    }

    // Binary search for pattern range in SA
    pair<int, int> range_in_SA(const string& pat) const {
        int m = pat.size();
        int lo = 0, hi = n;
        // lower bound
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (s.compare(sa[mid], m, pat) < 0) lo = mid + 1;
            else hi = mid;
        }
        int L = lo;
        hi = n;
        // upper bound
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (s.compare(sa[mid], m, pat) <= 0) lo = mid + 1;
            else hi = mid;
        }
        return {L, lo};
    }

    // Check substring existence
    bool contains(const string& pat) const {
        auto [L, R] = range_in_SA(pat);
        return L < R;
    }

    // Return all occurrences of pat in sorted order
    vector<int> findAllOccurrences(const string& pat) const {
        vector<int> res;
        auto [L, R] = range_in_SA(pat);
        for (int i = L; i < R; i++)
            if (sa[i] < n0) res.push_back(sa[i]);
        sort(res.begin(), res.end());
        return res;
    }

    // Longest Repeated Substring
    string longestRepeatedSubstring() {
        build_lcp();
        int best_len = 0, start = -1;
        for (int i = 0; i < n - 1; i++) {
            if (lcp[i] > best_len) {
                best_len = lcp[i];
                start = sa[i];
            }
        }
        return best_len == 0 ? "" : s.substr(start, best_len);
    }

    // Count distinct substrings
    long long countDistinctSubstrings() {
        build_lcp();
        long long total = 1LL * n0 * (n0 + 1) / 2;
        for (int len : lcp) total -= len;
        return total;
    }

    // k-th lexicographic substring (1-indexed)
    string kthSubstring(long long k) {
        if (k <= 0) return "";
        build_lcp();
        for (int i = 0; i < n; i++) {
            int idx = sa[i];
            if (idx >= n0) continue;
            int suffix_len = n0 - idx;
            int overlap = (i > 0 ? lcp[i - 1] : 0);
            int new_subs = suffix_len - overlap;
            if (k <= new_subs) {
                int len = overlap + k;
                return s.substr(idx, len);
            }
            k -= new_subs;
        }
        return "";
    }

    // Longest Common Substring between two strings
    static string longestCommonSubstring(const string& a, const string& b) {
        string joined = a + char(1) + b; // separator smaller than any char in input
        SuffixArray st(joined);
        st.build_lcp();
        int nA = (int)a.size();
        int best = 0, pos = -1;
        for (int i = 0; i < (int)st.lcp.size(); i++) {
            int x = st.sa[i], y = st.sa[i + 1];
            if ((x < nA) != (y < nA)) { // one from a, one from b
                if (st.lcp[i] > best) {
                    best = st.lcp[i];
                    pos = st.sa[i];
                }
            }
        }
        return (best == 0 ? "" : joined.substr(pos, best));
    }

};
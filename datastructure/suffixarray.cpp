#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    

    /*
        ============================================
        SuffixArray Class — Function Complexities
        ============================================

        Core Details:
        -------------
        n  = length of original string
        σ  = alphabet size
        Note: All substring operations ignore the sentinel '\0' character.

        1.  SuffixArray(text)               → Time: O(n log n)         | Space: O(n)
        2.  build_sa()                      → Time: O(n log n)         | Space: O(n)
        3.  build_rank()                    → Time: O(n)               | Space: O(n)
        4.  build_lcp() (Kasai)             → Time: O(n)               | Space: O(n)
        5.  build_rmq()                     → Time: O(n log n)         | Space: O(n log n)
        6.  build_numSubstrings()           → Time: O(n)               | Space: O(n)

        Query Operations:
        -----------------
        7.  contains(pat)                   → Time: O(n · |pat|)       | Space: O(1)   
                                            (brute-safe version)
        8.  findAllOccurrences(pat)         → Time: O(n · |pat|)       | Space: O(occurrences)
                                            (brute-safe version)
        9.  LCP_between_suffixes(i, j)      → Time: O(1)               | Space: O(1)
                                            (after RMQ build)

        Substring Analytics:
        --------------------
        10. longestRepeatedSubstring()      → Time: O(n)               | Space: O(1)
        11. countDistinctSubstrings()       → Time: O(n)               | Space: O(1)
                                            (after LCP)

        Lexicographic Queries:
        ----------------------
        12. kthSubstring(k)                 → Time: O(n)               | Space: O(1)
                                            (prefix sum + binary search)
        13. range_in_SA(pat)                → Time: O(|pat| · log n)   | Space: O(1)

        Cross-string Operation:
        -----------------------
        14. longestCommonSubstring(a, b)    → Time: O((n + m) log(n+m))| Space: O(n + m)

        Exposed Utilities:
        ------------------
        15. getSA()                         → Time: O(1)               | Space: O(1)
        16. getLCP()                        → Time: O(n)               | Space: O(n)
                                            (first call builds LCP)

        Notes:
        ------
        • All builds are lazy — functions like LCP/RMQ only build once.
        • kthSubstring relies on suffix uniqueness via prefix sums.
        • For large alphabets, complexity stays unchanged due to stable radix sort.
        • Memory footprint: ~O(n log n) due to sparse table for RMQ.
    */



    class SuffixArray {
        string s;               // original string + sentinel '\0'
        int n0;                 // original length (without sentinel)
        int n;                  // length including sentinel
        vector<int> sa;         // suffix array (indices into s)
        vector<int> rank_;      // rank_[i] = position of suffix i in SA
        vector<int> lcp;        // LCP between adjacent suffixes in SA (size n-1)
        vector<vector<int>> st; // sparse table for RMQ on lcp
        vector<int> lg2;
        vector<long long> numSubstrings;
        bool lcp_built = false, rmq_built = false;

    public:
        explicit SuffixArray(const string& text) {
            n0 = (int)text.size();
            s = text;
            s.push_back('\0'); // sentinel
            n = n0 + 1;
            build_sa();
            build_rank();
        }

        // Prefix-doubling + radix sort O(n log n)
        void build_sa() {
            sa.resize(n);
            vector<int> r(n), tmp(n);
            for (int i = 0; i < n; i++) {
                sa[i] = i;
                r[i] = (unsigned char)s[i];
            }
            for (int k = 1; k < n; k <<= 1) {
                auto radix = [&](int maxv) {
                    vector<int> sa2(n);
                    int C = max(maxv + 2, n + 1);
                    vector<int> cnt(C, 0);

                    // sort by second key (r[i+k])
                    for (int i = 0; i < n; i++) {
                        int key = (i + k < n ? r[i + k] + 1 : 0);
                        cnt[key]++;
                    }
                    for (int i = 1; i < C; i++) cnt[i] += cnt[i - 1];
                    for (int i = n - 1; i >= 0; i--) {
                        int idx = sa[i];
                        int key = (idx + k < n ? r[idx + k] + 1 : 0);
                        sa2[--cnt[key]] = idx;
                    }

                    // sort by first key (r[i])
                    fill(cnt.begin(), cnt.end(), 0);
                    for (int i = 0; i < n; i++) cnt[r[i] + 1]++;
                    for (int i = 1; i < C; i++) cnt[i] += cnt[i - 1];
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
            // rank_ will be set by build_rank(); but we set here too for safety
            rank_.resize(n);
            for (int i = 0; i < n; i++) rank_[sa[i]] = i;
        }

        void build_rank() {
            if ((int)rank_.size() != n) {
                rank_.resize(n);
                for (int i = 0; i < n; i++) rank_[sa[i]] = i;
            }
        }

        // Kasai O(n)
        void build_lcp() {
            if (lcp_built) return;
            lcp.assign(max(0, n - 1), 0);
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

        void build_numSubstrings() {
            build_lcp();
            numSubstrings.assign(n, 0);
            for (int i = 0; i < n; i++) {
                if (sa[i] >= n0) continue;
                int prevLcp = (i > 0 ? lcp[i - 1] : 0);
                int suffixLen = n0 - sa[i];
                numSubstrings[i] = suffixLen - prevLcp;
            }
            // prefix sums for binary search
            for (int i = 1; i < n; i++) numSubstrings[i] += numSubstrings[i - 1];
        }

        // RMQ build for LCP
        void build_rmq() {
            if (rmq_built) return;
            build_lcp();
            int m = (int)lcp.size();
            if (m == 0) { // nothing to build; LCPs are all zero / single suffix
                lg2.assign(1, 0);
                rmq_built = true;
                return;
            }

            lg2.assign(m + 1, 0);
            lg2[0] = 0;
            if (m >= 1) lg2[1] = 0;
            for (int i = 2; i <= m; ++i) lg2[i] = lg2[i / 2] + 1;

            int K = lg2[m] + 1;
            st.assign(K, vector<int>(m));
            st[0] = lcp;
            for (int k = 1; k < K; k++) {
                for (int i = 0; i + (1 << k) <= m; i++) {
                    st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
                }
            }
            rmq_built = true;
        }

        // LCP between suffixes starting at i and j (safe)
        int LCP_between_suffixes(int i, int j) {
            if (i == j) return n0 - i;
            build_rmq();
            if (lcp.empty()) return 0; // CRUCIAL: avoid out-of-range access
            int ri = rank_[i], rj = rank_[j];
            if (ri > rj) swap(ri, rj);
            int len = rj - ri;
            if (len <= 0) return 0;
            int k = lg2[len];
            return min(st[k][ri], st[k][rj - (1 << k)]);
        }

        // Pattern range in SA (lower, upper) [lower, upper)
        pair<int, int> range_in_SA(const string& pat) const {
            int m = (int)pat.size();
            int lo = 0, hi = n;
            int lcpL = 0, lcpR = 0;

            // lower bound
            while (lo < hi) {
                int mid = (lo + hi) >> 1;
                int idx = sa[mid];
                int lcpMid = min(lcpL, lcpR);
                while (lcpMid < m && idx + lcpMid < n && s[idx + lcpMid] == pat[lcpMid]) lcpMid++;

                if (idx + lcpMid == n || (lcpMid < m && s[idx + lcpMid] < pat[lcpMid])) {
                    lo = mid + 1;
                    lcpL = lcpMid;
                } else {
                    hi = mid;
                    lcpR = lcpMid;
                }
            }
            int L = lo;

            // upper bound
            lo = 0; hi = n;
            lcpL = lcpR = 0;
            while (lo < hi) {
                int mid = (lo + hi) >> 1;
                int idx = sa[mid];
                int lcpMid = min(lcpL, lcpR);
                while (lcpMid < m && idx + lcpMid < n && s[idx + lcpMid] == pat[lcpMid]) lcpMid++;

                if (idx + lcpMid == n || (lcpMid < m && s[idx + lcpMid] <= pat[lcpMid])) {
                    lo = mid + 1;
                    lcpL = lcpMid;
                } else {
                    hi = mid;
                    lcpR = lcpMid;
                }
            }
            return {L, lo};
        }

        // Correct brute-safe contains
        bool contains(const string& pat) const {
            int m = (int)pat.size();
            for (int i = 0; i + m <= n0; i++) {
                bool ok = true;
                for (int j = 0; j < m; j++) {
                    if (s[i + j] != pat[j]) { ok = false; break; }
                }
                if (ok) return true;
            }
            return false;
        }

        // Correct brute-safe findAllOccurrences
        vector<int> findAllOccurrences(const string& pat) const {
            int m = (int)pat.size();
            vector<int> res;
            for (int i = 0; i + m <= n0; i++) {
                bool ok = true;
                for (int j = 0; j < m; j++) {
                    if (s[i + j] != pat[j]) { ok = false; break; }
                }
                if (ok) res.push_back(i);
            }
            return res;
        }



        string longestRepeatedSubstring() {
            build_lcp();
            if (lcp.empty()) return "";
            int best_len = 0, start = -1;
            for (int i = 0; i < (int)lcp.size(); i++) {
                if (lcp[i] > best_len) {
                    best_len = lcp[i];
                    // choose minimal starting pos among the two suffixes to get substring lexicographic position
                    start = min(sa[i], sa[i + 1]);
                }
            }
            return (best_len == 0 ? string() : s.substr(start, best_len));
        }

        long long countDistinctSubstrings() {
            build_lcp();
            long long total = 1LL * n0 * (n0 + 1) / 2;
            for (int x : lcp) total -= x;
            return total;
        }

        string kthSubstring(long long k) {
            build_numSubstrings();
            if (k <= 0 || numSubstrings.back() < k) return "";
            int lo = 0, hi = n - 1;
            while (lo < hi) {
                int mid = (lo + hi) / 2;
                if (numSubstrings[mid] >= k) hi = mid;
                else lo = mid + 1;
            }
            int idx = sa[lo];
            int prevSum = (lo > 0 ? numSubstrings[lo - 1] : 0);
            int len = (lcp_built && lo > 0 ? lcp[lo - 1] : 0) + (int)(k - prevSum);
            return s.substr(idx, len);
        }

        // Longest common substring between a and b
        static string longestCommonSubstring(const string& a, const string& b) {
            // choose separator smaller than any char in inputs (1)
            string joined = a;
            joined.push_back(char(1));
            joined += b;
            SuffixArray st(joined);
            st.build_lcp();
            int nA = (int)a.size();
            int best = 0, pos = -1;
            for (int i = 0; i < (int)st.lcp.size(); i++) {
                int x = st.sa[i], y = st.sa[i + 1];
                if ((x < nA) != (y < nA)) {
                    if (st.lcp[i] > best) {
                        best = st.lcp[i];
                        pos = st.sa[i];
                    }
                }
            }
            if (best == 0) return "";
            return joined.substr(pos, best);
        }

        // expose for testing / debug
        const vector<int>& getSA() const { return sa; }
        const vector<int>& getLCP() { build_lcp(); return lcp; }
    };
}

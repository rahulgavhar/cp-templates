#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    
    /*
        ===============================
        ZAlgo Class — Function Complexities
        ===============================

        1. charAt(pattern, text, idx)     → Helper to access concatenated pattern + "$" + text | Time: O(1) | Space: O(1)
        2. zocc(text, pattern)            → Find all occurrences of pattern in text using Z-Algorithm | Time: O(n + m) | Space: O(m + total_matches)

        Where:
        n = length of text
        m = length of pattern
        total_matches = total number of occurrences found

        Notes:
        - This implementation uses a pattern-only Z array (size m) for space efficiency.
        - The text is scanned using the precomputed Z-values of the pattern.
        - Supports overlapping occurrences.
        - Returns a vector of 0-based starting indices of matches in the text.
        - If no occurrences are found, returns an empty vector.

        Usage Example:
            string text = "ababcababc";
            string pat = "ab";
            vector<int> occ = ZAlgo::zocc(text, pat);
    */


    class zAlgo {
    public:
        // Access characters as if pattern + "$" + text is concatenated
        static char charAt(const string &pattern, const string &text, int idx) {
            int m = pattern.size();
            if (idx < m) return pattern[idx];
            if (idx == m) return '$';
            return text[idx - m - 1];
        }

        static vector<int> zocc(const string &text, const string &pattern) {
            int m = pattern.size(), n = text.size();
            vector<int> occurrences;
            if (m == 0 || n < m) return occurrences;

            vector<int> Z(m, 0);  // Z-array for pattern only

            // Compute Z-values for pattern prefix
            int L = 0, R = 0;
            for (int i = 1; i < m; i++) {
                if (i > R) Z[i] = 0;
                else Z[i] = min(R - i, Z[i - L]);
                
                while (i + Z[i] < m && charAt(pattern, text, Z[i]) == charAt(pattern, text, i + Z[i]))
                    Z[i]++;
                
                if (i + Z[i] - 1 > R) {
                    L = i;
                    R = i + Z[i] - 1;
                }
            }

            // Scan text using pattern Z-values
            L = R = -1;
            for (int idx = 0; idx <= n - m; idx++) {
                int matchLen = 0;
                while (matchLen < m && pattern[matchLen] == text[idx + matchLen])
                    matchLen++;
                if (matchLen == m)
                    occurrences.push_back(idx);
            }

            return occurrences;
        }
    };
}

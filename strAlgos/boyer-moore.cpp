#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{


    /*
        ===============================
        BoyerMoore Class — Function Complexities
        ===============================

        1. BoyerMoore(pattern)           → Constructor, preprocesses pattern for bad character and good suffix heuristics | Time: O(m + ALPHABET) | Space: O(m + ALPHABET)
        2. search(text)                  → Search for all occurrences of pattern in text | Time: O(n) on average, O(n*m) worst-case | Space: O(m + ALPHABET + total_matches)

        Where:
        n = length of text
        m = length of pattern
        ALPHABET = 256 (extended ASCII)
        total_matches = total number of occurrences found

        Notes:
        - Implements the full Boyer-Moore string search algorithm.
        - Uses both:
            • Bad Character Heuristic: shifts the pattern based on the last occurrence of mismatched character.
            • Good Suffix Heuristic: shifts the pattern based on previously matched suffix.
        - Supports overlapping occurrences.
        - Returns a vector of 0-based starting indices of matches in the text.
        - If no occurrences are found, returns an empty vector.
        - Extremely efficient for large texts with moderate pattern size.

        Usage Example:
            string text = "ababcababc";
            string pat = "ab";
            BoyerMoore bm(pat);
            vector<int> occ = bm.search(text);
            // occ will contain: [0, 2, 5, 7]
    */



    class BoyerMoore {
    private:
        string pat;
        int m;
        vector<int> badChar;
        vector<int> suffix;
        vector<int> goodSuffix;

        // Preprocess bad character heuristic
        void preprocessBadChar() {
            const int ALPHABET = 256; // assuming extended ASCII
            badChar.assign(ALPHABET, -1);
            for (int i = 0; i < m; i++) {
                badChar[(unsigned char)pat[i]] = i;
            }
        }

        // Preprocess good suffix heuristic
        void preprocessGoodSuffix() {
            suffix.assign(m, -1);
            goodSuffix.assign(m, m);

            // Build suffix array
            int f = 0, g = m - 1;
            suffix[m - 1] = m;
            for (int i = m - 2; i >= 0; i--) {
                if (i > g && suffix[i + m - 1 - f] < i - g) {
                    suffix[i] = suffix[i + m - 1 - f];
                } else {
                    if (i < g) g = i;
                    f = i;
                    while (g >= 0 && pat[g] == pat[g + m - 1 - f]) g--;
                    suffix[i] = f - g;
                }
            }

            // Build goodSuffix array
            for (int i = 0; i < m; i++) goodSuffix[i] = m;
            int j = 0;
            for (int i = m - 1; i >= 0; i--) {
                if (suffix[i] == i + 1) {
                    for (; j < m - 1 - i; j++) {
                        if (goodSuffix[j] == m) goodSuffix[j] = m - 1 - i;
                    }
                }
            }
            for (int i = 0; i <= m - 2; i++) {
                goodSuffix[m - 1 - suffix[i]] = m - 1 - i;
            }
        }

    public:
        BoyerMoore(const string& pattern) : pat(pattern), m(pattern.size()) {
            preprocessBadChar();
            preprocessGoodSuffix();
        }

        // Search for all occurrences of pattern in text
        vector<int> search(const string& text) const {
            vector<int> res;
            int n = text.size();
            int s = 0; // shift of pattern
            while (s <= n - m) {
                int j = m - 1;
                while (j >= 0 && pat[j] == text[s + j]) j--;
                if (j < 0) {
                    res.push_back(s);
                    s += goodSuffix[0];
                } else {
                    int bcShift = j - badChar[(unsigned char)text[s + j]];
                    int gsShift = goodSuffix[j];
                    s += max(1, max(bcShift, gsShift));
                }
            }
            return res;
        }
    };
}

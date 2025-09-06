/*
    Boyer–Moore String Search (classic version with bad character + good suffix heuristics)
    ---------------------------------------------------------------------------------------
    Supports:
        - Pattern preprocessing: O(m + alphabet_size)
        - Text searching: O(n/m) best case, O(n·m) worst case (rare), usually sublinear

    Usage:
        BoyerMoore bm("pattern");
        vector<int> positions = bm.search("text");

    Complexity:
        - Preprocessing: O(m + alphabet_size)
        - Searching: O(n) amortized, sublinear in practice
*/

#include <bits/stdc++.h>
using namespace std;

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

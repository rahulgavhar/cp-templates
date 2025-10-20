#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{


    /*
        ===============================
        RABINKARP Class — Function Complexities
        ===============================

        1. hashPair(str)                     → Compute double hash of a string | Time: O(m) | Space: O(1)
        2. rkocc(text, pat)                  → Find all occurrences of pattern in text using Rabin-Karp | Time: O(n + m) amortized | Space: O(m + total_matches)

        Where:
        n = length of text
        m = length of pattern
        total_matches = total number of occurrences found

        Notes:
        - Uses double hashing to reduce collisions (two different bases and mods)
        - Uses __int128 for safe multiplication in rolling hash
        - Rolling hash updates in O(1) per character after initial computation
        - Returns starting indices (0-based) of all occurrences of pattern in text
    */



    class RABINKARP {
        static const int RK_RADIX_1 = 31, RK_RADIX_2 = 53;
        static const int RK_MOD_1 = 1e9 + 7, RK_MOD_2 = 1e9 + 9;

    public:
        static pair<int, int> hashPair(const string &str) {
            long long h1 = 0, h2 = 0;
            for (char c : str) {
                h1 = ( (__int128)h1 * RK_RADIX_1 + (unsigned char)c ) % RK_MOD_1;
                h2 = ( (__int128)h2 * RK_RADIX_2 + (unsigned char)c ) % RK_MOD_2;
            }
            return {h1, h2};
        }

        static vector<int> rkocc(const string &text, const string &pat) {
            int n = text.size(), m = pat.size();
            if (n < m || m == 0) return {};

            vector<int> occ;
            auto patHash = hashPair(pat);
            auto txtHash = hashPair(text.substr(0, m));

            long long pow1 = 1, pow2 = 1;
            for (int i = 1; i < m; i++) {
                pow1 = ( (__int128)pow1 * RK_RADIX_1 ) % RK_MOD_1;
                pow2 = ( (__int128)pow2 * RK_RADIX_2 ) % RK_MOD_2;
            }

            if (txtHash == patHash) occ.push_back(0);

            for (int i = 1; i <= n - m; i++) {
                // Remove left character
                txtHash.first = (txtHash.first - (__int128)(unsigned char)text[i - 1] * pow1) % RK_MOD_1;
                txtHash.second = (txtHash.second - (__int128)(unsigned char)text[i - 1] * pow2) % RK_MOD_2;
                if (txtHash.first < 0) txtHash.first += RK_MOD_1;
                if (txtHash.second < 0) txtHash.second += RK_MOD_2;

                // Add right character
                txtHash.first = (( (__int128)txtHash.first * RK_RADIX_1 ) + (unsigned char)text[i + m - 1]) % RK_MOD_1;
                txtHash.second = (( (__int128)txtHash.second * RK_RADIX_2 ) + (unsigned char)text[i + m - 1]) % RK_MOD_2;

                if (txtHash == patHash) occ.push_back(i);
            }
            return occ;
        }
    };
}

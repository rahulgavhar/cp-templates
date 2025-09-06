#include <bits/stdc++.h>
using namespace std;

// Rabin-Karp Implementation with DoubleHashing
// supports double-hashing and reduces probablity of spurious hits drastically to 10e-18 (so works for daily life strings / testcases on coding platforms)

class RABINKARP
{
    static const int RK_RADIX_1 = 31, RK_RADIX_2 = 53, RK_MOD_1 = int(1e9 + 7), RK_MOD_2 = int(1e9 + 9);

public:
    static pair<int, int> hashPair(string str)
    {
        int m = str.length();
        long long hash1 = 0, hash2 = 0, factor1 = 1, factor2 = 1;

        for (int i = m - 1; i >= 0; i--)
        {
            hash1 = (hash1 + ((unsigned char)(str[i]) * factor1) % RK_MOD_1) % RK_MOD_1;
            factor1 = (factor1 * RK_RADIX_1) % RK_MOD_1;

            hash2 = (hash2 + ((unsigned char)(str[i]) * factor2) % RK_MOD_2) % RK_MOD_2;
            factor2 = (factor2 * RK_RADIX_2) % RK_MOD_2;
        }
        return make_pair(hash1 % RK_MOD_1, hash2 % RK_MOD_2);
    }

    static vector<int> rkocc(string &text, string &pat)
    {
        int n = text.length(), m = pat.length(), pow_m = m;
        if (n < m || m == 0)
            return {};

        vector<int> occ;

        long long MAX_WEIGHT_1 = 1, MAX_WEIGHT_2 = 1, MW_FAC_1 = RK_RADIX_1, MW_FAC_2 = RK_RADIX_2;
        while (pow_m > 0)
        {
            if (pow_m & 1)
            {
                MAX_WEIGHT_1 = (MAX_WEIGHT_1 * MW_FAC_1) % RK_MOD_1;
                MAX_WEIGHT_2 = (MAX_WEIGHT_2 * MW_FAC_2) % RK_MOD_2;
            }
            pow_m >>= 1;
            MW_FAC_1 = (MW_FAC_1 * MW_FAC_1) % RK_MOD_1;
            MW_FAC_2 = (MW_FAC_2 * MW_FAC_2) % RK_MOD_2;
        }

        pair<int, int> patHash = hashPair(pat);
        pair<int, int> textHash = hashPair(text.substr(0, m));

        for (int i = 0; i <= n - m; i++)
        {
            if (i > 0)
            {
                textHash.first = (((1LL * textHash.first * RK_RADIX_1) % RK_MOD_1 - (1LL * (unsigned char)text[i - 1] * MAX_WEIGHT_1) % RK_MOD_1 + RK_MOD_1) % RK_MOD_1 + (unsigned char)text[i + m - 1]) % RK_MOD_1;
                textHash.second = (((1LL * textHash.second * RK_RADIX_2) % RK_MOD_2 - (1LL * (unsigned char)text[i - 1] * MAX_WEIGHT_2) % RK_MOD_2 + RK_MOD_2) % RK_MOD_2 + (unsigned char)text[i + m - 1]) % RK_MOD_2;
            }
            if (textHash == patHash)
                occ.push_back(i);
        }

        return occ;
    }
};

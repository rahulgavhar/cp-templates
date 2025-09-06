#include <bits/stdc++.h>
using namespace std;



class HashFunc {
    const int MOD1 = 1e9 + 7, MOD2 = 1e9 + 9, P1 = 131, P2 = 257;

    vector<int> hash1, hash2, pow1, pow2;

public:
    HashFunc(const string &s) {
        int n = s.length();
        if(n==0)
            return;
        hash1.assign(n, 0);
        hash2.assign(n, 0);
        pow1.assign(n, 1);
        pow2.assign(n, 1);

        hash1[0] = (unsigned char)s[0];
        hash2[0] = (unsigned char)s[0];

        for (int i = 1; i < n; i++) {
            pow1[i] = (1LL * pow1[i - 1] * P1) % MOD1;
            pow2[i] = (1LL * pow2[i - 1] * P2) % MOD2;

            hash1[i] = ( (1LL * hash1[i - 1] * P1) + (unsigned char)(s[i]) ) % MOD1;
            hash2[i] = ( (1LL * hash2[i - 1] * P2) + (unsigned char)(s[i]) ) % MOD2;
        }
    }

    pair<int,int> getHash(int l, int r, bool isOneIdx) {
        l = (isOneIdx ? l - 1 : l);
        r = (isOneIdx ? r - 1 : r);

        int h1 = hash1[r], h2 = hash2[r];
        if (l > 0) {
            int len = r - l + 1;
            h1 = (h1 - (1LL * hash1[l - 1] * pow1[len]) % MOD1 + MOD1) % MOD1;
            h2 = (h2 - (1LL * hash2[l - 1] * pow2[len]) % MOD2 + MOD2) % MOD2;
        }
        return {h1, h2};
    }
};
#include <bits/stdc++.h>
using namespace std;

class Sieve
{

    int count_primes_in_range(int L, int R){
        bool prime[R + 1];
        memset(prime, true, sizeof(prime));
        for (int p = 2; p * p <= R; p++)
        {
            if (prime[p])
            {
                for (int i = p * p; i <= R; i += p)
                {
                    prime[i] = false;
                }
            }
        }
        int prime_count = 0;
        for (int p = max(2, L); p <= R; p++)
        {
            if (prime[p])
            {
                prime_count++;
            }
        }
        return prime_count;
    }
};
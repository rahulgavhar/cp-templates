#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{


    /**
     * ================================================================
     *                     Combinatorics Utility Class
     * ================================================================
     *
     *  Overview:
     *  ----------
     *  Comprehensive Competitive Programming toolkit for combinatorics,
     *  modular arithmetic, and number theory. Supports fast computations
     *  of factorials, nCr/nPr, multinomial coefficients, Catalan numbers,
     *  modular exponentiation, modular inverses, GCD/LCM, Euler's Totient,
     *  Chinese Remainder Theorem (CRT), primality testing, and segmented
     *  sieves.
     *
     *  Precomputation:
     *  ---------------
     *  Must call init(n, m) for functions that rely on factorials or
     *  modular inverses. Precomputation is O(n) time and O(n) space.
     *
     *  Functions REQUIRING init(n, m): [Precomputation O(n), Space O(n)]
     *  ----------------------------------------------------------------
     *  - factorial(n)       : O(1) per query | returns n! % m
     *  - invFactorial(n)    : O(1) per query | returns modular inverse of n!
     *  - nCr(n, r)          : O(1) per query | combinatorial n choose r
     *  - nPr(n, r)          : O(1) per query | permutations n P r
     *  - multinomial({k})   : O(m) per query | multinomial coefficient for groups k
     *  - catalan(n)         : O(1) per query | Catalan numbers modulo m
     *  - nCrLarge(n, r, m)  : O(r) per query | nCr without precomputation
     *
     *  Functions INDEPENDENT of init(): [No precomputation needed]
     *  ----------------------------------------------------------------
     *  - gcd(a, b)          : O(log(min(a,b))) | greatest common divisor
     *  - lcm(a, b)          : O(log(min(a,b))) | least common multiple
     *  - phi(n)             : O(sqrt(n)) worst-case | Euler's totient
     *  - CRT(rem, mod)      : O(k * log(mod_i)) | Chinese Remainder Theorem
     *  - powmod(a, b, m)    : O(log b) | modular exponentiation
     *  - modInverse(a, m)   : O(log m) if m prime | modular inverse
     *  - isPrime(n)         : O(log^3 n) | deterministic Miller–Rabin 64-bit
     *  - segmentedSieve(L,R): O((R-L+1) log log R + sqrt(R)) | primes in range
     *
     *  Notes:
     *  ------
     *  - MOD in modular functions is assumed prime for Fermat inverses.
     *  - nCrLarge is suitable for very large n where precomputation is infeasible.
     *  - segmentedSieve efficiently computes primes in a given range [L, R].
     *  - All combinatorial functions return 0 for invalid inputs.
     *
     * ================================================================
     */



    class Combinatorics {
    private:
        static const long long DEFAULT_MOD = 1000000007LL;
        static const int DEFAULT_MAXN = 2000000;

        static vector<long long> fact;
        static vector<long long> invFact;
        static bool initialized;
        static long long currentMod;
        static int currentMaxN;

        // ---------- Internal Modular Power ----------
        static long long modPow(long long a, long long b, long long m) {
            long long res = 1;
            a %= m;
            while (b > 0) {
                if (b & 1) res = (__int128)res * a % m;
                a = (__int128)a * a % m;
                b >>= 1;
            }
            return res;
        }

    public:
        // ---------- Initialization ----------
        static void init(int n = DEFAULT_MAXN, long long m = DEFAULT_MOD) {
            currentMaxN = n;
            currentMod = m;
            fact.assign(n + 1, 1);
            invFact.assign(n + 1, 1);

            for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % m;

            invFact[n] = modPow(fact[n], m - 2, m);
            for (int i = n - 1; i >= 0; i--)
                invFact[i] = invFact[i + 1] * (i + 1) % m;

            initialized = true;
        }

        // ---------- Modular Arithmetic ----------
        static long long modInverse(long long a, long long m = DEFAULT_MOD) {
            return modPow(a, m - 2, m); // valid only if m is prime
        }

        static long long powmod(long long a, long long b, long long m = DEFAULT_MOD) {
            return modPow(a, b, m);
        }

        // ---------- Factorials ----------
        static long long factorial(int n, long long m = DEFAULT_MOD) {
            if (!initialized) init();
            if (n < 0 || n > currentMaxN) return 0;
            return fact[n] % m;
        }

        static long long invFactorial(int n, long long m = DEFAULT_MOD) {
            if (!initialized) init();
            if (n < 0 || n > currentMaxN) return 0;
            return invFact[n] % m;
        }

        // ---------- Combinatorics ----------
        static long long nCr(int n, int r, long long m = DEFAULT_MOD) {
            if (!initialized) init();
            if (r < 0 || r > n || n > currentMaxN) return 0;
            return fact[n] * invFact[r] % m * invFact[n - r] % m;
        }

        static long long nPr(int n, int r, long long m = DEFAULT_MOD) {
            if (!initialized) init();
            if (r < 0 || r > n) return 0;
            return fact[n] * invFact[n - r] % m;
        }

        static long long multinomial(const vector<int>& ks, long long m = DEFAULT_MOD) {
            if (!initialized) init();
            long long sum = 0;
            for (int k : ks) sum += k;
            if (sum > currentMaxN) return 0;
            long long res = fact[sum];
            for (int k : ks) res = res * invFact[k] % m;
            return res;
        }

        static long long catalan(int n, long long m = DEFAULT_MOD) {
            if (!initialized) init();
            long long n2 = (long long)n * 2;
            return nCr(n2, n, m) * modInverse(n + 1, m) % m;
        }

        static long long nCrLarge(long long n, long long r, long long m = DEFAULT_MOD) {
            if (r < 0 || r > n) return 0;
            if (r == 0 || r == n) return 1;
            if (r > n / 2) r = n - r;

            long long res = 1;
            for (long long i = 1; i <= r; i++) {
                long long num_term = (n - i + 1) % m;
                long long inv_den = modInverse(i, m);
                res = ((__int128)res * num_term) % m;
                res = ((__int128)res * inv_den) % m;
            }
            return res;
        }

        // ---------- Number Theory ----------
        static long long gcd(long long a, long long b) {
            return b == 0 ? a : gcd(b, a % b);
        }

        static long long lcm(long long a, long long b) {
            return a / gcd(a, b) * b;
        }

        static long long phi(long long n) {
            long long result = n;
            for (long long p = 2; p * p <= n; p++) {
                if (n % p == 0) {
                    while (n % p == 0) n /= p;
                    result -= result / p;
                }
            }
            if (n > 1) result -= result / n;
            return result;
        }

        static long long CRT(const vector<long long>& rem, const vector<long long>& mod) {
            long long x = 0, prod = 1;
            for (auto m : mod) prod *= m;

            for (size_t i = 0; i < rem.size(); i++) {
                long long pp = prod / mod[i];
                x = (x + rem[i] * modInverse(pp % mod[i], mod[i]) * pp) % prod;
            }
            return (x + prod) % prod;
        }

        static bool isPrime(long long n) {
            if (n < 2) return false;
            for (long long p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
                if (n % p == 0) return n == p;

            long long d = n - 1, s = 0;
            while ((d & 1) == 0) d >>= 1, s++;

            auto check = [&](long long a) {
                if (a % n == 0) return true;
                long long x = modPow(a, d, n);
                if (x == 1 || x == n - 1) return true;
                for (int i = 1; i < s; i++) {
                    x = (__int128)x * x % n;
                    if (x == n - 1) return true;
                }
                return false;
            };

            for (long long a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022})
                if (!check(a)) return false;
            return true;
        }

        static vector<long long> segmentedSieve(long long L, long long R) {
            long long lim = sqrt(R) + 1;
            vector<char> mark(lim + 1, true);
            vector<long long> primes;

            for (long long i = 2; i <= lim; i++) {
                if (mark[i]) {
                    primes.push_back(i);
                    for (long long j = i * i; j <= lim; j += i) mark[j] = false;
                }
            }

            vector<char> isPrimeRange(R - L + 1, true);
            for (long long p : primes) {
                for (long long j = max(p * p, (L + p - 1) / p * p); j <= R; j += p)
                    isPrimeRange[j - L] = false;
            }

            vector<long long> res;
            for (long long i = L; i <= R; i++)
                if (isPrimeRange[i - L] && i > 1) res.push_back(i);
            return res;
        }
    };

    // ---------- Static Variable Definitions ----------
    vector<long long> Combinatorics::fact;
    vector<long long> Combinatorics::invFact;
    bool Combinatorics::initialized = false;
    long long Combinatorics::currentMod = Combinatorics::DEFAULT_MOD;
    int Combinatorics::currentMaxN = Combinatorics::DEFAULT_MAXN;
}

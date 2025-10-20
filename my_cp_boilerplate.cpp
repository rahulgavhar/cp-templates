// -------------------- HEADER FILES --------------------
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// -------------------- TYPE ALIASES --------------------
using ll = long long;
using vi = vector<int>;
using vll = vector<ll>;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
using mii = map<int,int>;
using umii = unordered_map<int,int>;
using si = set<int>;
using usi = unordered_set<int>;

// Policy-Based Data Structures (ordered sets)
template <typename T> 
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template <typename T> 
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;

// -------------------- CONSTANTS --------------------
constexpr ll MOD = 1e9 + 7;
constexpr double PI = 3.14159265358979323846;
constexpr int INF = 1e9;
constexpr ll LINF = 1e18;

// -------------------- MACROS --------------------
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define pb push_back
#define eb emplace_back
#define F first
#define S second

// Loops
#define rep(i,a,b) for(ll i=(a); i<(b); i++)
#define rrep(i,a,b) for(ll i=(b)-1; i>=(a); i--)

// Min/Max helpers
template<typename T> inline void chmin(T &a, T b){ if(b < a) a = b; }
template<typename T> inline void chmax(T &a, T b){ if(b > a) a = b; }


// -------------------- CUSTOM UTILITIES --------------------

// Vector operations
template<typename T> void read_vector(vector<T> &v, int n){ v.resize(n); for(auto &x:v) cin >> x; }
template<typename T> void print_vector(const vector<T> &v){ for(const auto &x:v) cout << x << " "; cout << "\n"; }
template<typename T> T sum_vector(const vector<T> &v){ return accumulate(all(v), T(0)); }
template<typename T> T max_vector(const vector<T> &v){ return *max_element(all(v)); }
template<typename T> T min_vector(const vector<T> &v){ return *min_element(all(v)); }

// Math utilities
ll gcd(ll a, ll b){ return b == 0 ? a : gcd(b, a % b); }
ll lcm(ll a, ll b){ return a / gcd(a,b) * b; }
bool isPerfectSquare(ll x){ if(x < 0) return false; ll sr = sqrt(x); return sr*sr == x; }
string to_upper(string s){ for(auto &c:s) if(c>='a' && c<='z') c -= 32; return s; }
string to_lower(string s){ for(auto &c:s) if(c>='A' && c<='Z') c += 32; return s; }

// Output shortcuts
void yes(){ cout << "YES\n"; }
void no(){ cout << "NO\n"; }
void maybe(){ cout << "MAYBE\n"; }

// -------------------- FAST I/O --------------------
void fast_io(){ ios::sync_with_stdio(false); cin.tie(nullptr); }

// -------------------- Classes / Functions --------------------



















// -------------------- MAIN FUNCTION --------------------
int main(){
    fast_io();

    ll tc = 1;


    // cin >> tc;


    while(tc--){
        
        
        
    }

    return 0;
}

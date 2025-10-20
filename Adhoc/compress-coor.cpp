#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /**
     * CoordinateCompressor<T>
     * ------------------------
     * Compresses values into [0..unique_count-1] while preserving order.
     * Uses gp_hash_table with custom hash for speed and crash-on-error safety.
     *
     * Functions:
     *  - compress(val) -> int              : O(1) avg, returns compressed index; crashes if val not present
     *  - decompress(idx) -> T              : O(1), original value of index; crashes if index invalid
     *  - get_compressed() -> vector<int>   : O(n), compressed array in original input order
     *  - get_sorted_compressed() -> vector<int> : O(n log n), compressed array sorted ascending
     *  - get_sorted_unique_original() -> const vector<T>& : O(1), sorted unique values
     *  - get_frequency_array() -> vector<int> : O(n), frequency of each compressed index
     *  - get_empty_freq_array() -> vector<int> : O(u), zeroed array of size = number of unique values
     *  - get_original() -> const vector<T>& : O(1), reference to original array
     *  - size() -> int                     : O(1), number of unique values
     */




    // Custom hash to avoid collision hacks (critical for long long / large datasets)
    struct custom_hash_cc {
        static uint64_t splitmix64(uint64_t x) {
            x += 0x9e3779b97f4a7c15;
            x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
            x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
            return x ^ (x >> 31);
        }

        size_t operator()(uint64_t x) const {
            static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
            return splitmix64(x + FIXED_RANDOM);
        }
    };

    // Safe gp_hash_table wrapper with crash-on-error philosophy
    template <typename Key, typename Value>
    using HashMap = gp_hash_table<Key, Value, custom_hash_cc>;

    // --------------------------- Coordinate Compressor ---------------------------
    template <typename T>
    class CoordinateCompressor {
    private:
        vector<T> original;           // Stores the original input values
        vector<T> sorted_unique;      // Stores sorted unique values
        HashMap<T, int> compress_map; // Maps original value -> compressed index

    public:
        // Constructor: initializes compressor with input values
        CoordinateCompressor(const vector<T>& values) {
            original = values;

            // Step 1: Sort and remove duplicates to get unique values
            sorted_unique = values;
            sort(sorted_unique.begin(), sorted_unique.end());
            sorted_unique.erase(unique(sorted_unique.begin(), sorted_unique.end()), sorted_unique.end());

            // Step 2: Build gp_hash_table mapping value -> compressed index
            for (int i = 0; i < (int)sorted_unique.size(); i++)
                compress_map[sorted_unique[i]] = i;

            // Memory optimization for large datasets
            original.shrink_to_fit();
            sorted_unique.shrink_to_fit();
        }

        // Compress a single value → returns its compressed index
        // Crashes with runtime error if value not found
        int compress(const T& value) const {
            auto it = compress_map.find(value);
            if (it == compress_map.end()) {
                cerr << "Runtime Error: Value not found in CoordinateCompressor -> " << value << "\n";
                exit(1);
            }
            return it->second;
        }

        // Decompress a compressed index → returns original value
        // Crashes if index is invalid
        T decompress(int index) const {
            if (index < 0 || index >= size()) {
                cerr << "Runtime Error: Invalid index in decompress -> " << index << "\n";
                exit(1);
            }
            return sorted_unique[index];
        }

        // Returns compressed array for the original values in input order
        vector<int> get_compressed() const {
            vector<int> res;
            res.reserve(original.size());
            for (auto& v : original)
                res.push_back(compress(v));
            return res;
        }

        // Returns compressed array sorted in ascending order
        vector<int> get_sorted_compressed() const {
            vector<int> res = get_compressed();
            sort(res.begin(), res.end());
            return res;
        }

        // Returns reference to sorted unique original values
        const vector<T>& get_sorted_unique_original() const { return sorted_unique; }

        // Returns frequency array of compressed values
        vector<int> get_frequency_array() const {
            vector<int> freq(size(), 0);
            for (auto& v : original)
                freq[compress(v)]++;
            return freq;
        }

        // Returns empty frequency array (all zeros) of size = number of unique values
        vector<int> get_empty_freq_array() const {
            return vector<int>(size(), 0);
        }

        // Returns reference to the original input array
        const vector<T>& get_original() const { return original; }

        // Returns the number of unique values
        int size() const { return (int)sorted_unique.size(); }
    };
}
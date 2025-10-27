#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===========================================
        Disjoint Set Union (Union-Find) - Template
        ===========================================

        Functions:
        1. findParent(u)                      -> O(alpha(N))
        2. unionByRank(u, v)                  -> O(alpha(N))
        3. unionBySize(u, v)                  -> O(alpha(N))
        4. isSameSet(u, v)                    -> O(alpha(N))
        5. getSize(u)                         -> O(alpha(N))
        6. getComponentCount()                -> O(1)

        Notes:
        - Path Compression + Union by Rank/Size makes DSU almost O(1)
        - Useful for Kruskal, connectivity queries, cycle detection, grouping
    */

    class DSU {
    private:
        vector<int> parent, rank, size;
        int components;

    public:
        DSU(int n) {
            parent.resize(n);
            rank.assign(n, 0);
            size.assign(n, 1);
            components = n;
            for (int i = 0; i < n; i++) parent[i] = i;
        }

        // Find parent with PATH COMPRESSION
        int findParent(int node) {
            if (parent[node] == node) return node;
            return parent[node] = findParent(parent[node]);
        }

        // UNION by RANK
        void unionByRank(int u, int v) {
            int pu = findParent(u);
            int pv = findParent(v);
            if (pu == pv) return;

            components--;  // merging two components

            if (rank[pu] < rank[pv]) {
                parent[pu] = pv;
            } else if (rank[pv] < rank[pu]) {
                parent[pv] = pu;
            } else {
                parent[pv] = pu;
                rank[pu]++;
            }
        }

        // UNION by SIZE
        void unionBySize(int u, int v) {
            int pu = findParent(u);
            int pv = findParent(v);
            if (pu == pv) return;

            components--;  // merging two components

            if (size[pu] < size[pv]) {
                parent[pu] = pv;
                size[pv] += size[pu];
            } else {
                parent[pv] = pu;
                size[pu] += size[pv];
            }
        }

        // Check if two nodes are in same set
        bool isSameSet(int u, int v) {
            return findParent(u) == findParent(v);
        }

        // Get size of component containing node u
        int getSize(int u) {
            return size[findParent(u)];
        }

        // Number of connected components
        int getComponentCount() {
            return components;
        }
    };
}
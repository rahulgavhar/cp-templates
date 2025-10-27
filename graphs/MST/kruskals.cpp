#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ======================================
        Kruskal's Algorithm (MST using DSU)
        ======================================

        Graph Type     : Undirected + Weighted
        Output         : MST total weight
        Uses DSU       : Yes (for cycle detection)

        Time Complexity  : O(E log E)
        Space Complexity : O(E + V)

        NOTE:
        - Efficient for sparse graphs
        - Reads graph edges directly
    */

    class DSUK {
    private:
        vector<int> parent, rank, size;
        int components;

    public:
        DSUK(int n) {
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

    long long kruskalMST(int n, vector<tuple<int,int,int>> &edges) {
        // edges = {weight, u, v}
        sort(edges.begin(), edges.end()); // sort by weight

        DSUK dsu(n);
        long long mstWeight = 0;
        int edgesUsed = 0;

        for (auto &[wt, u, v] : edges) {
            if (!dsu.isSameSet(u, v)) {
                dsu.unionBySize(u, v);  // OR unionByRank(u, v)
                mstWeight += wt;
                edgesUsed++;
            }
            if (edgesUsed == n - 1) break; // MST completed
        }
        return mstWeight;
    }
}
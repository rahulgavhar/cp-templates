#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================
        Bipartite Graph Check (DFS) — Function Complexities
        ===============================

        1. isBipartite(adj)
        → Time: O(V + E)
        → Space: O(V) for visited + color arrays

        2. dfsColor(node, adj, color)
        → Time: O(V + E) over entire graph
        → Space: O(H) recursion stack
                    = O(V) worst-case (skewed graph)

        Notes:
        - Works on disconnected graphs (handles multiple components)
        - Colors nodes with 2 colors using DFS
        - If a conflict in coloring is found → graph is NOT bipartite
        - Typical use cases:
            * Checking if graph is 2-colorable
            * Detecting odd-length cycles
            * Used in problems involving partitioning or graph coloring
    */

    bool dfsColor(int node, int currentColor, unordered_map<int, list<int>> &adj, vector<int> &color) {
        color[node] = currentColor;

        for (auto &nbr : adj[node]) {
            if (color[nbr] == -1) {
                // Assign opposite color to neighbor
                if (!dfsColor(nbr, 1 - currentColor, adj, color))
                    return false;
            }
            else if (color[nbr] == currentColor) {
                // Same color conflict → not bipartite
                return false;
            }
        }
        return true;
    }

    bool isBipartite(unordered_map<int, list<int>> &adj, int n) {
        vector<int> color(n, -1);  // -1 means uncolored

        // Handle multiple components
        for (int node = 0; node < n; node++) {
            if (color[node] == -1) {
                if (!dfsColor(node, 0, adj, color))
                    return false;
            }
        }
        return true;
    }

}
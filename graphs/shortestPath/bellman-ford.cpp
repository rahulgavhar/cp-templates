#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ======================================================
        Bellman-Ford Algorithm — Handles Negative Weights
        ======================================================

        Purpose:
        - Finds shortest path from source to all nodes
        - Works with NEGATIVE edge weights
        - Detects NEGATIVE WEIGHT CYCLES

        Complexity:
        1. bellmanFord(edges, n, src)
        → Time:  O(V * E)
        → Space: O(V)

        Notes:
        - Works for both directed and undirected graphs
        - Slower than Dijkstra → Use only when negative edges exist
        - Detects negative weight cycle if distance can still relax
    */

    vector<int> bellmanFord(vector<tuple<int,int,int>> &edges, int n, int src) {
        vector<int> dist(n, INT_MAX);
        dist[src] = 0;

        // Relax all edges V-1 times
        for (int i = 1; i < n; i++) {
            bool updated = false;
            for (auto &e : edges) {
                int u, v, wt;
                tie(u, v, wt) = e;

                if (dist[u] != INT_MAX && dist[u] + wt < dist[v]) {
                    dist[v] = dist[u] + wt;
                    updated = true;
                }
            }
            // Optimization: break early if no relaxation
            if (!updated) break;
        }

        // Check for negative weight cycle
        for (auto &e : edges) {
            int u, v, wt;
            tie(u, v, wt) = e;

            if (dist[u] != INT_MAX && dist[u] + wt < dist[v]) {
                cout << "Negative Weight Cycle Detected\n";
                return {}; // return empty if cycle exists
            }
        }

        return dist;
    }
}
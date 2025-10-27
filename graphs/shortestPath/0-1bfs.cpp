#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ============================================
        0/1 BFS — Shortest Path with 0/1 Edge Weights
        ============================================

        Purpose:
        - Finds shortest path in graphs where edge weights are ONLY 0 or 1
        - Faster than Dijkstra → uses deque instead of priority_queue

        Complexity:
        1. zeroOneBFS(adj, n, src)
        → Time:  O(V + E)
        → Space: O(V)

        Notes:
        - Works for both directed and undirected graphs
        - If edge weight = 0 → push_front
        - If edge weight = 1 → push_back
        - Guarantees optimal shortest path
    */

    vector<int> zeroOneBFS(unordered_map<int, vector<pair<int,int>>> &adj, int n, int src) {
        // dist[node] = shortest distance from src to node
        vector<int> dist(n, INT_MAX);
        deque<int> dq;

        dist[src] = 0;
        dq.push_front(src);

        while (!dq.empty()) {
            int node = dq.front();
            dq.pop_front();

            for (auto &edge : adj[node]) {
                int nbr = edge.first;
                int wt = edge.second;  // 0 or 1

                if (dist[node] + wt < dist[nbr]) {
                    dist[nbr] = dist[node] + wt;

                    // Push based on weight
                    if (wt == 0) dq.push_front(nbr);
                    else dq.push_back(nbr);
                }
            }
        }
        return dist;
    }
}
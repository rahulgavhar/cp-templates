#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ============================================
        Dijkstra's Algorithm — Using Min Heap + Visited
        ============================================

        Purpose:
        - Shortest path in graphs with NON-NEGATIVE edge weights
        - Efficient using priority_queue (min heap)

        Complexity:
        1. dijkstra(adj, n, src)
        → Time:  O((V + E) log V)
        → Space: O(V)

        Notes:
        - Works for both directed and undirected graphs
        - Stops reprocessing nodes using a `visited` array
        - Does NOT work with negative weights (use Bellman-Ford instead)
    */

    vector<int> dijkstra(unordered_map<int, vector<pair<int,int>>> &adj, int n, int src) {
        vector<int> dist(n, INT_MAX);          // shortest distance array
        vector<bool> visited(n, false);        // visited array
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

        dist[src] = 0;
        pq.push({0, src}); // {distance, node}

        while (!pq.empty()) {
            auto [d, node] = pq.top();
            pq.pop();

            if (visited[node]) continue;  // Skip if already processed
            visited[node] = true;

            for (auto &edge : adj[node]) {
                int nbr = edge.first;
                int wt = edge.second;

                if (dist[node] + wt < dist[nbr]) {
                    dist[nbr] = dist[node] + wt;
                    pq.push({dist[nbr], nbr});
                }
            }
        }

        return dist;
    }
}
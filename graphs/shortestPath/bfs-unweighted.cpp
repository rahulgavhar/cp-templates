#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ============================================
        Shortest Path in Unweighted Graph — BFS
        ============================================

        1. bfsShortestPath(adj, n, src)
        → Finds shortest distance from src to every node
        → Time:  O(V + E)
        → Space: O(V) for queue + visited + distance

        Notes:
        - Works only for UNWEIGHTED graphs (or all edges weight = 1)
        - Guarantees shortest path because BFS explores level by level
        - Can also be modified to store actual shortest path
    */

    vector<int> bfsShortestPath(unordered_map<int, list<int>> &adj, int n, int src) {
        vector<int> dist(n, INT_MAX);   // store distance from src
        queue<int> q;

        dist[src] = 0;
        q.push(src);

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (auto &nbr : adj[node]) {
                // Visit only if a shorter path is found
                if (dist[nbr] == INT_MAX) {
                    dist[nbr] = dist[node] + 1;
                    q.push(nbr);
                }
            }
        }

        return dist; // contains shortest distances from src
    }

}
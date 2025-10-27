#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ===============================
        BFS Traversal — Function Complexities
        ===============================

        1. bfs(start, adj)
        → Time: O(V + E)
        → Space: O(V)

        Includes:
        - Queue storage O(V)
        - Visited map O(V)
        - Traversal through all edges O(E)
        - Works for directed/undirected graphs

        Notes:
        - Best for shortest path in unweighted graphs
        - Guarantees minimum number of edges from start → node
    */


    vector<int> bfs(int start, unordered_map<int, list<int>>& adj) {
        unordered_map<int, bool> visited;
        queue<int> q;
        vector<int> order;  // stores BFS traversal order

        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            order.push_back(node);

            for (int neigh : adj[node]) {
                if (!visited[neigh]) {
                    visited[neigh] = true;
                    q.push(neigh);
                }
            }
        }
        return order;
    }
}
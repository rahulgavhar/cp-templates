#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================
        Cycle Detection — Undirected Graph (DFS)
        ===============================

        1. hasCycleDFS(adj)
        → Time: O(V + E)
        → Space: O(V) for visited + recursion stack

        2. dfsCycle(node, parent, adj, visited)
        → Time: O(V + E) over entire DFS
        → Space: O(H) where H is recursion depth (≤ V)

        Explanation:
        - Works only for UNDIRECTED graphs.
        - While traversing, if we reach an already visited node
        that is NOT the parent, a cycle exists.
        - DFS ensures all nodes in all connected components are explored.

        Notes:
        - For directed graphs, cycle detection logic is DIFFERENT.
        - This version avoids false cycle detection using `parent` tracking.
    */

    bool dfsCycle(int node, int parent, unordered_map<int, list<int>>& adj, unordered_map<int, bool>& visited) {
        visited[node] = true;

        for (int neigh : adj[node]) {
            if (!visited[neigh]) {
                if (dfsCycle(neigh, node, adj, visited))
                    return true;
            }
            // If neighbor is visited and not parent → cycle
            else if (neigh != parent) {
                return true;
            }
        }
        return false;
    }

    bool hasCycleDFS(unordered_map<int, list<int>>& adj) {
        unordered_map<int, bool> visited;

        for (auto& it : adj) {
            int node = it.first;
            if (!visited[node]) {
                if (dfsCycle(node, -1, adj, visited))
                    return true;
            }
        }
        return false;
    }
}
#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ===============================
        DFS Traversal — Function Complexities
        ===============================

        1. dfs(start, adj)
        → Time: O(V + E)
        → Space: O(V + stack depth)
                = O(V) worst-case

        2. dfsHelper(node, adj, visited, order)
        → Time: O(V + E) when called over whole graph
        → Space: O(H) additional where H = height of DFS recursion
                = O(V) worst-case (skewed graph)

        Notes:
        - Useful for exploring connected components and paths
        - Good for tree processing and edge classification
        - Recursion may cause stack overflow for deep graphs (use iterative DFS if needed)
    */



    // DFS helper function
    void dfsHelper(int node, unordered_map<int, list<int>>& adj, unordered_map<int, bool>& visited, vector<int>& order) {
        visited[node] = true;
        order.push_back(node);

        for (int neigh : adj[node]) {
            if (!visited[neigh]) {
                dfsHelper(neigh, adj, visited, order);
            }
        }
    }

    // DFS traversal from a start node
    vector<int> dfs(int start, unordered_map<int, list<int>>& adj) {
        unordered_map<int, bool> visited;
        vector<int> order; // stores DFS traversal order
        dfsHelper(start, adj, visited, order);
        return order;
    }
}
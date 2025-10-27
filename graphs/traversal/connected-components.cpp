#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ===============================
        Connected Components — BFS Version
        ===============================

        1. connectedComponents(adj)
        → Time: O(V + E)
        → Space: O(V)

        Explanation:
        - Iterates over all nodes to ensure even disconnected components are visited.
        - BFS is started from each unvisited node to explore its full component.
        - Uses a visited map to track visited nodes.
        - Works for both directed and undirected graphs.
        - Returns a vector of components, where each component is stored as a vector<int>.
    */

    vector<vector<int>> connectedComponents(unordered_map<int, list<int>>& adj) {
        unordered_map<int, bool> visited;
        vector<vector<int>> components;

        for (auto& nodePair : adj) {
            int start = nodePair.first;
            if (!visited[start]) {
                vector<int> component;
                queue<int> q;
                q.push(start);
                visited[start] = true;

                while (!q.empty()) {
                    int node = q.front();
                    q.pop();
                    component.push_back(node);

                    for (int neigh : adj[node]) {
                        if (!visited[neigh]) {
                            visited[neigh] = true;
                            q.push(neigh);
                        }
                    }
                }
                components.push_back(component);
            }
        }
        return components;
    }
}
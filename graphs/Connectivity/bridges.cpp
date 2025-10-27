#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ==============================================
        Bridges in Graph (Tarjan's Algorithm - DFS)
        ==============================================
        Graph Type       : Undirected
        Purpose          : Find all Bridges (Critical Edges)
        Why important?   : Used in Network Reliability, SCC, Biconnected Graphs

        Time Complexity  : O(V + E)
        Space Complexity : O(V)
    */

    vector<pair<int,int>> bridges;     // stores bridge edges
    int timer;

    void dfsBridge(int node, int parent,
                unordered_map<int, list<int>> &adj,
                vector<int> &disc, vector<int> &low,
                vector<bool> &visited) {

        visited[node] = true;
        disc[node] = low[node] = timer++;   // discovery time & low time

        for (int nbr : adj[node]) {
            if (nbr == parent) continue;  // skip edge to parent

            if (!visited[nbr]) {
                dfsBridge(nbr, node, adj, disc, low, visited);

                // Update low time after return
                low[node] = min(low[node], low[nbr]);

                // Check Bridge Condition
                if (low[nbr] > disc[node]) {
                    bridges.push_back({node, nbr});
                }
            } else {
                // Back-edge -> update low time
                low[node] = min(low[node], disc[nbr]);
            }
        }
    }

    vector<pair<int,int>> findBridges(int n, unordered_map<int, list<int>> &adj) {
        bridges.clear();
        timer = 0;

        vector<int> disc(n, -1), low(n, -1);
        vector<bool> visited(n, false);

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfsBridge(i, -1, adj, disc, low, visited);
            }
        }
        return bridges;
    }
}
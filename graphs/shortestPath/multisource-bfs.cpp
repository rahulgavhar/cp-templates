#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        =======================================
        Multi-Source BFS (Unweighted Graph)
        =======================================

        Purpose:
        - Used when we have MULTIPLE starting points
        - Example: distance to nearest source, infection spread, fire spread

        Graph Type:
        - Works for both Directed and Undirected (Unweighted)

        Time Complexity:
        - O(V + E)

        Space Complexity:
        - O(V)

        NOTE:
        - Sources are pushed in queue initially with dist = 0
        - BFS expands in all directions simultaneously from every source
    */

    vector<int> multiSourceBFS(int n, unordered_map<int, list<int>> &adj, vector<int> &sources) {
        vector<int> dist(n, INT_MAX);
        queue<int> q;

        // Step 1: Push all sources in queue initially
        for (int src : sources) {
            dist[src] = 0;
            q.push(src);
        }

        // Step 2: Standard BFS
        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (auto &nbr : adj[node]) {
                if (dist[nbr] == INT_MAX) {
                    dist[nbr] = dist[node] + 1;
                    q.push(nbr);
                }
            }
        }

        return dist;
    }

}
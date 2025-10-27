#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ======================================================
        Articulation Points (Cut Vertices) - Tarjan's Algorithm
        ======================================================

        Graph Type       : Undirected
        Purpose          : Find all Articulation Points
        Why important?   : Network Reliability, Biconnected Components

        Time Complexity  : O(V + E)
        Space Complexity : O(V)
    */

    vector<int> articulationPoints;
    int timerAP;

    void dfsAP(int node, int parent,
            unordered_map<int, list<int>> &adj,
            vector<int> &disc, vector<int> &low,
            vector<bool> &visited, vector<bool> &isAP) {

        visited[node] = true;
        disc[node] = low[node] = timerAP++;
        int childCount = 0; // track children for root

        for (auto nbr : adj[node]) {
            if (nbr == parent) continue;

            if (!visited[nbr]) {
                childCount++;
                dfsAP(nbr, node, adj, disc, low, visited, isAP);

                // update low time
                low[node] = min(low[node], low[nbr]);

                // Articulation Point Case 1: Non-root
                if (parent != -1 && low[nbr] >= disc[node]) {
                    isAP[node] = true;
                }
            }
            else {
                // Back edge
                low[node] = min(low[node], disc[nbr]);
            }
        }

        // Articulation Point Case 2: Root node
        if (parent == -1 && childCount > 1) {
            isAP[node] = true;
        }
    }

    vector<int> findArticulationPoints(int n, unordered_map<int, list<int>> &adj) {
        timerAP = 0;
        articulationPoints.clear();

        vector<int> disc(n, -1), low(n, -1);
        vector<bool> visited(n, false), isAP(n, false);

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfsAP(i, -1, adj, disc, low, visited, isAP);
            }
        }

        for (int i = 0; i < n; i++) {
            if (isAP[i]) articulationPoints.push_back(i);
        }
        return articulationPoints;
    }

}
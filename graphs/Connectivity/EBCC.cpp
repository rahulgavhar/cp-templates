#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ======================================================
        Biconnected Components (BCC) - Tarjan's Algorithm
        ======================================================

        Graph Type        : Undirected
        Handles           : Multiple Components
        Output            : Each BCC as group of edges
        Use Cases         : Network reliability, articulation problems,
                            block-cut tree, decomposition

        Time Complexity   : O(V + E)
        Space Complexity  : O(V + E)
    */

    vector<vector<pair<int,int>>> BCC;          // Stores all Biconnected Components
    stack<pair<int,int>> edgeStack;             // Stack to store edges
    int timerBCC;

    void dfsBCC(int node, int parent,
                unordered_map<int, list<int>> &adj,
                vector<int> &disc, vector<int> &low,
                vector<bool> &visited) {

        visited[node] = true;
        disc[node] = low[node] = timerBCC++;

        for (int nbr : adj[node]) {
            if (nbr == parent) continue;

            if (!visited[nbr]) {
                // Push edge into stack
                edgeStack.push({node, nbr});
                dfsBCC(nbr, node, adj, disc, low, visited);

                // Update low time
                low[node] = min(low[node], low[nbr]);

                // BCC CONDITION
                if (low[nbr] >= disc[node]) {
                    vector<pair<int,int>> component;
                    while (!edgeStack.empty() && edgeStack.top() != make_pair(node, nbr)) {
                        component.push_back(edgeStack.top());
                        edgeStack.pop();
                    }
                    component.push_back(edgeStack.top());
                    edgeStack.pop();
                    BCC.push_back(component);
                }
            }
            else if (disc[nbr] < disc[node]) {
                // Back edge
                low[node] = min(low[node], disc[nbr]);
                edgeStack.push({node, nbr});
            }
        }
    }

    vector<vector<pair<int,int>>> findBCC(int n, unordered_map<int, list<int>> &adj) {
        BCC.clear();
        while (!edgeStack.empty()) edgeStack.pop();
        timerBCC = 0;

        vector<int> disc(n, -1), low(n, -1);
        vector<bool> visited(n, false);

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfsBCC(i, -1, adj, disc, low, visited);

                // Empty remaining edges for last component
                if (!edgeStack.empty()) {
                    vector<pair<int,int>> component;
                    while (!edgeStack.empty()) {
                        component.push_back(edgeStack.top());
                        edgeStack.pop();
                    }
                    BCC.push_back(component);
                }
            }
        }

        return BCC;
    }

}
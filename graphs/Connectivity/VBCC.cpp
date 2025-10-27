#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================================
        VBCC - Vertex Biconnected Components (Tarjan)
        ===============================================

        Graph Type  : Undirected
        Returns     : Set of vertex-level biconnected components
        Note        : Uses articulation points
    */

    void vbccDFS(int node, int parent,
                unordered_map<int, vector<int>>& adj,
                vector<int>& disc, vector<int>& low,
                vector<bool>& visited, stack<int>& st,
                vector<vector<int>>& components, int& timer) {

        visited[node] = true;
        disc[node] = low[node] = timer++;
        st.push(node);

        int childCount = 0;
        for (int nbr : adj[node]) {
            if (nbr == parent) continue;

            if (!visited[nbr]) {
                childCount++;
                vbccDFS(nbr, node, adj, disc, low, visited, st, components, timer);

                low[node] = min(low[node], low[nbr]);

                // Articulation point condition
                if ((parent != -1 && low[nbr] >= disc[node]) ||
                    (parent == -1 && childCount > 1)) {
                    vector<int> component;
                    while (!st.empty() && st.top() != nbr) {
                        component.push_back(st.top());
                        st.pop();
                    }
                    component.push_back(nbr);
                    component.push_back(node);
                    components.push_back(component);
                }

            } else {
                // Back edge
                low[node] = min(low[node], disc[nbr]);
            }
        }
    }

    vector<vector<int>> vertexBiconnectedComponents(int n,
                        unordered_map<int, vector<int>>& adj) {
        vector<int> disc(n, -1), low(n, -1);
        vector<bool> visited(n, false);
        stack<int> st;
        vector<vector<int>> components;
        int timer = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                vbccDFS(i, -1, adj, disc, low, visited, st, components, timer);
                if (!st.empty()) {
                    vector<int> comp;
                    while (!st.empty()) {
                        comp.push_back(st.top());
                        st.pop();
                    }
                    components.push_back(comp);
                }
            }
        }
        return components;
    }

}
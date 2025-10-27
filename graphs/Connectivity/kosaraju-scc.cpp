#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================================
        Kosaraju Algorithm - Strongly Connected Components
        ===============================================

        Graph Type  : Directed
        Output      : List of SCCs (vector<vector<int>>)
        Complexity  : O(V + E)
    */

    void topoDFS(int node,
                unordered_map<int, vector<int>>& adj,
                vector<bool>& visited,
                stack<int>& st) {
        visited[node] = true;
        for (auto nbr : adj[node]) {
            if (!visited[nbr]) {
                topoDFS(nbr, adj, visited, st);
            }
        }
        st.push(node);
    }

    void revDFS(int node,
                unordered_map<int, vector<int>>& adjT,
                vector<bool>& visited,
                vector<int>& component) {
        visited[node] = true;
        component.push_back(node);
        for (auto nbr : adjT[node]) {
            if (!visited[nbr]) {
                revDFS(nbr, adjT, visited, component);
            }
        }
    }

    vector<vector<int>> kosarajuSCC(int n,
            unordered_map<int, vector<int>>& adj) {

        // Step 1: Topo order using original graph
        vector<bool> visited(n, false);
        stack<int> st;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                topoDFS(i, adj, visited, st);
            }
        }

        // Step 2: Reverse graph
        unordered_map<int, vector<int>> adjT;
        for (int i = 0; i < n; i++) {
            for (auto nbr : adj[i]) {
                adjT[nbr].push_back(i);
            }
        }

        // Step 3: DFS using reverse graph in order of stack
        fill(visited.begin(), visited.end(), false);
        vector<vector<int>> scc;

        while (!st.empty()) {
            int node = st.top();
            st.pop();

            if (!visited[node]) {
                vector<int> component;
                revDFS(node, adjT, visited, component);
                scc.push_back(component);
            }
        }

        return scc;
    }
}
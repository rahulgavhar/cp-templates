#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================
        Topological Sort (DFS)
        ===============================
        Graph Type : Directed Acyclic Graph (DAG)
        Method     : DFS + Stack
        Time       : O(V + E)
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

    vector<int> topoSortDFS(int n,
                unordered_map<int, vector<int>>& adj) {
        vector<bool> visited(n, false);
        stack<int> st;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                topoDFS(i, adj, visited, st);
            }
        }

        vector<int> topo;
        while (!st.empty()) {
            topo.push_back(st.top());
            st.pop();
        }
        return topo;
    }

}
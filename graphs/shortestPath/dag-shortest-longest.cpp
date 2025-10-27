#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ======================================================
        DAG Shortest & Longest Path (Topological + Relaxation)
        ======================================================

        Works only for: Directed Acyclic Graphs (DAG)
        Graph Type    : Weighted / Unweighted
        Supports      : Shortest Path + Longest Path
        Why fast?     : O(V + E) with topo ordering

        mode = 0 → shortest path
        mode = 1 → longest path
    */

    void topoDFS(int node, unordered_map<int, vector<pair<int,int>>> &adj, vector<bool> &visited, stack<int> &st) {
        visited[node] = true;
        for (auto &edge : adj[node]) {
            int nbr = edge.first;
            if (!visited[nbr]) {
                topoDFS(nbr, adj, visited, st);
            }
        }
        st.push(node);
    }

    vector<long long> dagPath(int n, int src, unordered_map<int, vector<pair<int,int>>> &adj, bool mode) {
        // mode = 0 -> shortest path
        // mode = 1 -> longest path

        const long long INF = 1e18;
        vector<long long> dist(n, mode ? -INF : INF);
        dist[src] = 0;

        // Step 1: Topological Sort
        vector<bool> visited(n, false);
        stack<int> st;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) topoDFS(i, adj, visited, st);
        }

        // Step 2: Relax nodes in topo order
        while (!st.empty()) {
            int node = st.top();
            st.pop();

            if (dist[node] != (mode ? -INF : INF)) {
                for (auto &edge : adj[node]) {
                    int nbr = edge.first;
                    int wt = edge.second;

                    if (mode == 0) {
                        // Shortest path relaxation
                        if (dist[node] + wt < dist[nbr]) {
                            dist[nbr] = dist[node] + wt;
                        }
                    } else {
                        // Longest path relaxation
                        if (dist[node] + wt > dist[nbr]) {
                            dist[nbr] = dist[node] + wt;
                        }
                    }
                }
            }
        }

        return dist;
    }
}
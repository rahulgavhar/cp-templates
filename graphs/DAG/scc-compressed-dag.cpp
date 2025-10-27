#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ============================================================
        SCCCompressor Class — SCC Condensed DAG using Kosaraju
        ============================================================

        Purpose:
        Compress a directed graph into a Directed Acyclic Graph (DAG)
        where each node represents a Strongly Connected Component (SCC).

        Graph Type    : Directed
        Handles Cycles: Yes (compresses cycles into single nodes)
        Output        : Condensed DAG + SCC groups + Component IDs
        Uses          : Kosaraju's Algorithm (2-pass DFS)

        Time Complexity:
        - computeSCC()          → O(V + E)
        - buildCondensedDAG()   → O(V + E)

        Space Complexity:
        - O(V + E)

        Useful For:
        - DAG DP after SCC compression
        - Reachability in directed graphs
        - Solving 2-SAT
        - Maximum path on condensation graph
    */

    class SCCCompressor {
    private:
        int n;
        unordered_map<int, vector<int>> adj;
        unordered_map<int, vector<int>> adjT;
        vector<bool> visited;
        stack<int> st;

        void topoDFS(int node) {
            visited[node] = true;
            for (auto nbr : adj[node]) {
                if (!visited[nbr]) topoDFS(nbr);
            }
            st.push(node);
        }

        void reverseDFS(int node, vector<int>& component) {
            visited[node] = true;
            component.push_back(node);
            for (auto nbr : adjT[node]) {
                if (!visited[nbr]) reverseDFS(nbr, component);
            }
        }

    public:
        vector<int> compId;    // Component ID for each node
        vector<vector<int>> scc;   // List of SCC groups

        SCCCompressor(int n) {
            this->n = n;
            visited.resize(n, false);
            compId.resize(n, -1);
        }

        void addEdge(int u, int v) {
            adj[u].push_back(v);
            adjT[v].push_back(u); // Build reverse graph simultaneously
        }

        // Step 1 & 2: Run Kosaraju and find SCCs
        void computeSCC() {
            // 1. Topo ordering
            for (int i = 0; i < n; i++) {
                if (!visited[i]) topoDFS(i);
            }

            // 2. Reverse DFS to collect SCCs
            fill(visited.begin(), visited.end(), false);

            while (!st.empty()) {
                int node = st.top();
                st.pop();
                if (!visited[node]) {
                    vector<int> component;
                    reverseDFS(node, component);
                    scc.push_back(component);
                }
            }

            // 3. Assign component IDs
            for (int id = 0; id < (int)scc.size(); id++) {
                for (int node : scc[id]) {
                    compId[node] = id;
                }
            }
        }

        // Build SCC condensed graph (DAG)
        vector<vector<int>> buildCondensedDAG() {
            int sccCount = scc.size();
            vector<vector<int>> dag(sccCount);
            unordered_set<long long> edgeSet;

            for (int u = 0; u < n; u++) {
                for (auto v : adj[u]) {
                    if (compId[u] != compId[v]) {
                        long long hash = 1LL * compId[u] * n + compId[v];
                        if (!edgeSet.count(hash)) {
                            edgeSet.insert(hash);
                            dag[compId[u]].push_back(compId[v]);
                        }
                    }
                }
            }
            return dag;
        }
    };

}
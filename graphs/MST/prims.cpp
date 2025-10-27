#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ======================================
        Prim's Algorithm (MST using Min-Heap)
        ======================================

        Graph Type     : Undirected + Weighted
        Output         : MST total weight + parent array
        Priority Queue : Min-Heap
        Safe for CF    : Yes

        Time Complexity  : O(E log V)
        Space Complexity : O(V)

        NOTE:
        - Works only for connected components. For disconnected graph, call Prim per component.
        - parent[] gives MST structure
    */

    long long primMST(int n, unordered_map<int, list<pair<int,int>>> &adj) {
        // Min-heap: (weight, node)
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

        vector<bool> inMST(n, false);
        vector<int> parent(n, -1);
        vector<long long> key(n, LLONG_MAX);

        long long totalWeight = 0;

        // Start from node 0 (or any node)
        key[0] = 0;
        pq.push({0, 0});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (inMST[u]) continue;  // already processed
            inMST[u] = true;
            totalWeight += key[u];

            for (auto &edge : adj[u]) {
                int v = edge.first;
                int wt = edge.second;

                if (!inMST[v] && wt < key[v]) {
                    key[v] = wt;
                    parent[v] = u;
                    pq.push({key[v], v});
                }
            }
        }

        return totalWeight;  // Sum of weights in MST
    }
}
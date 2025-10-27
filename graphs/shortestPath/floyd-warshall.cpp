#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ======================================================
        Floyd–Warshall Algorithm — All Pairs Shortest Path
        Self-contained version (initializes dist inside)
        ======================================================

        edges format: vector of {u, v, w}
                    (directed edge u -> v with weight w)

        Complexity:
        Time:  O(V^3)
        Space: O(V^2)
    */

    vector<vector<long long>> floydWarshall(int n, vector<tuple<int,int,long long>> &edges) {
        const long long INF = 1e18;

        // Step 1: Initialize dist matrix
        vector<vector<long long>> dist(n, vector<long long>(n, INF));
        for (int i = 0; i < n; i++) dist[i][i] = 0;

        // Step 2: Fill edges
        for (auto &e : edges) {
            int u, v;
            long long w;
            tie(u, v, w) = e;
            dist[u][v] = min(dist[u][v], w);  // In case of multiple edges
        }

        // Step 3: Core Floyd–Warshall Relaxation
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                if (dist[i][k] == INF) continue;
                for (int j = 0; j < n; j++) {
                    if (dist[k][j] == INF) continue;
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }

        // Step 4: Detect Negative Cycles
        for (int i = 0; i < n; i++) {
            if (dist[i][i] < 0) {
                cout << "Negative Weight Cycle Detected\n";
                return {};
            }
        }

        return dist;
    }

}
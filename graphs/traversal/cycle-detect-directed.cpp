#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================================
        Cycle Detection in Directed Graph — DFS Method
        ===============================================

        Detects if there is a cycle in a directed graph using DFS
        + recursion stack tracking (also called pathVis array)

        Function Complexities:
        ----------------------
        detectCycleDFS(node, adj, vis, pathVis)
        → Time:  O(V + E)     (standard DFS traversal)
        → Space: O(V)         (visited + pathVisited + recursion stack)

        hasCycleDirected(adj)
        → Time:  O(V + E)     over all components
        → Space: O(V)

        Notes:
        ------
        - Works only for DIRECTED graphs
        - Uses recursion stack to detect back edges
        - If vis[node] == 1 → already processed fully, no need to recheck
        - If pathVis[node] == 1 and we visit it again → cycle detected
    */

    bool detectCycleDFS(int node, unordered_map<int, list<int>>& adj, unordered_map<int, bool>& vis, unordered_map<int, bool>& pathVis){
        // mark current node visited
        vis[node] = true;
        pathVis[node] = true;   // part of current recursion path

        // explore neighbors
        for (auto& nbr : adj[node]) {
            if (!vis[nbr]) {
                if (detectCycleDFS(nbr, adj, vis, pathVis))
                    return true;
            }
            // if the neighbor is visited AND also in current recursion path => CYCLE
            else if (pathVis[nbr]) {
                return true;
            }
        }

        // remove node from current path
        pathVis[node] = false;
        return false;
    }

    bool hasCycleDirected(unordered_map<int, list<int>>& adj){
        unordered_map<int, bool> vis;
        unordered_map<int, bool> pathVis;

        // run DFS for all components
        for (auto& it : adj) {
            int node = it.first;
            if (!vis[node]) {
                if (detectCycleDFS(node, adj, vis, pathVis))
                    return true;
            }
        }
        return false;
    }

}
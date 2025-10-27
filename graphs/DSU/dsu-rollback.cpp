#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ============================================
        DSU (Disjoint Set Union) with Rollback
        ============================================

        Supports:
        - union(a, b)         → merges components
        - find(a)             → finds leader (no path compression)
        - snapshot()          → save rollback point
        - rollback()          → revert to last snapshot

        Features:
        - Rollback uses a stack of changes
        - No path compression (important!)
        - Union by size/rank supported
        - Used for offline queries and divide & conquer DP

        Time: Amortized ~O(α(N)) per operation
    */

    struct DSURollback {
        vector<int> parent, sz;
        stack<pair<int,int>> history; // stores (node, old_parent_or_size)
        int components;

        DSURollback(int n) {
            parent.resize(n);
            sz.assign(n, 1);
            components = n;
            for (int i = 0; i < n; i++) parent[i] = i;
        }

        int find(int x) {
            while (x != parent[x]) x = parent[x]; // no path compression
            return x;
        }

        bool unite(int a, int b) {
            a = find(a), b = find(b);
            if (a == b) {
                history.push({-1, -1}); // nothing changed
                return false;
            }
            // Union by size
            if (sz[a] < sz[b]) swap(a, b);

            history.push({b, sz[a]}); // record change
            parent[b] = a;
            sz[a] += sz[b];
            components--;
            return true;
        }

        void rollback() {
            auto last = history.top();
            history.pop();

            if (last.first == -1) return; // no change to revert

            int b = last.first;
            int oldSize = last.second;
            int a = parent[b];

            parent[b] = b;         // restore parent
            sz[a] = oldSize;       // restore size
            components++;
        }

        int snapshot() {
            return history.size(); // save rollback checkpoint
        }

        void rollbackTo(int checkpoint) {
            while (history.size() > checkpoint) rollback();
        }
    };

}
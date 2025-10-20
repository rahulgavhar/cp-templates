#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ===============================
        HLD Class — Function Complexities
        ===============================

        1. addEdge(u, v, w = 0)         → Adds an edge between nodes u and v with optional weight w | Time: O(1) | Space: O(1)
        2. setNodeValue(u, val)         → Sets the value of node u | Time: O(1) | Space: O(1)
        3. init(root = 0)               → Runs DFS + decomposition from root | Time: O(N) | Space: O(N)
        4. dfs(u, p)                    → Computes parent, depth, heavy child, and subtree sizes | Time: O(N) | Space: O(N)
        5. decompose(u, h)              → Flattens nodes into chains for HLD | Time: O(N) | Space: O(N)
        6. flattenEdges(root = 0)       → Flattens edge weights into the flat array | Time: O(N) | Space: O(N)
        7. getFlat()                     → Returns the flattened node/edge array | Time: O(1) | Space: O(1)
        8. getPos(u)                     → Returns the flattened position of node u | Time: O(1) | Space: O(1)
        9. getChainHead(u)               → Returns the chain head of node u | Time: O(1) | Space: O(1)
        10. getParent(u)                 → Returns the parent of node u | Time: O(1) | Space: O(1)
        11. getDepth(u)                  → Returns the depth of node u | Time: O(1) | Space: O(1)
        12. getHeavyChild(u)             → Returns the heavy child of node u | Time: O(1) | Space: O(1)
        13. getSubtreeSize(u)            → Returns the size of the subtree rooted at node u | Time: O(1) | Space: O(1)

        Notes:
        - N = number of nodes in the tree
        - All internal arrays are 0-based; getters return 1-based values if oneBased = true
        - Suitable for node and edge value queries, path queries, and chain decomposition
        - Flattened array can be used with segment trees for efficient path/range queries

    */




    class HLD {
    private:
        int N, timer;
        bool oneBased; // true if input nodes are 1-based

        struct Edge {
            int to, weight;
        };

        vector<vector<Edge>> g;
        vector<int> parent, depth, heavy, head, pos, subtree;
        vector<int> nodeValue, edgeValue; // node & edge values
        vector<int> flat;

        int dfs(int u, int p) {
            parent[u] = p;
            subtree[u] = 1;
            int max_size = 0;

            for (auto e : g[u]) {
                int v = e.to;
                if (v == p) continue;

                depth[v] = depth[u] + 1;
                edgeValue[v] = e.weight;

                int sz = dfs(v, u);
                subtree[u] += sz;

                if (sz > max_size) {
                    max_size = sz;
                    heavy[u] = v;
                }
            }
            return subtree[u];
        }

        void decompose(int u, int h) {
            head[u] = h;
            pos[u] = timer++;
            flat[pos[u]] = nodeValue[u]; // store node values by default

            if (heavy[u] != -1) decompose(heavy[u], h);

            for (auto e : g[u]) {
                int v = e.to;
                if (v != parent[u] && v != heavy[u]) {
                    decompose(v, v);
                }
            }
        }

        int convert(int u) const {
            // Convert user input to 0-based internally
            return oneBased ? u - 1 : u;
        }

    public:
        HLD(int n, bool oneBasedInput = false) : N(n), timer(0), oneBased(oneBasedInput) {
            g.assign(N, {});
            parent.assign(N, -1);
            depth.assign(N, 0);
            heavy.assign(N, -1);
            head.assign(N, -1);
            pos.assign(N, 0);
            subtree.assign(N, 0);
            nodeValue.assign(N, 0);
            edgeValue.assign(N, 0);
            flat.assign(N, 0);
        }

        void addEdge(int u, int v, int w = 0) {
            u = convert(u); v = convert(v);
            g[u].push_back({v, w});
            g[v].push_back({u, w});
        }

        void setNodeValue(int u, int val) {
            u = convert(u);
            nodeValue[u] = val;
        }

        void init(int root = 0) {
            root = convert(root);
            dfs(root, -1);
            decompose(root, root);
            edgeValue[root] = 0; // root has no parent edge
        }

        const vector<int>& getFlat() const { return flat; }

        void flattenEdges(int root = 0) {
            root = convert(root);
            for (int i = 0; i < N; ++i) flat[pos[i]] = edgeValue[i];
            flat[pos[root]] = 0;
        }

        // Getter functions
        int getPos(int u) const { return pos[convert(u)]; }
        int getChainHead(int u) const { return head[convert(u)] + (oneBased ? 1 : 0); }
        int getParent(int u) const { 
            int p = parent[convert(u)];
            return (p == -1 ? -1 : p + (oneBased ? 1 : 0));
        }
        int getHeavyChild(int u) const { 
            int h = heavy[convert(u)];
            return (h == -1 ? -1 : h + (oneBased ? 1 : 0));
        }
        int getDepth(int u) const { return depth[convert(u)]; }
        int getSubtreeSize(int u) const { return subtree[convert(u)]; }
    };
}

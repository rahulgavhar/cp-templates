#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================
        CentroidDecomposition — Function Complexities
        ===============================

        Constructor:
            CentroidDecomposition(n, oneBased)    → O(n) | Initializes tree and centroid structures

        Tree Operations:
            addEdge(u, v)                         → O(1) | Adds edge to tree
            computeOriginalSubtreeSizes(root)     → O(n) | Computes original subtree sizes

        Centroid Decomposition:
            decompose(u, p=-1, depth=0)          → O(n log n) | Builds centroid tree recursively
            initPaths()                            → O(n log n) | Computes centroid paths

        Helpers:
            dfsSubSize(u, p)                       → O(subtree) | Computes subtree size
            findCentroid(u, p, size)               → O(subtree) | Finds centroid of subtree
            buildCentroidPaths(u, p, dist, root)   → O(subtree) | Builds centroid-path distances

        Getters:
            getParentCentroid()                     → O(n) | Returns parent in centroid tree
            getDepthInCT()                          → O(n) | Returns depth in centroid tree
            getSubtreeSizes()                       → O(n) | Returns current subtree sizes
            getOriginalSubtreeSizes()               → O(n) | Returns original subtree sizes
            getCentroidPaths()                      → O(n) | Returns centroid paths for all nodes
    */


    class CentroidDecomposition {
    public:
        int n;                                      // Number of nodes
        bool isOneBased;                            // True if input edges are 1-based
        vector<vector<int>> tree;                   // Original tree
        vector<bool> isCentroid;                    // Marks whether node is already a centroid
        vector<int> subSize;                        // Subtree sizes for centroid decomposition
        vector<int> origSubSize;                    // Stores original subtree sizes
        vector<int> parentCentroid;                 // Parent of each node in centroid tree
        vector<int> depthInCT;                      // Depth of each node in centroid tree
        vector<vector<pair<int,int>>> centroidPath; // List of {centroid, distance} for each node

        // Constructor: initializes all arrays and sets input type
        CentroidDecomposition(int nodes, bool oneBased = true) {
            n = nodes;
            isOneBased = oneBased;
            tree.assign(n, vector<int>());
            isCentroid.assign(n, false);
            subSize.assign(n, 0);
            origSubSize.assign(n, 0);
            parentCentroid.assign(n, -1);
            depthInCT.assign(n, 0);
            centroidPath.assign(n, vector<pair<int,int>>());
        }

        // Adds an edge to the tree; automatically converts to 0-based if necessary
        void addEdge(int u, int v) {
            if (isOneBased) u--, v--;
            tree[u].push_back(v);
            tree[v].push_back(u);
        }

    private:
        // Computes the size of each subtree recursively
        void dfsSubSize(int u, int p) {
            subSize[u] = 1;
            for (int v : tree[u]) {
                if (v != p && !isCentroid[v]) {
                    dfsSubSize(v, u);
                    subSize[u] += subSize[v];
                }
            }
        }

        // Finds the centroid of a subtree rooted at node u
        int findCentroid(int u, int p, int n) {
            for (int v : tree[u]) {
                if (v != p && !isCentroid[v] && subSize[v] > n/2)
                    return findCentroid(v, u, n);
            }
            return u;
        }

        // Builds centroid paths and distances for each node recursively
        void buildCentroidPaths(int u, int p, int dist, int root) {
            centroidPath[u].push_back({root, dist});
            for (int v : tree[u]) {
                if (v != p && !isCentroid[v])
                    buildCentroidPaths(v, u, dist + 1, root);
            }
        }

    public:
        // Public wrapper to compute original subtree sizes before decomposition
        void computeOriginalSubtreeSizes(int root = 0) {
            dfsSubSize(root, -1);
            origSubSize = subSize;
        }

        // Performs centroid decomposition and stores parent and depth in centroid tree
        int decompose(int u, int p = -1, int depth = 0) {
            dfsSubSize(u, -1);
            int c = findCentroid(u, -1, subSize[u]);
            isCentroid[c] = true;
            parentCentroid[c] = p;
            depthInCT[c] = depth;

            for (int v : tree[c]) {
                if (!isCentroid[v])
                    decompose(v, c, depth + 1);
            }

            return c;
        }

        // Initializes centroid paths for all nodes after decomposition
        void initPaths() {
            fill(isCentroid.begin(), isCentroid.end(), false);
            queue<int> q;
            int root = -1;
            for (int i = 0; i < n; i++) {
                if (parentCentroid[i] == -1) { root = i; break; }
            }
            if (root == -1) return;

            q.push(root);
            while (!q.empty()) {
                int c = q.front(); q.pop();
                isCentroid[c] = true;
                buildCentroidPaths(c, -1, 0, c);
                for (int v : tree[c]) {
                    if (!isCentroid[v])
                        q.push(v);
                }
            }
            fill(isCentroid.begin(), isCentroid.end(), false);
        }

        // Getters
        vector<int> getParentCentroid() { return parentCentroid; }
        vector<int> getDepthInCT() { return depthInCT; }
        vector<int> getSubtreeSizes() { return subSize; }
        vector<int> getOriginalSubtreeSizes() { return origSubSize; }
        vector<vector<pair<int,int>>> getCentroidPaths() { return centroidPath; }
    };
}

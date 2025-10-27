#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ====================================================
        2-SAT Solver using Kosaraju (Implication Graph)
        ====================================================

        Variables range: 0 to n-1
        For each variable x:
            Node 2*x     -> x   (True)
            Node 2*x + 1 -> ~x  (False)

        Supports:
        - Adding clauses: (x OR y)
        - Equivalence, XOR, Implication
        - Checks satisfiability

        Time Complexity: O((n + m) * α(n))
    */

    class TwoSAT {
    private:
        int n;  // number of variables
        vector<vector<int>> adj, adjRev;
        vector<int> order, comp;
        vector<bool> visited;

        // Mapping literal to graph node index
        int var(int x) {
            return x << 1;
        }
        int neg(int x) {
            return x ^ 1;
        }

        // DFS for order
        void dfs1(int node) {
            visited[node] = true;
            for (int nxt : adj[node]) {
                if (!visited[nxt]) dfs1(nxt);
            }
            order.push_back(node);
        }

        // DFS on reversed graph to assign components
        void dfs2(int node, int cid) {
            comp[node] = cid;
            for (int nxt : adjRev[node]) {
                if (comp[nxt] == -1) dfs2(nxt, cid);
            }
        }

    public:
        TwoSAT(int n) : n(n) {
            adj.resize(2 * n);
            adjRev.resize(2 * n);
            comp.assign(2 * n, -1);
            visited.assign(2 * n, false);
        }

        // Add implication: a => b
        void implies(int a, int b) {
            adj[a].push_back(b);
            adjRev[b].push_back(a);
        }

        // Add clause: (x OR y)
        // x and y represent literal indices, where literals use:
        // var(i) = i*2    represents x_i
        // neg(var(i)) = i*2^1 represents ~x_i
        void addOR(int x, int y) {
            implies(neg(x), y);
            implies(neg(y), x);
        }

        // Clause: x == y  (x is equivalent to y)
        void addEqual(int x, int y) {
            addOR(x, neg(y));
            addOR(neg(x), y);
        }

        // Clause: x XOR y (Exactly one true)
        void addXOR(int x, int y) {
            addOR(x, y);
            addOR(neg(x), neg(y));
        }

        // Clause: x => y
        void addImplication(int x, int y) {
            addOR(neg(x), y);
        }

        // Solve the 2-SAT problem
        bool solve(vector<bool> &ans) {
            // 1. Order vertices by DAG finish time
            for (int i = 0; i < 2 * n; i++) {
                if (!visited[i]) dfs1(i);
            }

            // 2. Reverse order traversal
            int cid = 0;
            for (int i = 2 * n - 1; i >= 0; i--) {
                int v = order[i];
                if (comp[v] == -1) dfs2(v, cid++);
            }

            // 3. Check for contradiction
            ans.resize(n);
            for (int i = 0; i < n; i++) {
                if (comp[var(i)] == comp[neg(var(i))]) {
                    return false; // UNSAT
                }
                ans[i] = (comp[var(i)] > comp[neg(var(i))]);
            }
            return true;
        }
    };
}
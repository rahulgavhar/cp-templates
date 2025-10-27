#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        =====================================================
        Offline Dynamic Connectivity (Full Edge Upgrade)
        =====================================================

        - Uses: Segment tree over time + Rollback DSU
        - Edge struct: supports u, v, w, id (future MST/weight support)
        - API:
            addEdge(u, v, w, start, end, id=-1)   // add edge active in [start,end]
            addQuery(u, v, time)                  // add connectivity query at time
            buildAndSolve()                       // run solver for time range [0, q-1]
            getAnswers()                          // returns vector<string> of "YES"/"NO"
        - Notes:
            * Time indices are inclusive: 0 .. q-1
            * Works for undirected graphs
            * Edge weights are stored but unused by connectivity (future use)
        - Complexity:
            * addEdge : O(log q) (segment tree intervals)
            * solve    : O((N + Q) log Q * α(N))
    */

    class RollbackDSU {
    private:
        vector<int> parent, sz;
        stack<pair<int,int>> history; // (node_b, old_size_of_parent_a) or (-1,-1) no-op marker

    public:
        RollbackDSU() = default;
        RollbackDSU(int n) { init(n); }

        void init(int n) {
            parent.resize(n);
            sz.assign(n, 1);
            while (!history.empty()) history.pop();
            for (int i = 0; i < n; ++i) parent[i] = i;
        }

        int find(int x) {
            while (x != parent[x]) x = parent[x];
            return x;
        }

        // Unite a,b (undirected)
        void unite(int a, int b) {
            a = find(a); b = find(b);
            if (a == b) {
                history.push({-1, -1}); // marker - no change
                return;
            }
            if (sz[a] < sz[b]) swap(a, b);
            // record (childRoot, oldSizeOfParentRoot)
            history.push({b, sz[a]});
            parent[b] = a;
            sz[a] += sz[b];
        }

        // rollback one operation
        void rollback() {
            if (history.empty()) return;
            auto last = history.top(); history.pop();
            if (last.first == -1) return; // no-op
            int b = last.first;
            int oldSizeA = last.second;
            int a = parent[b]; // current parent
            parent[b] = b;
            sz[a] = oldSizeA;
        }

        int snapshot() const {
            return (int)history.size();
        }

        void rollbackTo(int snap) {
            while ((int)history.size() > snap) rollback();
        }
    };


    // Edge structure (future-proof: weights, ids)
    struct Edge {
        int u, v;
        long long w; // weight (for future MST)
        int id;      // optional edge id
        Edge() {}
        Edge(int _u, int _v, long long _w = 0, int _id = -1) : u(_u), v(_v), w(_w), id(_id) {}
    };


    class OfflineDynamicConnectivity {
    private:
        int n;            // number of nodes
        int q;            // number of time slots (0..q-1)
        RollbackDSU dsu;

        // segment tree over time; each node stores list of Edge active on that segment
        vector<vector<Edge>> seg;

        // queries[t] = pair(u,v) for connectivity query at time t; (-1,-1) means no query
        vector<pair<int,int>> queries;
        vector<string> answers;

        // internal helper to add an edge into segment-tree interval
        void addEdgeInterval(int idx, int l, int r, int ql, int qr, const Edge &e) {
            if (ql > r || qr < l) return;
            if (ql <= l && r <= qr) {
                seg[idx].push_back(e);
                return;
            }
            int mid = (l + r) >> 1;
            addEdgeInterval(idx<<1, l, mid, ql, qr, e);
            addEdgeInterval(idx<<1|1, mid+1, r, ql, qr, e);
        }

        // recursive D&C over time
        void solveRec(int idx, int l, int r) {
            int snap = dsu.snapshot();

            // apply all edges active in this time segment
            for (const Edge &e : seg[idx]) {
                dsu.unite(e.u, e.v);
            }

            if (l == r) {
                // answer query at time l if present
                if (queries[l].first != -1) {
                    int u = queries[l].first;
                    int v = queries[l].second;
                    answers[l] = (dsu.find(u) == dsu.find(v) ? string("YES") : string("NO"));
                }
            } else {
                int mid = (l + r) >> 1;
                solveRec(idx<<1, l, mid);
                solveRec(idx<<1|1, mid+1, r);
            }

            // rollback to previous snapshot
            dsu.rollbackTo(snap);
        }

    public:
        // Constructor: n = nodes count, q = number of time slots (queries)
        OfflineDynamicConnectivity(int nNodes, int qSlots) : n(nNodes), q(qSlots) {
            dsu.init(n);
            seg.assign(4 * max(1, q), {});
            queries.assign(max(1, q), {-1, -1});
            answers.assign(max(1, q), "");
        }

        // Reset (useful for multiple runs)
        void reset(int nNodes, int qSlots) {
            n = nNodes; q = qSlots;
            dsu.init(n);
            seg.clear();
            seg.assign(4 * max(1, q), {});
            queries.assign(max(1, q), {-1, -1});
            answers.assign(max(1, q), "");
        }

        // Add an edge active during time interval [start, end] (inclusive)
        // start,end must satisfy: 0 <= start <= end <= q-1
        // w and id are optional for future use
        void addEdge(int u, int v, long long w, int start, int end, int id = -1) {
            if (q == 0) return; // no time slots
            if (start > end) return;
            Edge e(u, v, w, id);
            addEdgeInterval(1, 0, q-1, start, end, e);
        }

        // Register connectivity query (u,v) at time t (0-based)
        void addQuery(int u, int v, int t) {
            if (t < 0 || t >= q) return;
            queries[t] = {u, v};
        }

        // Build (run D&C & DSU) and answer queries for time range [0, q-1]
        void buildAndSolve() {
            if (q == 0) return;
            // ensure DSU is initialized (in case reset was used)
            dsu.init(n);
            solveRec(1, 0, q-1);
        }

        // Get answers in original time order ("" for time slots without queries)
        vector<string> getAnswers() const {
            return answers;
        }

        // Helper: set edge weight later or retrieve seg structure (for extension)
        // You can iterate seg tree and examine Edge::w / Edge::id for MST extension
        const vector<vector<Edge>>& getSegmentTree() const {
            return seg;
        }

        // Optional: direct access to queries list
        const vector<pair<int,int>>& getQueries() const {
            return queries;
        }
    };
}
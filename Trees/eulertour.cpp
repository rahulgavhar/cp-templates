#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{


    /*
        ===============================================
        EulerTour Class — DFS-based Euler Tour on Trees
        ===============================================

        Features:
        ---------
        1. Performs an Euler tour on a tree to record various DFS timelines.
        2. Supports multiple timelines: tin, tout, entryExitTour, fullTour, edgeTimeline, firstOccur.
        3. Computes depth and parent arrays for tree nodes.
        4. Supports ancestor queries and subtree size queries.
        5. Can generate different types of tours using the 'type' parameter.

        Methods and Complexity:
        -----------------------
        EulerTour(int _n, int _type=0)          : O(n) time, O(n) space — constructor initializes graph.
        void init(int _n, int _type)           : O(n) time, O(n) space — reinitializes all structures.
        void addEdge(int u, int v)             : O(1) time, O(1) space — adds an undirected edge.
        void build(int root=0)                 : O(n) time, O(n) space — builds Euler tour starting from root.
        void dfs(int v, int p, int d)          : O(n) time, O(n) space — depth-first traversal for all arrays.
        bool isAncestor(int u, int v)          : O(1) time, O(1) space — checks if u is ancestor of v.
        int subtreeSize(int v)                  : O(1) time, O(1) space — returns subtree size of node v.
        const vector<int>& getTin() const       : O(1) time, O(1) space — returns entry times.
        const vector<int>& getTout() const      : O(1) time, O(1) space — returns exit times.
        const vector<int>& getEntryTimeline() const  : O(1) time, O(1) space — returns entry timeline.
        const vector<int>& getExitTimeline() const   : O(1) time, O(1) space — returns exit timeline.
        const vector<int>& getEntryExitTimeline() const : O(1) time, O(1) space — returns combined entry+exit timeline.
        const vector<int>& getFullTour() const       : O(1) time, O(1) space — returns full DFS tour including backtracking.
        const vector<int>& getDepthArray() const     : O(1) time, O(1) space — returns depth of each node.
        const vector<int>& getParentArray() const    : O(1) time, O(1) space — returns parent of each node.
        const vector<pair<int,int>>& getEdgeTimeline() const : O(1) time, O(1) space — returns edge traversal sequence.
        const vector<int>& getFirstOccur() const     : O(1) time, O(1) space — returns first occurrence of nodes in full tour.
    */


    class EulerTour {
    public:
        int n, timer, type;
        vector<vector<int>> g;
        vector<int> tin, tout, depth, parent;
        vector<int> entry, exit_tl, entryExitTour, fullTour;
        vector<pair<int,int>> edgeTimeline;
        vector<int> firstOccur;

        // Constructor
        EulerTour(int _n, int _type=0){ init(_n,_type); }

        // Initialize graph structures
        void init(int _n, int _type){
            n=_n; type=_type; timer=0;
            g.assign(n,{});
            tin.assign(n,-1); tout.assign(n,-1);
            depth.assign(n,0); parent.assign(n,-1);
            firstOccur.assign(n,-1);
            entry.clear(); exit_tl.clear(); entryExitTour.clear();
            fullTour.clear(); edgeTimeline.clear();
        }

        // Add an undirected edge
        void addEdge(int u,int v){ g[u].push_back(v); g[v].push_back(u); }

        // Build Euler tour starting from root
        void build(int root=0){ dfs(root,-1,0); }

        // Depth-first traversal to generate Euler tour
        void dfs(int v,int p,int d){
            parent[v]=p; depth[v]=d;

            if(type==-1 || type==0){
                tin[v]=timer++;
                entry.push_back(v);
                entryExitTour.push_back(v);
            }
            fullTour.push_back(v);
            if(firstOccur[v] == -1) firstOccur[v] = (int)fullTour.size()-1;

            for(int to:g[v]) if(to!=p){
                edgeTimeline.push_back({v,to});
                dfs(to,v,d+1);
                edgeTimeline.push_back({to,v});
                fullTour.push_back(v);
                if(type==0){
                    entryExitTour.push_back(v);
                }
            }

            if(type==0){
                entryExitTour.push_back(v);
            }

            if(type==1 || type==0){
                tout[v]=timer++;
                exit_tl.push_back(v);
            }
        }

        // Check if u is ancestor of v
        bool isAncestor(int u,int v){ return tin[u]<=tin[v] && tout[v]<=tout[u]; }

        // Get size of subtree rooted at v
        int subtreeSize(int v){ return tout[v]-tin[v]; }

        // Get entry times
        const vector<int>& getTin() const { return tin; }

        // Get exit times
        const vector<int>& getTout() const { return tout; }

        // Get entry timeline
        const vector<int>& getEntryTimeline() const { return entry; }

        // Get exit timeline
        const vector<int>& getExitTimeline() const { return exit_tl; }

        // Get entry + exit timeline
        const vector<int>& getEntryExitTimeline() const { return entryExitTour; }

        // Get full DFS visit timeline
        const vector<int>& getFullTour() const { return fullTour; }

        // Get depth array
        const vector<int>& getDepthArray() const { return depth; }

        // Get parent array
        const vector<int>& getParentArray() const { return parent; }

        // Get edge timeline
        const vector<pair<int,int>>& getEdgeTimeline() const { return edgeTimeline; }

        // Get first occurrence array
        const vector<int>& getFirstOccur() const { return firstOccur; }
    };
}

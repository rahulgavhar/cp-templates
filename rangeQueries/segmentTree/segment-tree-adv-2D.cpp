/*
   SegTreeAdv2D Class

   This is a 2D Segment Tree for advanced numeric queries with lazy propagation.
   Supports the following efficiently (all in O(log n * log m)):

   Supported Queries:
      - rangeGCD(x1,y1,x2,y2)       -> GCD of values in the rectangle
      - rangeLCM(x1,y1,x2,y2)       -> LCM of values in the rectangle
      - rangeProdMod(x1,y1,x2,y2,M) -> Product modulo M of values in the rectangle
      - pointQuery(x,y)             -> value at (x,y)

   Supported Updates:
      - rangeAssign(x1,y1,x2,y2,val) -> set all values in rectangle to val
      - pointAssign(x,y,val)         -> set value at (x,y) = val

   Internally each node stores:
      - val     : representative value (for assign operations)
      - gcdVal  : GCD of its segment
      - lcmVal  : LCM of its segment
      - prodVal : product of its segment (full product, mod applied only in queries)
      - assignVal, hasAssign for lazy propagation
*/

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class SegTreeAdv2D {
    struct Node {
        ll val;        // representative value for assign
        ll gcdVal;
        ll lcmVal;
        ll prodVal;
        bool hasAssign;
        ll assignVal;
        Node(): val(0), gcdVal(0), lcmVal(1), prodVal(1), hasAssign(false), assignVal(0) {}
    };

    int n, m;
    vector<vector<Node>> tree;

    // ---------------- Build ----------------
    void buildY(int idxX, int idxY, int ly, int ry, const vector<ll>& row) {
        Node &node = tree[idxX][idxY];
        if(ly == ry) {
            ll v = row.empty() ? 0 : row[ly];
            node.val = v;
            node.gcdVal = v;
            node.lcmVal = v;
            node.prodVal = v;
            return;
        }
        int mid = (ly + ry) / 2;
        buildY(idxX, idxY*2, ly, mid, row);
        buildY(idxX, idxY*2+1, mid+1, ry, row);
        pullY(idxX, idxY);
    }

    void buildX(int idxX, int lx, int rx, const vector<vector<ll>>& grid) {
        if(lx == rx) {
            buildY(idxX, 1, 0, m-1, grid.empty() ? vector<ll>(m,0) : grid[lx]);
        } else {
            int mid = (lx + rx) / 2;
            buildX(idxX*2, lx, mid, grid);
            buildX(idxX*2+1, mid+1, rx, grid);
            buildY(idxX, 1, 0, m-1, {});
        }
    }

    // ---------------- Lazy Helpers ----------------
    void applyAssign(int idxX,int idxY,ll val,ll lenY) {
        Node &node = tree[idxX][idxY];
        node.val = val;
        node.gcdVal = val;
        node.lcmVal = val;
        node.prodVal = 1;
        for(int i=0;i<lenY;i++) node.prodVal *= val; // can overflow if too big
        node.assignVal = val;
        node.hasAssign = true;
    }

    void pushY(int idxX,int idxY,int ly,int ry){
        Node &node = tree[idxX][idxY];
        if(node.hasAssign && ly != ry){
            int mid = (ly + ry) / 2;
            applyAssign(idxX,idxY*2,node.assignVal, mid-ly+1);
            applyAssign(idxX,idxY*2+1,node.assignVal, ry-mid);
            node.hasAssign = false;
        }
    }

    void pullY(int idxX,int idxY){
        Node &node = tree[idxX][idxY];
        Node &L = tree[idxX][idxY*2], &R = tree[idxX][idxY*2+1];
        node.gcdVal = __gcd(L.gcdVal,R.gcdVal);
        ll lcmGcd = __gcd(L.lcmVal,R.lcmVal);
        node.lcmVal = (lcmGcd == 0 ? max(L.lcmVal,R.lcmVal) : (L.lcmVal / lcmGcd) * R.lcmVal);
        node.prodVal = L.prodVal * R.prodVal;
    }

    void pullX(int idxX,int idxY){
        Node &node = tree[idxX][idxY];
        Node &L = tree[idxX*2][idxY];
        Node &R = tree[idxX*2+1][idxY];
        node.gcdVal = __gcd(L.gcdVal,R.gcdVal);
        ll lcmGcd = __gcd(L.lcmVal,R.lcmVal);
        node.lcmVal = (lcmGcd == 0 ? max(L.lcmVal,R.lcmVal) : (L.lcmVal / lcmGcd) * R.lcmVal);
        node.prodVal = L.prodVal * R.prodVal;
    }

    // ---------------- Updates ----------------
    void rangeAssignY(int idxX,int idxY,int ly,int ry,int y1,int y2,ll val){
        if(y2<ly || ry<y1) return;
        if(y1<=ly && ry<=y2){ applyAssign(idxX,idxY,val,ry-ly+1); return; }
        pushY(idxX,idxY,ly,ry);
        int mid = (ly + ry) / 2;
        rangeAssignY(idxX,idxY*2,ly,mid,y1,y2,val);
        rangeAssignY(idxX,idxY*2+1,mid+1,ry,y1,y2,val);
        pullY(idxX,idxY);
    }

    void rangeAssignX(int idxX,int lx,int rx,int x1,int x2,int y1,int y2,ll val){
        if(x2<lx || rx<x1) return;
        if(x1<=lx && rx<=x2){ rangeAssignY(idxX,1,0,m-1,y1,y2,val); return; }
        int mid = (lx + rx) / 2;
        rangeAssignX(idxX*2,lx,mid,x1,x2,y1,y2,val);
        rangeAssignX(idxX*2+1,mid+1,rx,x1,x2,y1,y2,val);
        mergeY(idxX,1,0,m-1);
    }

    void mergeY(int idxX,int idxY,int ly,int ry){
        if(ly == ry){
            pullX(idxX,idxY);
            return;
        }
        int mid = (ly + ry) / 2;
        mergeY(idxX,idxY*2,ly,mid);
        mergeY(idxX,idxY*2+1,mid+1,ry);
        pullY(idxX,idxY);
    }

    // ---------------- Queries ----------------
    ll queryGCDY(int idxX,int idxY,int ly,int ry,int y1,int y2){
        if(y2<ly || ry<y1) return 0;
        Node &node = tree[idxX][idxY];
        if(y1<=ly && ry<=y2) return node.gcdVal;
        pushY(idxX,idxY,ly,ry);
        int mid = (ly + ry)/2;
        return __gcd(queryGCDY(idxX,idxY*2,ly,mid,y1,y2),
                     queryGCDY(idxX,idxY*2+1,mid+1,ry,y1,y2));
    }

    ll queryGCDX(int idxX,int lx,int rx,int x1,int x2,int y1,int y2){
        if(x2<lx || rx<x1) return 0;
        if(x1<=lx && rx<=x2) return queryGCDY(idxX,1,0,m-1,y1,y2);
        int mid = (lx+rx)/2;
        return __gcd(queryGCDX(idxX*2,lx,mid,x1,x2,y1,y2),
                     queryGCDX(idxX*2+1,mid+1,rx,x1,x2,y1,y2));
    }

    ll queryLCMY(int idxX,int idxY,int ly,int ry,int y1,int y2){
        if(y2<ly || ry<y1) return 1;
        Node &node = tree[idxX][idxY];
        if(y1<=ly && ry<=y2) return node.lcmVal;
        pushY(idxX,idxY,ly,ry);
        int mid = (ly + ry)/2;
        ll L = queryLCMY(idxX,idxY*2,ly,mid,y1,y2);
        ll R = queryLCMY(idxX,idxY*2+1,mid+1,ry,y1,y2);
        if(L==1) return R;
        if(R==1) return L;
        return (L/__gcd(L,R))*R;
    }

    ll queryLCMX(int idxX,int lx,int rx,int x1,int x2,int y1,int y2){
        if(x2<lx || rx<x1) return 1;
        if(x1<=lx && rx<=x2) return queryLCMY(idxX,1,0,m-1,y1,y2);
        int mid = (lx+rx)/2;
        ll L = queryLCMX(idxX*2,lx,mid,x1,x2,y1,y2);
        ll R = queryLCMX(idxX*2+1,mid+1,rx,x1,x2,y1,y2);
        if(L==1) return R;
        if(R==1) return L;
        return (L/__gcd(L,R))*R;
    }

    ll queryProdModY(int idxX,int idxY,int ly,int ry,int y1,int y2,ll mod){
        if(y2<ly || ry<y1) return 1;
        Node &node = tree[idxX][idxY];
        if(y1<=ly && ry<=y2) return node.prodVal % mod;
        pushY(idxX,idxY,ly,ry);
        int mid = (ly+ry)/2;
        return (queryProdModY(idxX,idxY*2,ly,mid,y1,y2,mod) *
                queryProdModY(idxX,idxY*2+1,mid+1,ry,y1,y2,mod)) % mod;
    }

    ll queryProdModX(int idxX,int lx,int rx,int x1,int x2,int y1,int y2,ll mod){
        if(x2<lx || rx<x1) return 1;
        if(x1<=lx && rx<=x2) return queryProdModY(idxX,1,0,m-1,y1,y2,mod);
        int mid = (lx+rx)/2;
        return (queryProdModX(idxX*2,lx,mid,x1,x2,y1,y2,mod) *
                queryProdModX(idxX*2+1,mid+1,rx,x1,x2,y1,y2,mod)) % mod;
    }

public:
    // ---------------- Constructors ----------------
    SegTreeAdv2D(int n_, int m_): n(n_), m(m_) {
        tree.assign(4*n, vector<Node>(4*m));
    }

    SegTreeAdv2D(const vector<vector<ll>>& grid): n(grid.size()), m(grid[0].size()) {
        tree.assign(4*n, vector<Node>(4*m));
        buildX(1,0,n-1,grid);
    }

    // ---------------- Public API ----------------
    void rangeAssign(int x1,int y1,int x2,int y2,ll val){
        rangeAssignX(1,0,n-1,x1,x2,y1,y2,val);
    }

    void pointAssign(int x,int y,ll val){
        rangeAssign(x,y,x,y,val);
    }

    ll rangeGCD(int x1,int y1,int x2,int y2){
        return queryGCDX(1,0,n-1,x1,x2,y1,y2);
    }

    ll rangeLCM(int x1,int y1,int x2,int y2){
        return queryLCMX(1,0,n-1,x1,x2,y1,y2);
    }

    ll rangeProdMod(int x1,int y1,int x2,int y2,ll mod){
        return queryProdModX(1,0,n-1,x1,x2,y1,y2,mod);
    }

    ll pointQuery(int x,int y){
        return rangeGCD(x,y,x,y); // default GCD value at point
    }
};

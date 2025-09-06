/*
   SegTree2D Class

   This is a general-purpose 2D Segment Tree with Lazy Propagation.
   It supports the following efficiently (all in O(log n * log m)):

   Supported Queries:
      - rangeSum(x1,y1,x2,y2) -> sum of values in rectangle [(x1,y1),(x2,y2)]
      - rangeMin(x1,y1,x2,y2) -> minimum value in rectangle [(x1,y1),(x2,y2)]
      - rangeMax(x1,y1,x2,y2) -> maximum value in rectangle [(x1,y1),(x2,y2)]
      - pointQuery(x,y)       -> value at cell (x,y)

   Supported Updates:
      - rangeAssign(x1,y1,x2,y2,val) -> set all values in rectangle [(x1,y1),(x2,y2)] to val
      - rangeAdd(x1,y1,x2,y2,val)    -> add val to all values in rectangle [(x1,y1),(x2,y2)]
      - pointAssign(x,y,val)         -> set value at cell (x,y) = val
      - pointAdd(x,y,val)            -> add val to cell (x,y)

   Internally each node stores:
      - sum  : sum of its rectangle
      - mn   : min of its rectangle
      - mx   : max of its rectangle
      - add, assignVal, hasAssign for lazy propagation
*/



#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class SegTree2D {
private:
    struct Node {
        ll sum;
        ll mn;
        ll mx;
        ll add;         // lazy add
        ll assignVal;   // lazy assign
        bool hasAssign;
        int sz;         // number of cells covered by this node
        Node(): sum(0), mn(LLONG_MAX), mx(LLONG_MIN),
                add(0), assignVal(0), hasAssign(false), sz(0) {}
    };

    int n, m;
    vector<vector<Node>> tree;
    int szX, szY;

    void pushX(int idxX) {
        if ((idxX << 1) >= szX) return; // Check left child exists
        if ((idxX << 1 | 1) >= szX) return; // Check right child exists

        Node &node = tree[idxX][1];
        if (node.hasAssign) {
            rangeAssignY(idxX<<1, 1, 0, m-1, 0, m-1, node.assignVal);
            rangeAssignY(idxX<<1|1, 1, 0, m-1, 0, m-1, node.assignVal);
            node.hasAssign = false;
            node.assignVal = 0;
        } else if (node.add != 0) {
            rangeAddY(idxX<<1, 1, 0, m-1, 0, m-1, node.add);
            rangeAddY(idxX<<1|1, 1, 0, m-1, 0, m-1, node.add);
            node.add = 0;
        }
    }

public:
    SegTree2D(int n_ = 0, int m_ = 0): n(n_), m(m_) {
        if (n > 0 && m > 0) init_empty(n,m);
    }

    SegTree2D(const vector<vector<ll>>& grid) {
        init(grid);
    }

    void init_empty(int nn, int mm){
        n = nn; m = mm;
        szX = 4 * max(1, n) + 5;
        szY = 4 * max(1, m) + 5;
        tree.assign(szX, vector<Node>(szY));
    }

    void init(const vector<vector<ll>>& grid) {
        if (grid.empty() || grid[0].empty()) {
            n = m = 0;
            tree.clear();
            return;
        }
        n = (int)grid.size();
        m = (int)grid[0].size();
        init_empty(n,m);
        buildX(1, 0, n-1, grid);
    }

    void clear() {
        tree.clear();
        n = m = 0;
        szX = szY = 0;
    }

    void rangeAssign(int x1,int y1,int x2,int y2,ll val){ ensure_not_empty(); rangeAssignX(1,0,n-1,x1,x2,y1,y2,val); }
    void rangeAdd(int x1,int y1,int x2,int y2,ll val){ ensure_not_empty(); rangeAddX(1,0,n-1,x1,x2,y1,y2,val); }
    void pointAssign(int x,int y,ll val){ rangeAssign(x,y,x,y,val); }
    void pointAdd(int x,int y,ll val){ rangeAdd(x,y,x,y,val); }

    ll rangeSum(int x1,int y1,int x2,int y2){ ensure_not_empty(); return rangeSumX(1,0,n-1,x1,x2,y1,y2); }
    ll rangeMin(int x1,int y1,int x2,int y2){ ensure_not_empty(); return rangeMinX(1,0,n-1,x1,x2,y1,y2); }
    ll rangeMax(int x1,int y1,int x2,int y2){ ensure_not_empty(); return rangeMaxX(1,0,n-1,x1,x2,y1,y2); }
    ll pointQuery(int x,int y){ ensure_not_empty(); return rangeSum(x,y,x,y); }

private:
    void buildX(int idxX,int lx,int rx,const vector<vector<ll>>& grid){
        if(lx==rx){
            buildYLeaf(idxX,1,0,m-1,grid[lx],1);
        } else {
            int mid=(lx+rx)/2;
            buildX(idxX<<1,lx,mid,grid);
            buildX(idxX<<1|1,mid+1,rx,grid);
            buildYMerge(idxX,1,0,m-1);
        }
    }

    void buildYLeaf(int idxX,int idxY,int ly,int ry,const vector<ll>& row,int xLen){
        Node &node = tree[idxX][idxY];
        int yLen = ry - ly + 1;
        node.sz = xLen * yLen;
        node.add = 0; node.hasAssign = false; node.assignVal = 0;
        if(ly==ry){
            ll v = row[ly];
            node.sum = v * (ll)xLen;
            node.mn = node.mx = v;
            return;
        }
        int mid=(ly+ry)/2;
        buildYLeaf(idxX, idxY<<1, ly, mid, row, xLen);
        buildYLeaf(idxX, idxY<<1|1, mid+1, ry, row, xLen);
        pull(idxX, idxY);
        node.add = 0; node.hasAssign = false; node.assignVal = 0;
    }

    void buildYMerge(int idxX,int idxY,int ly,int ry){
        Node &node = tree[idxX][idxY];
        node.add = 0; node.hasAssign = false; node.assignVal = 0;
        if(ly==ry){
            const Node &L = tree[idxX<<1][idxY];
            const Node &R = tree[idxX<<1|1][idxY];
            node.sz = L.sz + R.sz;
            node.sum = L.sum + R.sum;
            node.mn = min(L.mn, R.mn);
            node.mx = max(L.mx, R.mx);
            return;
        }
        int mid=(ly+ry)/2;
        buildYMerge(idxX, idxY<<1, ly, mid);
        buildYMerge(idxX, idxY<<1|1, mid+1, ry);
        pull(idxX, idxY);
        node.add = 0; node.hasAssign = false; node.assignVal = 0;
    }

    inline void applyAssign(int idxX,int idxY,ll val){
        Node &node = tree[idxX][idxY];
        node.sum = val * (ll)node.sz;
        node.mn = node.mx = val;
        node.assignVal = val;
        node.hasAssign = true;
        node.add = 0;
    }
    inline void applyAdd(int idxX,int idxY,ll val){
        Node &node = tree[idxX][idxY];
        if(node.hasAssign){
            node.assignVal += val;
            ll newVal = node.assignVal;
            node.sum = newVal * (ll)node.sz;
            node.mn = node.mx = newVal;
            return;
        }
        node.add += val;
        node.sum += val * (ll)node.sz;
        if(node.mn != LLONG_MAX) node.mn += val;
        if(node.mx != LLONG_MIN) node.mx += val;
    }
    inline void push(int idxX,int idxY){
        Node &node = tree[idxX][idxY];
        int left = idxY<<1, right = left|1;
        if(left >= szY || right >= szY) return;

        if(node.hasAssign){
            applyAssign(idxX, left, node.assignVal);
            applyAssign(idxX, right, node.assignVal);
            node.hasAssign = false;
        }
        if(node.add != 0){
            ll v = node.add;
            applyAdd(idxX, left, v);
            applyAdd(idxX, right, v);
            node.add = 0;
        }
    }
    inline void pull(int idxX,int idxY){
        Node &node = tree[idxX][idxY];
        const Node &L = tree[idxX][idxY<<1];
        const Node &R = tree[idxX][idxY<<1|1];
        node.sz = L.sz + R.sz;
        node.sum = L.sum + R.sum;
        node.mn = min(L.mn, R.mn);
        node.mx = max(L.mx, R.mx);
    }

    void rangeYMerge(int idxX,int idxY,int ly,int ry){
        Node &node = tree[idxX][idxY];
        node.add = 0; node.hasAssign = false; node.assignVal = 0;
        if(ly==ry){
            const Node &L = tree[idxX<<1][idxY];
            const Node &R = tree[idxX<<1|1][idxY];
            node.sz = L.sz + R.sz;
            node.sum = L.sum + R.sum;
            node.mn = min(L.mn, R.mn);
            node.mx = max(L.mx, R.mx);
            return;
        }
        int mid=(ly+ry)/2;
        rangeYMerge(idxX, idxY<<1, ly, mid);
        rangeYMerge(idxX, idxY<<1|1, mid+1, ry);
        pull(idxX, idxY);
        node.add = 0; node.hasAssign = false; node.assignVal = 0;
    }

    void rangeAssignY(int idxX,int idxY,int ly,int ry,int ql,int qr,ll val){
        if(qr<ly||ry<ql) return;
        if(ql<=ly&&ry<=qr){ applyAssign(idxX,idxY,val); return; }
        push(idxX,idxY);
        int mid=(ly+ry)/2;
        rangeAssignY(idxX,idxY<<1,ly,mid,ql,qr,val);
        rangeAssignY(idxX,idxY<<1|1,mid+1,ry,ql,qr,val);
        pull(idxX,idxY);
    }
    void rangeAddY(int idxX,int idxY,int ly,int ry,int ql,int qr,ll val){
        if(qr<ly||ry<ql) return;
        if(ql<=ly&&ry<=qr){ applyAdd(idxX,idxY,val); return; }
        push(idxX,idxY);
        int mid=(ly+ry)/2;
        rangeAddY(idxX,idxY<<1,ly,mid,ql,qr,val);
        rangeAddY(idxX,idxY<<1|1,mid+1,ry,ql,qr,val);
        pull(idxX,idxY);
    }

    void rangeAssignX(int idxX,int lx,int rx,int qlx,int qrx,int qly,int qry,ll val){
        if(qrx<lx||rx<qlx) return;
        if(qlx<=lx&&rx<=qrx){
            rangeAssignY(idxX,1,0,m-1,qly,qry,val);
            return;
        }
        pushX(idxX);
        int mid=(lx+rx)/2;
        rangeAssignX(idxX<<1,lx,mid,qlx,qrx,qly,qry,val);
        rangeAssignX(idxX<<1|1,mid+1,rx,qlx,qrx,qly,qry,val);
        rangeYMerge(idxX,1,0,m-1);
    }
    void rangeAddX(int idxX,int lx,int rx,int qlx,int qrx,int qly,int qry,ll val){
        if(qrx<lx||rx<qlx) return;
        if(qlx<=lx&&rx<=qrx){
            rangeAddY(idxX,1,0,m-1,qly,qry,val);
            return;
        }
        pushX(idxX);
        int mid=(lx+rx)/2;
        rangeAddX(idxX<<1,lx,mid,qlx,qrx,qly,qry,val);
        rangeAddX(idxX<<1|1,mid+1,rx,qlx,qrx,qly,qry,val);
        rangeYMerge(idxX,1,0,m-1);
    }

    ll rangeSumY(int idxX,int idxY,int ly,int ry,int ql,int qr){
        if(qr<ly||ry<ql) return 0;
        if(ql<=ly&&ry<=qr) return tree[idxX][idxY].sum;
        push(idxX,idxY);
        int mid=(ly+ry)/2;
        return rangeSumY(idxX,idxY<<1,ly,mid,ql,qr)+
               rangeSumY(idxX,idxY<<1|1,mid+1,ry,ql,qr);
    }
    ll rangeMinY(int idxX,int idxY,int ly,int ry,int ql,int qr){
        if(qr<ly||ry<ql) return LLONG_MAX;
        if(ql<=ly&&ry<=qr) return tree[idxX][idxY].mn;
        push(idxX,idxY);
        int mid=(ly+ry)/2;
        return min(rangeMinY(idxX,idxY<<1,ly,mid,ql,qr),
                   rangeMinY(idxX,idxY<<1|1,mid+1,ry,ql,qr));
    }
    ll rangeMaxY(int idxX,int idxY,int ly,int ry,int ql,int qr){
        if(qr<ly||ry<ql) return LLONG_MIN;
        if(ql<=ly&&ry<=qr) return tree[idxX][idxY].mx;
        push(idxX,idxY);
        int mid=(ly+ry)/2;
        return max(rangeMaxY(idxX,idxY<<1,ly,mid,ql,qr),
                   rangeMaxY(idxX,idxY<<1|1,mid+1,ry,ql,qr));
    }

    ll rangeSumX(int idxX,int lx,int rx,int qlx,int qrx,int qly,int qry){
        if(qrx<lx||rx<qlx) return 0;
        if(qlx<=lx&&rx<=qrx) return rangeSumY(idxX,1,0,m-1,qly,qry);
        pushX(idxX);
        int mid=(lx+rx)/2;
        return rangeSumX(idxX<<1,lx,mid,qlx,qrx,qly,qry)+
               rangeSumX(idxX<<1|1,mid+1,rx,qlx,qrx,qly,qry);
    }
    ll rangeMinX(int idxX,int lx,int rx,int qlx,int qrx,int qly,int qry){
        if(qrx<lx||rx<qlx) return LLONG_MAX;
        if(qlx<=lx&&rx<=qrx) return rangeMinY(idxX,1,0,m-1,qly,qry);
        pushX(idxX);
        int mid=(lx+rx)/2;
        return min(rangeMinX(idxX<<1,lx,mid,qlx,qrx,qly,qry),
                   rangeMinX(idxX<<1|1,mid+1,rx,qlx,qrx,qly,qry));
    }
    ll rangeMaxX(int idxX,int lx,int rx,int qlx,int qrx,int qly,int qry){
        if(qrx<lx||rx<qlx) return LLONG_MIN;
        if(qlx<=lx&&rx<=qrx) return rangeMaxY(idxX,1,0,m-1,qly,qry);
        pushX(idxX);
        int mid=(lx+rx)/2;
        return max(rangeMaxX(idxX<<1,lx,mid,qlx,qrx,qly,qry),
                   rangeMaxX(idxX<<1|1,mid+1,rx,qlx,qrx,qly,qry));
    }

    void ensure_not_empty() const {
        if(n==0||m==0) throw runtime_error("SegTree2D: empty tree");
    }
};
/**
 * SegTree2D<T>
 * ----------------
 * 2D lazy segment tree (dynamic quadtree) supporting:
 *   - Range add updates
 *   - Range assign (set) updates
 *   - Range sum / min / max queries
 *   - Point queries
 *
 * Complexity:
 *   - Updates & Queries: O(logN * logM)
 *   - Space: O(number of active nodes), up to O(N*M)
 */

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template<typename T>
class SegTree2D {
    struct Node {
        T sum = 0;
        T mn = numeric_limits<T>::max();
        T mx = numeric_limits<T>::lowest();
        T lazy_add = 0;
        T lazy_set = numeric_limits<T>::lowest(); // sentinel for "no set"
        int child[4] = {-1, -1, -1, -1}; // indices in nodes[]
    };

    int n = 0, m = 0;
    vector<Node> nodes;

    int newNode() {
        nodes.push_back(Node());
        return (int)nodes.size() - 1;
    }

    inline long long area(int x1, int x2, int y1, int y2) {
        return 1LL * (x2 - x1 + 1) * (y2 - y1 + 1);
    }

    void pull(int v) {
        Node &nd = nodes[v];
        nd.sum = 0;
        nd.mn = numeric_limits<T>::max();
        nd.mx = numeric_limits<T>::lowest();
        for (int i = 0; i < 4; i++) {
            int c = nd.child[i];
            if (c == -1) continue;
            nd.sum += nodes[c].sum;
            nd.mn = min(nd.mn, nodes[c].mn);
            nd.mx = max(nd.mx, nodes[c].mx);
        }
    }

    void apply_set(int v, int x1, int x2, int y1, int y2, T val) {
        Node &nd = nodes[v];
        nd.sum = val * area(x1, x2, y1, y2);
        nd.mn = nd.mx = val;
        nd.lazy_set = val;
        nd.lazy_add = 0;
    }

    void apply_add(int v, int x1, int x2, int y1, int y2, T val) {
        Node &nd = nodes[v];
        nd.sum += val * area(x1, x2, y1, y2);
        if (nd.lazy_set != numeric_limits<T>::lowest()) nd.lazy_set += val;
        else nd.lazy_add += val;
        if (nd.mn != numeric_limits<T>::max()) nd.mn += val;
        if (nd.mx != numeric_limits<T>::lowest()) nd.mx += val;
    }

    void push(int v, int x1, int x2, int y1, int y2) {
        Node &nd = nodes[v];
        if (x1 == x2 && y1 == y2) {
            nd.lazy_set = numeric_limits<T>::lowest();
            nd.lazy_add = 0;
            return;
        }
        int mx = (x1 + x2) >> 1;
        int my = (y1 + y2) >> 1;
        for (int i = 0; i < 4; i++) {
            int cx1 = (i < 2 ? x1 : mx+1);
            int cx2 = (i < 2 ? mx : x2);
            int cy1 = (i % 2 == 0 ? y1 : my+1);
            int cy2 = (i % 2 == 0 ? my : y2);
            if (cx1 > cx2 || cy1 > cy2) continue;
            if (nd.child[i] == -1) nd.child[i] = newNode();
            int c = nd.child[i];
            if (nd.lazy_set != numeric_limits<T>::lowest()) apply_set(c, cx1, cx2, cy1, cy2, nd.lazy_set);
            if (nd.lazy_add != 0) apply_add(c, cx1, cx2, cy1, cy2, nd.lazy_add);
        }
        nd.lazy_set = numeric_limits<T>::lowest();
        nd.lazy_add = 0;
    }

    void build_rec(int v, int x1, int x2, int y1, int y2, const vector<vector<T>>& mat) {
        if (x1 > x2 || y1 > y2) return;
        if (x1 == x2 && y1 == y2) {
            nodes[v].sum = nodes[v].mn = nodes[v].mx = mat[x1][y1];
            return;
        }
        int mx = (x1 + x2) >> 1;
        int my = (y1 + y2) >> 1;
        for (int i = 0; i < 4; i++) {
            int cx1 = (i < 2 ? x1 : mx+1);
            int cx2 = (i < 2 ? mx : x2);
            int cy1 = (i % 2 == 0 ? y1 : my+1);
            int cy2 = (i % 2 == 0 ? my : y2);
            if (cx1 > cx2 || cy1 > cy2) continue;
            nodes[v].child[i] = newNode();
            build_rec(nodes[v].child[i], cx1, cx2, cy1, cy2, mat);
        }
        pull(v);
    }

    void update_add_rec(int v,int x1,int x2,int y1,int y2,int ux1,int uy1,int ux2,int uy2,T val) {
        if (ux1 > x2 || ux2 < x1 || uy1 > y2 || uy2 < y1) return;
        if (ux1 <= x1 && x2 <= ux2 && uy1 <= y1 && y2 <= uy2) {
            apply_add(v,x1,x2,y1,y2,val);
            return;
        }
        push(v,x1,x2,y1,y2);
        int mx = (x1 + x2) >> 1;
        int my = (y1 + y2) >> 1;
        for (int i = 0; i < 4; i++) {
            int cx1 = (i < 2 ? x1 : mx+1);
            int cx2 = (i < 2 ? mx : x2);
            int cy1 = (i % 2 == 0 ? y1 : my+1);
            int cy2 = (i % 2 == 0 ? my : y2);
            if (cx1 > cx2 || cy1 > cy2) continue;
            if (nodes[v].child[i] == -1) nodes[v].child[i] = newNode();
            update_add_rec(nodes[v].child[i], cx1,cx2,cy1,cy2, ux1,uy1,ux2,uy2,val);
        }
        pull(v);
    }

    void update_set_rec(int v,int x1,int x2,int y1,int y2,int ux1,int uy1,int ux2,int uy2,T val) {
        if (ux1 > x2 || ux2 < x1 || uy1 > y2 || uy2 < y1) return;
        if (ux1 <= x1 && x2 <= ux2 && uy1 <= y1 && y2 <= uy2) {
            apply_set(v,x1,x2,y1,y2,val);
            return;
        }
        push(v,x1,x2,y1,y2);
        int mx = (x1 + x2) >> 1;
        int my = (y1 + y2) >> 1;
        for (int i = 0; i < 4; i++) {
            int cx1 = (i < 2 ? x1 : mx+1);
            int cx2 = (i < 2 ? mx : x2);
            int cy1 = (i % 2 == 0 ? y1 : my+1);
            int cy2 = (i % 2 == 0 ? my : y2);
            if (cx1 > cx2 || cy1 > cy2) continue;
            if (nodes[v].child[i] == -1) nodes[v].child[i] = newNode();
            update_set_rec(nodes[v].child[i], cx1,cx2,cy1,cy2, ux1,uy1,ux2,uy2,val);
        }
        pull(v);
    }

    T query_sum_rec(int v,int x1,int x2,int y1,int y2,int qx1,int qy1,int qx2,int qy2) {
        if (qx1 > x2 || qx2 < x1 || qy1 > y2 || qy2 < y1) return 0;
        if (qx1 <= x1 && x2 <= qx2 && qy1 <= y1 && y2 <= qy2) return nodes[v].sum;
        push(v,x1,x2,y1,y2);
        int mx = (x1 + x2) >> 1;
        int my = (y1 + y2) >> 1;
        T res = 0;
        for (int i = 0; i < 4; i++) {
            int cx1 = (i < 2 ? x1 : mx+1);
            int cx2 = (i < 2 ? mx : x2);
            int cy1 = (i % 2 == 0 ? y1 : my+1);
            int cy2 = (i % 2 == 0 ? my : y2);
            if (cx1 > cx2 || cy1 > cy2) continue;
            if (nodes[v].child[i] == -1) continue;
            res += query_sum_rec(nodes[v].child[i], cx1,cx2,cy1,cy2,qx1,qy1,qx2,qy2);
        }
        return res;
    }

    T query_min_rec(int v,int x1,int x2,int y1,int y2,int qx1,int qy1,int qx2,int qy2) {
        if (qx1 > x2 || qx2 < x1 || qy1 > y2 || qy2 < y1) return numeric_limits<T>::max();
        if (qx1 <= x1 && x2 <= qx2 && qy1 <= y1 && y2 <= qy2) return nodes[v].mn;
        push(v,x1,x2,y1,y2);
        int mx = (x1 + x2) >> 1;
        int my = (y1 + y2) >> 1;
        T res = numeric_limits<T>::max();
        for (int i = 0; i < 4; i++) {
            int cx1 = (i < 2 ? x1 : mx+1);
            int cx2 = (i < 2 ? mx : x2);
            int cy1 = (i % 2 == 0 ? y1 : my+1);
            int cy2 = (i % 2 == 0 ? my : y2);
            if (cx1 > cx2 || cy1 > cy2) continue;
            if (nodes[v].child[i] == -1) continue;
            res = min(res, query_min_rec(nodes[v].child[i], cx1,cx2,cy1,cy2,qx1,qy1,qx2,qy2));
        }
        return res;
    }

    T query_max_rec(int v,int x1,int x2,int y1,int y2,int qx1,int qy1,int qx2,int qy2) {
        if (qx1 > x2 || qx2 < x1 || qy1 > y2 || qy2 < y1) return numeric_limits<T>::lowest();
        if (qx1 <= x1 && x2 <= qx2 && qy1 <= y1 && y2 <= qy2) return nodes[v].mx;
        push(v,x1,x2,y1,y2);
        int mx = (x1 + x2) >> 1;
        int my = (y1 + y2) >> 1;
        T res = numeric_limits<T>::lowest();
        for (int i = 0; i < 4; i++) {
            int cx1 = (i < 2 ? x1 : mx+1);
            int cx2 = (i < 2 ? mx : x2);
            int cy1 = (i % 2 == 0 ? y1 : my+1);
            int cy2 = (i % 2 == 0 ? my : y2);
            if (cx1 > cx2 || cy1 > cy2) continue;
            if (nodes[v].child[i] == -1) continue;
            res = max(res, query_max_rec(nodes[v].child[i], cx1,cx2,cy1,cy2,qx1,qy1,qx2,qy2));
        }
        return res;
    }

public:
    SegTree2D() = default;
    SegTree2D(const vector<vector<T>>& mat) { build(mat); }

    void build(const vector<vector<T>>& mat) {
        n = (int)mat.size();
        m = n ? (int)mat[0].size() : 0;
        nodes.clear();
        if (!n || !m) return;
        nodes.reserve(4 * n * m);
        nodes.push_back(Node());
        build_rec(0, 0, n-1, 0, m-1, mat);
    }

    void update_add(int x1,int y1,int x2,int y2,T val) {
        if (!n || !m) return;
        x1 = max(0, x1); y1 = max(0, y1);
        x2 = min(n-1, x2); y2 = min(m-1, y2);
        if (x1 > x2 || y1 > y2) return;
        update_add_rec(0, 0, n-1, 0, m-1, x1,y1,x2,y2,val);
    }

    void update_set(int x1,int y1,int x2,int y2,T val) {
        if (!n || !m) return;
        x1 = max(0, x1); y1 = max(0, y1);
        x2 = min(n-1, x2); y2 = min(m-1, y2);
        if (x1 > x2 || y1 > y2) return;
        update_set_rec(0, 0, n-1, 0, m-1, x1,y1,x2,y2,val);
    }

    T query_sum(int x1,int y1,int x2,int y2) {
        if (!n || !m) return 0;
        x1 = max(0, x1); y1 = max(0, y1);
        x2 = min(n-1, x2); y2 = min(m-1, y2);
        if (x1 > x2 || y1 > y2) return 0;
        return query_sum_rec(0, 0, n-1, 0, m-1, x1,y1,x2,y2);
    }

    T query_min(int x1,int y1,int x2,int y2) {
        if (!n || !m) return numeric_limits<T>::max();
        x1 = max(0, x1); y1 = max(0, y1);
        x2 = min(n-1, x2); y2 = min(m-1, y2);
        if (x1 > x2 || y1 > y2) return numeric_limits<T>::max();
        return query_min_rec(0, 0, n-1, 0, m-1, x1,y1,x2,y2);
    }

    T query_max(int x1,int y1,int x2,int y2) {
        if (!n || !m) return numeric_limits<T>::lowest();
        x1 = max(0, x1); y1 = max(0, y1);
        x2 = min(n-1, x2); y2 = min(m-1, y2);
        if (x1 > x2 || y1 > y2) return numeric_limits<T>::lowest();
        return query_max_rec(0, 0, n-1, 0, m-1, x1,y1,x2,y2);
    }

    T query_point(int x,int y) { return query_sum(x,y,x,y); }
};
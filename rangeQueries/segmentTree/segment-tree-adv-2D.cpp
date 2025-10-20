#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ===============================
        SegTreeAdv2D Class — Function Complexities (1 line each)
        ===============================

        SegTreeAdv2D()                               → Default constructor | Time: O(1) | Space: O(1)
        SegTreeAdv2D(N, M, MOD, LCM_CAP)            → Constructor for empty matrix | Time: O(1) | Space: O(1)
        SegTreeAdv2D(arr, MOD, LCM_CAP)             → Constructor from 2D array | Time: O(n*m) | Space: O(n*m)
        rangeAssign(x1, y1, x2, y2, val)           → Assign val to rectangle | Time: O(log n * log m) | Space: O(new nodes)
        pointAssign(x, y, val)                      → Assign val to single cell | Time: O(log n * log m) | Space: O(new nodes)
        rangeGCD(x1, y1, x2, y2)                    → Query GCD in rectangle | Time: O(log n * log m) | Space: O(log n * log m)
        rangeLCM(x1, y1, x2, y2)                    → Query LCM in rectangle | Time: O(log n * log m) | Space: O(log n * log m)
        rangeProdMod(x1, y1, x2, y2)                → Query product modulo in rectangle | Time: O(log n * log m) | Space: O(log n * log m)
        pointQuery(x, y)                             → Query exact cell (GCD) | Time: O(log n * log m) | Space: O(log n * log m)
        nodeCount()                                  → Total allocated nodes | Time: O(1) | Space: O(1)

        --- Internal Helpers ---
        newNode()                                    → Create new tree node | Time: O(1) | Space: O(1)
        applyAssign(v, x1, x2, y1, y2, val)         → Apply assignment lazily | Time: O(1) + mod_pow | Space: O(1)
        pull(v)                                      → Merge child nodes | Time: O(1) | Space: O(1)
        push(v, x1, x2, y1, y2)                      → Propagate lazy assignment | Time: O(1) per child | Space: O(1)
        build_rec(v, x1, x2, y1, y2, a)             → Build tree recursively | Time: O(n*m) | Space: O(n*m)
        rangeAssign_rec(...)                          → Recursive range assign | Time: O(log n * log m) | Space: O(log n * log m)
        queryGCD_rec(...)                             → Recursive GCD query | Time: O(log n * log m) | Space: O(log n * log m)
        queryLCM_rec(...)                             → Recursive LCM query | Time: O(log n * log m) | Space: O(log n * log m)
        queryProdMod_rec(...)                         → Recursive product modulo query | Time: O(log n * log m) | Space: O(log n * log m)
    */



    template<typename T>
    class SegTreeAdv2D {
        struct Node {
            T gcdVal = 0;
            T lcmVal = 1;
            T prodMod = 1;

            bool hasAssign = false;
            T assignVal = 0;

            int child[4] = {-1,-1,-1,-1};
        };

        int n = 0, m = 0;
        T MOD = (T)1000000007;
        T LCM_CAP = (T(1) << 62);
        vector<Node> nodes;

        inline int newNode() {
            nodes.push_back(Node());
            return (int)nodes.size() - 1;
        }

        inline long long area(int x1,int x2,int y1,int y2) const {
            return 1LL * (x2 - x1 + 1) * (y2 - y1 + 1);
        }

        static inline T gcdT(T a,T b){ return std::gcd(a,b); }

        inline T lcm_saturate(T a,T b) const {
            if (a==0 || b==0) return 0;
            __int128 g = std::gcd(a,b);
            __int128 t = (__int128)a / g * b;
            if (t > LCM_CAP) return LCM_CAP;
            return (T)t;
        }

        inline T mod_pow(T a,long long e) const {
            T r = 1 % MOD;
            a = (a % MOD + MOD) % MOD;
            while (e>0) {
                if (e&1) r = (__int128)r * a % MOD;
                a = (__int128)a * a % MOD;
                e >>= 1;
            }
            return r;
        }

        void applyAssign(int v,int x1,int x2,int y1,int y2,T val){
            Node &nd = nodes[v];
            long long ar = area(x1,x2,y1,y2);
            nd.gcdVal = val;
            nd.lcmVal = std::min(LCM_CAP, (T)std::llabs((long long)val));
            nd.prodMod = mod_pow(val, ar);
            nd.hasAssign = true;
            nd.assignVal = val;
        }

        void pull(int v){
            Node &nd = nodes[v];
            bool any=false;
            T g=0,L=1,P=1;
            for(int i=0;i<4;i++){
                int c = nd.child[i];
                if (c==-1) continue;
                any=true;
                Node &C = nodes[c];
                g = gcdT(g,C.gcdVal);
                L = lcm_saturate(L,C.lcmVal);
                P = (__int128)P * C.prodMod % MOD;
            }
            if (any){ nd.gcdVal=g; nd.lcmVal=L; nd.prodMod=P; }
        }

        void push(int v,int x1,int x2,int y1,int y2){
            Node &nd = nodes[v];
            if (!nd.hasAssign) return;
            if (x1==x2 && y1==y2){ nd.hasAssign=false; return; }
            int mx=(x1+x2)>>1,my=(y1+y2)>>1;
            auto ensure=[&](int i,int cx1,int cx2,int cy1,int cy2){
                if (cx1>cx2||cy1>cy2) return;
                if (nd.child[i]==-1) nd.child[i]=newNode();
                applyAssign(nd.child[i],cx1,cx2,cy1,cy2,nd.assignVal);
            };
            ensure(0,x1,mx,y1,my);
            ensure(1,x1,mx,my+1,y2);
            ensure(2,mx+1,x2,y1,my);
            ensure(3,mx+1,x2,my+1,y2);
            nd.hasAssign=false;
        }

        // ---------- BUILD ----------
        void build_rec(int v,int x1,int x2,int y1,int y2,const vector<vector<T>> &a){
            if (x1>x2 || y1>y2) return;
            if (x1==x2 && y1==y2){
                applyAssign(v,x1,x2,y1,y2,a[x1][y1]);
                return;
            }
            int mx=(x1+x2)>>1,my=(y1+y2)>>1;
            int c0=nodes[v].child[0]=newNode();
            int c1=nodes[v].child[1]=newNode();
            int c2=nodes[v].child[2]=newNode();
            int c3=nodes[v].child[3]=newNode();
            build_rec(c0,x1,mx,y1,my,a);
            build_rec(c1,x1,mx,my+1,y2,a);
            build_rec(c2,mx+1,x2,y1,my,a);
            build_rec(c3,mx+1,x2,my+1,y2,a);
            pull(v);
        }

        // ---------- UPDATE ----------
        void rangeAssign_rec(int v,int x1,int x2,int y1,int y2,int ux1,int uy1,int ux2,int uy2,T val){
            if (ux1>x2||ux2<x1||uy1>y2||uy2<y1) return;
            if (ux1<=x1&&x2<=ux2&&uy1<=y1&&y2<=uy2){
                applyAssign(v,x1,x2,y1,y2,val);
                return;
            }
            push(v,x1,x2,y1,y2);
            int mx=(x1+x2)>>1,my=(y1+y2)>>1;
            if (x1<=mx && y1<=my){
                if (nodes[v].child[0]==-1) nodes[v].child[0]=newNode();
                rangeAssign_rec(nodes[v].child[0],x1,mx,y1,my,ux1,uy1,ux2,uy2,val);
            }
            if (x1<=mx && my+1<=y2){
                if (nodes[v].child[1]==-1) nodes[v].child[1]=newNode();
                rangeAssign_rec(nodes[v].child[1],x1,mx,my+1,y2,ux1,uy1,ux2,uy2,val);
            }
            if (mx+1<=x2 && y1<=my){
                if (nodes[v].child[2]==-1) nodes[v].child[2]=newNode();
                rangeAssign_rec(nodes[v].child[2],mx+1,x2,y1,my,ux1,uy1,ux2,uy2,val);
            }
            if (mx+1<=x2 && my+1<=y2){
                if (nodes[v].child[3]==-1) nodes[v].child[3]=newNode();
                rangeAssign_rec(nodes[v].child[3],mx+1,x2,my+1,y2,ux1,uy1,ux2,uy2,val);
            }
            pull(v);
        }

        // ---------- QUERIES ----------
        T queryGCD_rec(int v,int x1,int x2,int y1,int y2,int qx1,int qy1,int qx2,int qy2){
            if (qx1>x2||qx2<x1||qy1>y2||qy2<y1) return 0;
            if (qx1<=x1&&x2<=qx2&&qy1<=y1&&y2<=qy2) return nodes[v].gcdVal;
            push(v,x1,x2,y1,y2);
            int mx=(x1+x2)>>1,my=(y1+y2)>>1;
            T res=0;
            if (x1<=mx && y1<=my && nodes[v].child[0]!=-1) res=gcdT(res,queryGCD_rec(nodes[v].child[0],x1,mx,y1,my,qx1,qy1,qx2,qy2));
            if (x1<=mx && my+1<=y2 && nodes[v].child[1]!=-1) res=gcdT(res,queryGCD_rec(nodes[v].child[1],x1,mx,my+1,y2,qx1,qy1,qx2,qy2));
            if (mx+1<=x2 && y1<=my && nodes[v].child[2]!=-1) res=gcdT(res,queryGCD_rec(nodes[v].child[2],mx+1,x2,y1,my,qx1,qy1,qx2,qy2));
            if (mx+1<=x2 && my+1<=y2 && nodes[v].child[3]!=-1) res=gcdT(res,queryGCD_rec(nodes[v].child[3],mx+1,x2,my+1,y2,qx1,qy1,qx2,qy2));
            return res;
        }

        T queryLCM_rec(int v,int x1,int x2,int y1,int y2,int qx1,int qy1,int qx2,int qy2){
            if (qx1>x2||qx2<x1||qy1>y2||qy2<y1) return 1;
            if (qx1<=x1&&x2<=qx2&&qy1<=y1&&y2<=qy2) return nodes[v].lcmVal;
            push(v,x1,x2,y1,y2);
            int mx=(x1+x2)>>1,my=(y1+y2)>>1;
            T L=1;
            if (x1<=mx && y1<=my && nodes[v].child[0]!=-1) L=lcm_saturate(L,queryLCM_rec(nodes[v].child[0],x1,mx,y1,my,qx1,qy1,qx2,qy2));
            if (x1<=mx && my+1<=y2 && nodes[v].child[1]!=-1) L=lcm_saturate(L,queryLCM_rec(nodes[v].child[1],x1,mx,my+1,y2,qx1,qy1,qx2,qy2));
            if (mx+1<=x2 && y1<=my && nodes[v].child[2]!=-1) L=lcm_saturate(L,queryLCM_rec(nodes[v].child[2],mx+1,x2,y1,my,qx1,qy1,qx2,qy2));
            if (mx+1<=x2 && my+1<=y2 && nodes[v].child[3]!=-1) L=lcm_saturate(L,queryLCM_rec(nodes[v].child[3],mx+1,x2,my+1,y2,qx1,qy1,qx2,qy2));
            return L;
        }

        T queryProdMod_rec(int v,int x1,int x2,int y1,int y2,int qx1,int qy1,int qx2,int qy2){
            if (qx1>x2||qx2<x1||qy1>y2||qy2<y1) return 1 % MOD;
            if (qx1<=x1&&x2<=qx2&&qy1<=y1&&y2<=qy2) return nodes[v].prodMod;
            push(v,x1,x2,y1,y2);
            int mx=(x1+x2)>>1,my=(y1+y2)>>1;
            __int128 P=1;
            if (x1<=mx && y1<=my && nodes[v].child[0]!=-1) P=P*queryProdMod_rec(nodes[v].child[0],x1,mx,y1,my,qx1,qy1,qx2,qy2)%MOD;
            if (x1<=mx && my+1<=y2 && nodes[v].child[1]!=-1) P=P*queryProdMod_rec(nodes[v].child[1],x1,mx,my+1,y2,qx1,qy1,qx2,qy2)%MOD;
            if (mx+1<=x2 && y1<=my && nodes[v].child[2]!=-1) P=P*queryProdMod_rec(nodes[v].child[2],mx+1,x2,y1,my,qx1,qy1,qx2,qy2)%MOD;
            if (mx+1<=x2 && my+1<=y2 && nodes[v].child[3]!=-1) P=P*queryProdMod_rec(nodes[v].child[3],mx+1,x2,my+1,y2,qx1,qy1,qx2,qy2)%MOD;
            return (T)P;
        }

    public:
        SegTreeAdv2D() = default;

        SegTreeAdv2D(int N,int M,T fixedMod=(T)1000000007,T lcmCap=(T(1)<<62))
            :n(N),m(M),MOD(fixedMod),LCM_CAP(lcmCap){
            nodes.clear(); nodes.push_back(Node());
            if (n&&m) applyAssign(0,0,n-1,0,m-1,0);
        }

        SegTreeAdv2D(const vector<vector<T>>& a,T fixedMod=(T)1000000007,T lcmCap=(T(1)<<62))
            :n((int)a.size()),m(n?(int)a[0].size():0),MOD(fixedMod),LCM_CAP(lcmCap){
            nodes.clear(); nodes.push_back(Node());
            if (n&&m) build_rec(0,0,n-1,0,m-1,a);
        }

        void rangeAssign(int x1,int y1,int x2,int y2,T val){
            if (!n||!m) return;
            x1=max(0,x1); y1=max(0,y1);
            x2=min(n-1,x2); y2=min(m-1,y2);
            if (x1>x2||y1>y2) return;
            rangeAssign_rec(0,0,n-1,0,m-1,x1,y1,x2,y2,val);
        }

        void pointAssign(int x,int y,T val){ rangeAssign(x,y,x,y,val); }

        T rangeGCD(int x1,int y1,int x2,int y2){
            if (!n||!m) return 0;
            x1=max(0,x1); y1=max(0,y1);
            x2=min(n-1,x2); y2=min(m-1,y2);
            if (x1>x2||y1>y2) return 0;
            return queryGCD_rec(0,0,n-1,0,m-1,x1,y1,x2,y2);
        }

        T rangeLCM(int x1,int y1,int x2,int y2){
            if (!n||!m) return 1;
            x1=max(0,x1); y1=max(0,y1);
            x2=min(n-1,x2); y2=min(m-1,y2);
            if (x1>x2||y1>y2) return 1;
            return queryLCM_rec(0,0,n-1,0,m-1,x1,y1,x2,y2);
        }

        T rangeProdMod(int x1,int y1,int x2,int y2){
            if (!n||!m) return 1 % MOD;
            x1=max(0,x1); y1=max(0,y1);
            x2=min(n-1,x2); y2=min(m-1,y2);
            if (x1>x2||y1>y2) return 1 % MOD;
            return queryProdMod_rec(0,0,n-1,0,m-1,x1,y1,x2,y2);
        }

        T pointQuery(int x,int y){ return rangeGCD(x,y,x,y); }

        int nodeCount() const { return (int)nodes.size(); }
    };
}

#include<bits/stdc++.h>
using namespace std;

// Aho-Corasick Algorithm
// A DP algorithm which builds links from top-down and is constructively created
// Used to match multiple patterns inside a long text




class AC_NODE{
public:
    unordered_map<char, AC_NODE*> children;
    AC_NODE *failure_link, *output_link;
    int pat_ind;
    char data;

    AC_NODE(char ch){
        data = ch;
        failure_link = nullptr;
        output_link = nullptr;
        pat_ind = -1;
    }
};


class AC_TRIE{
    AC_NODE *root;
public:
    AC_TRIE(){
        root = new AC_NODE('\0');
    }

    void build_failure_output_links(){
        //by default every node has a failure link to root
        //this func works only if root->failurelink == nullptr
        queue<AC_NODE *> bfsq;
        
        for(auto &p:root->children)
            bfsq.push(p.second);

        while(!bfsq.empty()){
            AC_NODE *fr = bfsq.front();
            bfsq.pop();
            
            for(auto &p:fr->children){
                AC_NODE *flink = fr->failure_link;
                do{
                    if(flink->children.find(p.first)!=flink->children.end()){
                        p.second->failure_link = flink->children[p.first];
                        break;
                    }
                    flink = flink->failure_link;
                } while (flink);

                bfsq.push(p.second);
            }

            if(fr->failure_link->pat_ind>=0)
                fr->output_link = fr->failure_link;
            else
                fr->output_link = fr->failure_link->output_link;
        }
    }

    void insert(const string &word, const int &ind) {
        AC_NODE* ptr = root;
        for(const char &c : word) {
            if(ptr->children.find(c)==ptr->children.end()) {
                ptr->children[c] = new AC_NODE(c);
            }
            ptr = ptr->children[c];
            ptr->failure_link = root;
        }
        ptr->pat_ind = ind;
    }


    void buildTrie(const vector<string>&vec){
        int sz=vec.size();
        for (int i = 0; i < sz;i++){
            insert(vec[i], i);
        }
    }

    vector<vector<int>> acocc(const string&text, const vector<string>&pat){
        int sz = pat.size(), n=text.size();
        vector<vector<int>> occ = vector<vector<int>>(sz,vector<int>(0));

        AC_NODE *ptr = root;
        for (int i = 0;i < n ;i++){
            char ch = text[i];
            if(ptr->children.find(ch)!=ptr->children.end()){
                ptr = ptr->children[ch];

                if(ptr->pat_ind>=0)
                    occ[ptr->pat_ind].push_back(i - (int)pat[ptr->pat_ind].length() + 1);

                AC_NODE *mol = ptr->output_link;
                while(mol){
                    occ[mol->pat_ind].push_back(i - (int)pat[mol->pat_ind].length() + 1);
                    mol = mol->output_link;
                }
            }else{
                while(ptr!=root && ptr->children.find(ch)==ptr->children.end())
                    ptr = ptr->failure_link;

                if(ptr->children.find(ch)!=ptr->children.end())
                    i--;
            }
        }

        return occ;
    }


    void freeNode(AC_NODE* node) {
        for(auto &p : node->children) {
            freeNode(p.second);
        }
        delete node;
    }

    ~AC_TRIE() {
        freeNode(root);
    }

};
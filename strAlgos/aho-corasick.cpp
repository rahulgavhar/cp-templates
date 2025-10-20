#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{

    /*
        ===============================
        ACTrie Class — Function Complexities
        ===============================

        1. ACTrie()                          → Constructor | Time: O(1) | Space: O(1)
        2. insert(word, idx)                 → Insert a pattern into the trie | Time: O(len(word)) | Space: O(len(word))
        3. buildTrie(patterns)               → Build trie for multiple patterns | Time: O(total_length) | Space: O(total_length)
        4. build_failure_output_links()      → Build failure and output links | Time: O(total_nodes * σ) worst-case, practical O(total_nodes) | Space: O(total_nodes)
        5. acocc(text, patterns)             → Search all pattern occurrences in text | Time: O(text_length + total_matches) | Space: O(patterns.size() + total_matches)

        --- Internal / User-defined Helpers ---
        6. freeNode(node)                     → Recursively free all nodes | Time: O(total_nodes) | Space: O(total_nodes) recursion stack

        Where:
        total_length = sum of lengths of all patterns
        total_nodes = number of nodes in the trie
        σ = alphabet size (number of possible characters)
        text_length = length of text
        total_matches = total number of occurrences of all patterns in text
        idx = index of pattern in original vector

        Usage Example:
            vector<string> patterns = {"he", "she", "his", "hers"};
            string text = "ahishers";

            AC_TRIE ac;
            ac.buildTrie(patterns);
            ac.build_failure_output_links();

            vector<vector<int>> occ = ac.acocc(text, patterns);

            // Print occurrences
            for(int i=0; i<(int)patterns.size(); i++){
                cout << "Pattern '" << patterns[i] << "' occurs at indices: ";
                for(int idx : occ[i]) cout << idx << " ";
                cout << "\n";
            }
    */


    class AC_NODE{
    public:
        unordered_map<char, AC_NODE*> children;
        AC_NODE *failure_link, *output_link;
        vector<int> pat_inds; // store all indices of patterns ending here
        char data;

        AC_NODE(char ch){
            data = ch;
            failure_link = nullptr;
            output_link = nullptr;
        }
    };

    class AC_TRIE{
        AC_NODE *root;
    public:
        AC_TRIE(){
            root = new AC_NODE('\0');
        }

        void build_failure_output_links(){
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

                if(!fr->failure_link->pat_inds.empty())
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
            ptr->pat_inds.push_back(ind); // store all indices for duplicate patterns
        }

        void buildTrie(const vector<string>&vec){
            int sz=vec.size();
            for (int i = 0; i < sz;i++){
                insert(vec[i], i);
            }
        }

        vector<vector<int>> acocc(const string&text, const vector<string>&pat){
            int sz = pat.size(), n=text.size();
            vector<vector<int>> occ(sz);

            AC_NODE *ptr = root;
            for (int i = 0;i < n ;i++){
                char ch = text[i];
                while(ptr!=root && ptr->children.find(ch)==ptr->children.end())
                    ptr = ptr->failure_link;

                if(ptr->children.find(ch)!=ptr->children.end())
                    ptr = ptr->children[ch];

                // record occurrences for all patterns ending here
                for(int idx : ptr->pat_inds)
                    occ[idx].push_back(i - (int)pat[idx].length() + 1);

                // record occurrences through output links
                AC_NODE *mol = ptr->output_link;
                while(mol){
                    for(int idx : mol->pat_inds)
                        occ[idx].push_back(i - (int)pat[idx].length() + 1);
                    mol = mol->output_link;
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
}

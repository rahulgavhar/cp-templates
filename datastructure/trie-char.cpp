#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
    ===============================
    CharTrie Class — Function Complexities
    ===============================
    1. insert(word)                → Time: O(L)       | Space: O(L)
    2. countWordsEqualTo(word)     → Time: O(L)       | Space: O(1)
    3. countWordsStartingWith(pre) → Time: O(L)       | Space: O(1)
    4. erase(word)                 → Time: O(L)       | Space: O(L) (recursive)
    5. eraseHelper(node, word, d) → Time: O(L)       | Space: O(L)
    6. getWordsWithPrefix(pre)     → Time: O(L + M)   | Space: O(M * L)
    7. dfs(node, cur, res)         → Time: O(M)       | Space: O(M * L)
    8. freeNode(node)              → Time: O(N)       | Space: O(1)
    9. CharTrieNode constructor    → Time: O(1)       | Space: O(1)
    10. CharTrie constructor       → Time: O(A)       | Space: O(A)
    11. ~CharTrie() (Destructor)   → Time: O(N)       | Space: O(1)

    Where:
    L = length of the word/prefix
    M = total words under the given prefix
    N = total nodes in the trie
    A = alphabet size (26 for default a-z)
    Overall space usage ≈ O(N * A)
    */


    class CharTrieNode {
    public:
        char data;
        vector<CharTrieNode*> children;
        bool isTerminal;
        int count; // words ending here
        int used;  // words passing through

        CharTrieNode(char ch, int alphabet_size)
            : data(ch), children(alphabet_size, nullptr), isTerminal(false), count(0), used(0) {}
    };

    class CharTrie {
    private:
        CharTrieNode* root;
        vector<char> alphabet; 
        unordered_map<char,int> charToIdx;
        int alphabet_size;

        void freeNode(CharTrieNode* node) {
            if(!node) return;
            for(auto child : node->children) freeNode(child);
            delete node;
        }

        bool eraseHelper(CharTrieNode* node, const string &word, int depth) {
            if(depth == word.size()) {
                if(node->count > 0) {
                    node->count--;
                    if(node->count == 0) node->isTerminal = false;
                    return true; // word existed, deletion happened
                }
                return false; // word not found
            }

            char c = word[depth];
            if(charToIdx.find(c) == charToIdx.end()) return false;
            int idx = charToIdx[c];
            CharTrieNode* child = node->children[idx];
            if(!child) return false;

            bool deleted = eraseHelper(child, word, depth + 1);

            if(deleted) {
                child->used--;
                if(child->count == 0 && !child->isTerminal && child->used == 0) {
                    delete child;
                    node->children[idx] = nullptr;
                }
            }
            return deleted;
        }

        void dfs(CharTrieNode* node, string current, vector<pair<string,int>> &res) {
            if(!node) return;
            if(node->isTerminal) res.push_back({current, node->count});
            for(int i=0; i<alphabet_size; i++) {
                if(node->children[i])
                    dfs(node->children[i], current + alphabet[i], res); // safe mapping
            }
        }

    public:
        CharTrie(bool lowercase = true, bool uppercase = true) {
            if(lowercase)
                for(char c='a'; c<='z'; c++) alphabet.push_back(c);
            if(uppercase)
                for(char c='A'; c<='Z'; c++) alphabet.push_back(c);

            // If both false, default to lowercase
            if(alphabet.empty()) 
                for(char c='a'; c<='z'; c++) alphabet.push_back(c);

            alphabet_size = alphabet.size();
            for(int i=0; i<alphabet_size; i++) charToIdx[alphabet[i]] = i;

            root = new CharTrieNode('\0', alphabet_size);
        }

        ~CharTrie() {
            freeNode(root);
        }

        // Insert a word
        void insert(const string &word) {
            CharTrieNode* node = root;
            node->used++;
            for(char c : word) {
                if(charToIdx.find(c) == charToIdx.end()) continue; // skip invalid chars
                int idx = charToIdx[c];
                if(!node->children[idx]) node->children[idx] = new CharTrieNode(c, alphabet_size);
                node = node->children[idx];
                node->used++;
            }
            node->isTerminal = true;
            node->count++;
        }

        // Count words exactly equal to given word
        int countWordsEqualTo(const string &word) {
            CharTrieNode* node = root;
            for(char c : word) {
                if(charToIdx.find(c) == charToIdx.end()) return 0;
                int idx = charToIdx[c];
                if(!node->children[idx]) return 0;
                node = node->children[idx];
            }
            return node->count;
        }

        // Count words starting with given prefix
        int countWordsStartingWith(const string &prefix) {
            CharTrieNode* node = root;
            for(char c : prefix) {
                if(charToIdx.find(c) == charToIdx.end()) return 0;
                int idx = charToIdx[c];
                if(!node->children[idx]) return 0;
                node = node->children[idx];
            }
            return node->used;
        }

        // Erase a word
        void erase(const string &word) {
            if(eraseHelper(root, word, 0)) root->used--;
        }

        // Get all words with prefix (duplicates counted)
        vector<pair<string,int>> getWordsWithPrefix(const string &prefix) {
            CharTrieNode* node = root;
            for(char c : prefix) {
                if(charToIdx.find(c) == charToIdx.end()) return {};
                int idx = charToIdx[c];
                if(!node->children[idx]) return {};
                node = node->children[idx];
            }
            vector<pair<string,int>> res;
            dfs(node, prefix, res);
            return res;
        }

        // Optional: clear the trie
        void clear() {
            freeNode(root);
            root = new CharTrieNode('\0', alphabet_size);
        }
    };
}
#include <bits/stdc++.h>
using namespace std;

class TrieNode {
public:
    char data;
    vector<TrieNode*> children;
    bool isTerminal;
    int count, used;

    TrieNode(char ch) : data(ch), children(26, nullptr), isTerminal(false), count(0), used(0) {}
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode('\0');
    }

    // Insert a word
    void insert(string word) {
        TrieNode* ptr = root;
        for(char c : word) {
            int idx = c - 'a';
            if(!ptr->children[idx]) {
                ptr->children[idx] = new TrieNode(c);
            }
            ptr = ptr->children[idx];
            ptr->used++;
        }
        ptr->isTerminal = true;
        ptr->count++;
    }

    // Count exact words
    int countWordsEqualTo(string word) {
        TrieNode* ptr = root;
        for(char c : word) {
            int idx = c - 'a';
            if(!ptr->children[idx]) return 0;
            ptr = ptr->children[idx];
        }
        return ptr->count;
    }

    // Count words starting with prefix
    int countWordsStartingWith(string word) {
        TrieNode* ptr = root;
        for(char c : word) {
            int idx = c - 'a';
            if(!ptr->children[idx]) return 0;
            ptr = ptr->children[idx];
        }
        return ptr->used;
    }

    // Helper for erase
    bool eraseHelper(TrieNode* node, string &word, int depth) {
        if(depth == word.size()) {
            if(node->count > 0) {
                node->count--;
                if(node->count == 0) node->isTerminal = false;
                return node->used == 0 && !node->isTerminal;
            }
            return false;
        }

        int idx = word[depth] - 'a';
        TrieNode* child = node->children[idx];
        if(!child) return false;

        bool shouldDelete = eraseHelper(child, word, depth+1);

        if(shouldDelete) {
            delete child;
            node->children[idx] = nullptr;
        }

        child->used--;
        return node != root && node->used == 0 && !node->isTerminal;
    }

    // Erase a word
    void erase(string word) {
        eraseHelper(root, word, 0);
    }

    // DFS helper for autocomplete
    void dfs(TrieNode* node, string current, vector<string>& results) {
        if(!node) return;
        if(node->isTerminal) results.push_back(current);
        for(int i=0; i<26; i++) {
            if(node->children[i]) {
                dfs(node->children[i], current + char('a' + i), results);
            }
        }
    }

    // Autocomplete: Get all words with given prefix
    vector<string> getWordsWithPrefix(string prefix) {
        TrieNode* ptr = root;
        for(char c : prefix) {
            int idx = c - 'a';
            if(!ptr->children[idx]) return {}; // prefix not found
            ptr = ptr->children[idx];
        }
        vector<string> results;
        dfs(ptr, prefix, results);
        return results;
    }

    // Destructor to free memory
    ~Trie() {
        freeNode(root);
    }

    void freeNode(TrieNode* node) {
        if(!node) return;
        for(auto child : node->children) freeNode(child);
        delete node;
    }
};
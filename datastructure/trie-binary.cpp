#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{
    /*
        ===============================
        BinaryTrie Class — Function Complexities
        ===============================

        1. insert(num)                 → Time: O(B)       | Space: O(B)
        2. erase(num)                  → Time: O(B)       | Space: O(B) (recursive)
        3. eraseHelper(node, num, d)   → Time: O(B)       | Space: O(B)
        4. exists(num)                 → Time: O(B)       | Space: O(1)
        5. maxXor(num)                 → Time: O(B)       | Space: O(1)
        6. minXor(num)                 → Time: O(B)       | Space: O(1)
        7. countXorLessThan(num, k)    → Time: O(B)       | Space: O(1)
        8. isEmpty()                   → Time: O(1)       | Space: O(1)
        9. freeNode(node)              → Time: O(N)       | Space: O(1)
        10. BinaryTrieNode constructor → Time: O(1)       | Space: O(1)
        11. BinaryTrie constructor     → Time: O(1)       | Space: O(1)
        12. ~BinaryTrie() (Destructor) → Time: O(N)       | Space: O(1)

        Where:
        B = number of bits per number (default 31 for int, 63 for long long)
        N = total nodes in the trie
    */



    template <typename T, int MAX_BITS = (sizeof(T) * 8) - 1>
    class BinaryTrieNode {
    public:
        BinaryTrieNode* child[2];   // 0 and 1 branches
        unsigned int used;          // how many numbers pass through this node

        BinaryTrieNode() {
            child[0] = child[1] = nullptr;
            used = 0;
        }
    };

    template <typename T, int MAX_BITS = (sizeof(T) * 8) - 1>
    class BinaryTrie {
    private:
        using Node = BinaryTrieNode<T, MAX_BITS>;
        Node* root;

        // Recursive cleanup
        void freeNode(Node* node) {
            if (!node) return;
            freeNode(node->child[0]);
            freeNode(node->child[1]);
            delete node;
        }

    public:
        /************** Constructor / Destructor **************/
        BinaryTrie() {
            root = new Node();
        }

        ~BinaryTrie() {
            freeNode(root);
        }

        /**************** Insertion ****************/
        void insert(T num) {
            Node* ptr = root;
            ptr->used++;
            for (int i = MAX_BITS; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (!ptr->child[bit])
                    ptr->child[bit] = new Node();
                ptr = ptr->child[bit];
                ptr->used++;
            }
        }

        /**************** Deletion (with cleanup) ****************/
        bool erase(T num) {
            if (!exists(num)) return false; // skip if not found
            eraseHelper(root, num, MAX_BITS);
            return true;
        }

    private:
        bool eraseHelper(Node* node, T num, int bitPos) {
            if (bitPos < 0) {
                node->used--;
                return (node->used == 0 && !node->child[0] && !node->child[1]);
            }

            int bit = (num >> bitPos) & 1;
            Node* next = node->child[bit];
            if (!next) return false; // not found

            bool shouldDelete = eraseHelper(next, num, bitPos - 1);
            if (shouldDelete) {
                delete next;
                node->child[bit] = nullptr;
            }

            node->used--;
            return (node->used == 0 && !node->child[0] && !node->child[1]);
        }

    public:
        /**************** Check if number exists ****************/
        bool exists(T num) const {
            Node* ptr = root;
            for (int i = MAX_BITS; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (!ptr->child[bit]) return false;
                ptr = ptr->child[bit];
            }
            return (ptr->used > 0);
        }

        /**************** Maximum XOR ****************/
        T maxXor(T num) const {
            Node* ptr = root;
            if (!ptr || ptr->used == 0) return 0;
            T res = 0;
            for (int i = MAX_BITS; i >= 0; i--) {
                int bit = (num >> i) & 1;
                int want = 1 - bit;
                if (ptr->child[want] && ptr->child[want]->used > 0) {
                    res |= (T(1) << i);
                    ptr = ptr->child[want];
                } else if (ptr->child[bit] && ptr->child[bit]->used > 0) {
                    ptr = ptr->child[bit];
                } else break;
            }
            return res;
        }

        /**************** Minimum XOR ****************/
        T minXor(T num) const {
            Node* ptr = root;
            if (!ptr || ptr->used == 0) return 0;
            T res = 0;
            for (int i = MAX_BITS; i >= 0; i--) {
                int bit = (num >> i) & 1;
                if (ptr->child[bit] && ptr->child[bit]->used > 0) {
                    ptr = ptr->child[bit];
                } else if (ptr->child[1 - bit] && ptr->child[1 - bit]->used > 0) {
                    res |= (T(1) << i);
                    ptr = ptr->child[1 - bit];
                } else break;
            }
            return res;
        }

        /**************** Count XOR < K ****************/
        // Count of numbers x such that (x XOR num) < k
        unsigned int countXorLessThan(T num, T k) const {
            Node* ptr = root;
            unsigned int res = 0;
            for (int i = MAX_BITS; i >= 0 && ptr; i--) {
                int bitNum = (num >> i) & 1;
                int bitK = (k >> i) & 1;

                if (bitK == 1) {
                    if (ptr->child[bitNum])
                        res += ptr->child[bitNum]->used;
                    ptr = ptr->child[1 - bitNum];
                } else {
                    ptr = ptr->child[bitNum];
                }
            }
            return res;
        }

        /**************** Utility ****************/
        bool isEmpty() const { return root->used == 0; }
    };
}

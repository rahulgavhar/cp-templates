#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;




inline namespace MY{


    /*
        ===============================================
        Manachers Class — Longest Palindromic Substring
        ===============================================

        Features:
        ---------
        1. Implements Manacher's algorithm for finding the longest palindromic substring.
        2. Handles both odd-length and even-length palindromes uniformly using '#' transformation.
        3. Linear-time algorithm: O(n) time complexity.
        4. Space-efficient: O(n) extra space for palindrome radii array.
        5. Works for empty strings and strings containing spaces.

        Methods and Complexity:
        -----------------------
        1. static string transform(const string &str)
        - Converts a string into a transformed string with '#' inserted between characters
        and at both ends to handle even-length palindromes.
        - Time: O(n), Space: O(n)

        2. static string lonPalindrome(const string &str)
        - Returns the longest palindromic substring of the input string.
        - Time: O(n), Space: O(n)
        - Algorithm:
        a. Transform the input string with '#' characters.
        b. Use an array `p` to store the radius of the palindrome centered at each position.
        c. Maintain a current palindrome boundary [l, r] to skip redundant comparisons.
        d. Update the maximum palindrome length and its center as the algorithm progresses.
        e. Extract the substring from the original string corresponding to the longest palindrome.
    */


    class Manachers{
    public:
        static string transform(const string &str){
            string s = "#";
            for(const char &c:str){
                s.push_back(c);
                s.push_back('#');
            }
            return s;
        }
        static string lonPalindrome(const string &str){
            if (str.empty()) return "";
            
            string t = transform(str);
            int n = t.length();

            int l = 0, r = 0;

            int center, maxLen=0;

            vector<int> p(n,0);

            for (int i = 1; i < n;i++){
                int k;

                if(i>r)
                    k = 0; // start comparing from the character itself
                else{
                    int j = l + (r - i);

                    if(j - p[j] > l){
                        p[i] = p[j]; 
                        // directly copy the segment length of p[j] in p[i] only if the j'th palindrome lies STRICTLY inside l, mismatch char should be l or ahead of l
                        // even if j - p[j] == l it means l'th char was a part of j'th palindrome, in that case move to else block
                        continue;
                    }else
                        k = r - i;
                        //Here we have to say, char from i to r(included) is a part of palindrome of i
                        //skip comparing the palindomic segment(r-i) which we already know is a palindrome -> '(r-i) chars left of' i '(r-i) chars right of' i
                        //start comparing ahead of right boundary
                }


                while((i-k)>=0 && (i+k)<n && t[i-k]==t[i+k])
                    k++;

                k--;

                l = i - k;
                r = i + k;
                // update left and right boundary

                p[i] = k;

                if(p[i] > maxLen){
                    maxLen = p[i];
                    center = i;
                }
                //update center and maxLen of longest Palindrome

            }

            int start = (center - maxLen), start_in_str = start / 2;
            return str.substr(start_in_str, maxLen);
        }

    };
}

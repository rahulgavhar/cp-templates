//Manachers Algo- Longest Palindromic Substring O(n)

// Proof of O(n) - pointer 'r' never moves backwards only forwards, thus inner while loop runs only if palindrome lies outside 'r' and if it runs it definately increases 'r' again
// Thus in worst case O(2 * n)




#include<bits/stdc++.h>
using namespace std;


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
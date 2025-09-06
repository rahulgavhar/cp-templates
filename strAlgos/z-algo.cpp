// Z algo similiar to KMP in O(n) time
// Space: O(m), Time: O(n+m)



#include<bits/stdc++.h>
using namespace std;


class zAlgo {
public:

    // Helper function to access characters as if pattern + "$" + text is concatenated
    static char charAt(const string &pattern, const string &text, int idx) {
        int m = pattern.size();
        if (idx < m) return pattern[idx];
        if (idx == m) return '$';
        return text[idx - m - 1];
    }

    static vector<int> zocc(const string &text, const string &pattern) {
        int m = pattern.size(), n = text.size(), sz = m + 1 + n;

        vector<int> occurrences;
        vector<int> Z(m, 0);

        // Step 1: Compute Z values for pattern prefix
        int L = 0, R = 0;  // Current Z-box boundaries in pattern
        for (int i = 1; i < m; i++) {
            
            if(i > R) Z[i]=0;
            else{
                if(i + Z[i - L] < R) continue;
                else Z[i] = R - i;
            }

            // Expand Z-box as long as characters match the prefix
            while ((i + Z[i]) < m && charAt(pattern, text, Z[i]) == charAt(pattern, text, i + Z[i])) Z[i]++;

            // Update current Z-box
            L = i;
            R = i + Z[i] - 1;
        }

        // Step 2: Scan text portion using pattern Z-values
        L = m + 1;   // Start of current Z-box in text
        R = m + 1;  // End of current Z-box in text

        for (int textIndex = m + 1; textIndex <= sz-m; textIndex++) {
            int matchLength;  // Length of current match with pattern prefix
            
            if(textIndex > R) matchLength = 0;
            else{
                if(textIndex + Z[textIndex - L] < R) continue;
                else matchLength = R - textIndex;
            }

            // Here R remaines same or is increased

            // Expand match as long as characters match
            while ((textIndex + matchLength) < sz && charAt(pattern, text, matchLength) == charAt(pattern, text, textIndex + matchLength)) matchLength++;

            // Update Z-box boundaries
            L = textIndex;
            R = textIndex + matchLength - 1;

            // If match length equals pattern, record occurrence
            if (matchLength == m) occurrences.push_back(textIndex - (m + 1)); // Index in text
        }

        return occurrences;
    }
};
#include <bits/stdc++.h>
using namespace std;

class KMP
{
public:
    static vector<int> buildLPS(const string &pat)
    {
        int m = pat.length(), j = 0, i = 1;
        vector<int> LPS(m, 0);

        while (i < m)
        {
            if (pat[i] == pat[j])
            {
                LPS[i] = j + 1;
                i++;
                j++;
            }
            else
            {
                if (j == 0)
                {
                    LPS[i] = 0;
                    i++;
                }
                else
                    j = LPS[j - 1];
            }
        }

        return LPS;
    }

    static vector<int> kmpocc(const string &text, const string &pat)
    {
        int n = text.length(), m = pat.length(), j = 0, i = 0;
        if (n < m || m == 0)
            return {};

        vector<int> occ;
        vector<int> LPS = buildLPS(pat);

        while (i < n)
        {
            if (text[i] == pat[j])
            {
                i++;
                j++;
                if (j == m)
                {
                    occ.push_back(i - m);
                    j = LPS[j - 1];
                }
            }
            else
            {
                if (j == 0)
                    i++;
                else
                    j = LPS[j - 1];
            }
        }

        return occ;
    }
};

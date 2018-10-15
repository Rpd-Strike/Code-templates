#include <bits/stdc++.h>

using namespace std;

const int NMAX = 1e5;

/// DSU Structure that can support undo operation,
/// has logN complexity
class UndoDSU
{
private:
    int tt[NMAX+2], gr[NMAX+2];
    vector<pair<int,int>> m_tt, m_gr;

    int ROOT(int nod)
    {
        if( tt[nod] == nod ) return nod;
        return ROOT(tt[nod]);
    }

public:
    UndoDSU(int n)
    {
        for( int i = 1;  i <= n;  ++i )
            tt[i] = i, gr[i] = 1;
    }

    void addEdge(int x, int y)
    {
        x = ROOT(x), y = ROOT(y);
        if( gr[x] < gr[y] ) swap(x, y);

        m_tt.push_back({y, tt[y]});
        m_gr.push_back({x, gr[x]});

        tt[y] = x;
        gr[x] += (gr[x] == gr[y]);
    }

    void undoEdge()
    {
        assert(!m_tt.empty() && !m_gr.empty());
        assert(m_tt.size() == m_gr.size());
        pair<int,int> mod;

        mod = m_tt.back();
        tt[mod.first] = mod.second;
        m_tt.pop_back();

        mod = m_gr.back();
        gr[mod.first] = mod.second;
        m_gr.pop_back();
    }

    bool areConnected(int x, int y)
    {
        return ROOT(x) == ROOT(y);
    }
};


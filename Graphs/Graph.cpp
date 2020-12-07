#include <bits/stdc++.h>

using namespace std;

// 1-indexed graph
struct CostGraph
{
    using Edge = pair<int, int>;  // <to, cost>
    using Graph = vector<vector<Edge>>;

    int N;
    Graph mGraph;
    vector<pair<int, Edge>> mEdges;

    CostGraph(int n) : N(n), mGraph(n + 1) 
    { }

    void insertEdge(int from, Edge to)
    {
        assert(1 <= from && from <= N);
        assert(1 <= to.first && to.first <= N);

        mGraph[from].push_back(to);
        mEdges.push_back({from, to});
    }

    int getAPM()
    {
        vector<int> tt(N + 1), gr(N + 1, 1);
        
        function<int (int)> ROOT = [&](int nod) {
            if (tt[nod] == nod)
                return nod;
            return tt[nod] = ROOT(tt[nod]);
        };

        function<bool (int, int)> UNION = [&](int x, int y) {
            x = ROOT(x), y = ROOT(y);
            if ( x == y )
                return false;
            if (gr[x] < gr[y])
                swap(x, y);

            gr[x] += gr[y];
            tt[y] = x;
            return true;
        };
        
        iota(tt.begin(), tt.end(), 0);
        sort(mEdges.begin(), mEdges.end(), [](const pair<int, Edge>& a, const pair<int, Edge>& b){
            return a.second.second < b.second.second;
        });

        int apmCost = 0;
        for (auto [from, ed] : mEdges) {
            auto [to, cost] = ed;
            if (UNION(from, to))
                apmCost += cost;
        }
        return apmCost;
    }
};
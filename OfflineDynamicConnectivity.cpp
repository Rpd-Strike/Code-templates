#include <bits/stdc++.h>

using namespace std;

#include "UndoDSU.cpp"

/// Offline DSU Connectivity,
/// can be modified together with UndoDSU to be smarter
class OfflineDynamicConnectivity
{
private:
    /// The undo dsu structure implementation
    UndoDSU dsu;

    int N, Nodes;
    vector<vector<pair<int,int>>> queries, aint;  /// queries[time][i] = i-th query for time
    vector<vector<bool>> answers;                 /// answers[time][i] = for time, answer for i-th query

    void update(int nod, int st, int dr, int x, int y, int ed_x, int ed_y)
    {
        if( y < st || dr < x ) return ;
        if( x <= st && dr <= y ) {
            aint[nod].push_back({ed_x, ed_y});
            return ;
        }

        int m = (st + dr) / 2;
        update(2 * nod, st, m, x, y, ed_x, ed_y);
        update(2 * nod + 1, m + 1, dr, x, y, ed_x, ed_y);
    }

    void dfs(int nod, int st, int dr)
    {
        for( auto ed : aint[nod] )
            dsu.addEdge(ed.first, ed.second);

        if( st == dr ) {
            for( auto qr : queries[st] )
                answers[st].push_back(dsu.areConnected(qr.first, qr.second));
        }
        else {
            int m = (st + dr) / 2;
            dfs(2 * nod, st, m);
            dfs(2 * nod + 1, m + 1, dr);
        }

        for( auto ed : aint[nod] )
            dsu.undoEdge();
    }

public:
    OfflineDynamicConnectivity(int max_time, int max_nodes) : dsu(UndoDSU(max_nodes))
    {
        N = max_time;
        Nodes = max_nodes;
        queries = vector<vector<pair<int,int>>>(N + 1, vector<pair<int,int>>());
        aint = vector<vector<pair<int,int>>>(4 * N + 5, vector<pair<int,int>>());
        answers = vector<vector<bool>>(N + 1, vector<bool>());
    }

    /// Add edge x-y being active in time interval [start, finish]
    void addEdge(int x, int y, int start, int finish)
    {
        assert(1 <= min(x, y) && max(x, y) <= Nodes);
        assert(1 <= start && finish <= N);
        assert(start <= finish);

        update(1, 1, N, start, finish, x, y);
    }

    /// I will ask if there is a x-y path at time
    void addQuery(int x, int y, int time)
    {
        assert(1 <= time && time <= N);
        assert(1 <= min(x, y) && max(x, y) <= Nodes);

        queries[time].push_back({x, y});
    }

    /// ...
    void performQueries()
    {
        dfs(1, 1, N);
    }

    /// returns an array with the answers for each time frame,
    /// in order given by addQuery
    vector<vector<bool>> getAnswers()
    {
        return answers;
    }

    /// In case you forget what order you put the queries inside the structure
    vector<vector<pair<int,int>>> getQueries()
    {
        return queries;
    }
};

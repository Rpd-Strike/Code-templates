struct Biconexe
{
    /// takes an undirected graph (with both (x,y) (y,x) edges included)
    /// and returns a list of bixonex components on a per CRITICAL NODE basis

    static const int NMAX = 100000; /// assign accordingly

    int N;
    stack<pair<int,int>> stk;
    vector<vector<int>> comps;
    vector<int> G[NMAX+2];
    bool viz[NMAX+2];
    int niv[NMAX+2], low[NMAX+2];

    Biconexe(int n, vector<int> (&g)[NMAX+2])
    {
        N = n;
        for( int i = 1;  i <= N;  ++i ) G[i] = g[i];
    }

    void another_component(int x, int y)
    {
        int lx, ly;
        vector<int> comp;

        do {
            lx = stk.top().first; ly = stk.top().second;
            comp.push_back(ly);
            stk.pop();
        } while( !(x == lx && y == ly) );
        comp.push_back(x);

        comps.push_back(comp);
    }

    void dfs(int nod, int tata = -1, int time = 1)
    {
        viz[nod] = 1;
        niv[nod] = low[nod] = time;
        for( int& x : G[nod] ) {
            if( !viz[x] ) {
                stk.push({nod, x});
                dfs(x, nod, time + 1);
                low[nod] = min(low[nod], low[x]);
                if( low[x] >= niv[nod] )
                    another_component(nod, x);
            }
            else {
                low[nod] = min(low[nod], niv[x]);
            }
        }
    }

    vector<vector<int>> getBiconexe()
    {
        memset(viz, 0, sizeof(viz));
        for( int i = 1;  i <= N;  ++i ) if( !viz[i] )
            dfs(i);
        return comps;
    }

};
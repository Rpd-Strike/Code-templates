struct SuffixArray
{
    static const int LGMAX = 210005;
    static const int LOG = 19;

    int N;
    string str;
    vector<int> toSort;
    pair<int,int> aux[NMAX+2];
    int classes[LOG][NMAX+2];

    SuffixArray(string _str)
    {
        str = _str;
        N = (int)str.size();
        toSort = vector<int>(N);
    }

    vector<int> makeSuffixArray()
    {
        for( int i = 0;  i < N;  ++i ) {
            classes[0][i] = str[i];
            toSort[i] = i;
        }
        for( int j = 1;  j < LOG;  ++j ) {
            for( int i = 0;  i < N;  ++i )
                aux[i] = {classes[j - 1][i], i + (1 << j)/2 < N ? classes[j - 1][i + (1 << j)/2] : -1};
            sort(toSort.begin(), toSort.end(), [&](const int& a, const int& b){
                return aux[a] < aux[b];
            });
            for( int i = 0;  i < N;  ++i )
                classes[j][ toSort[i] ] = (i == 0 ? 1 : classes[j][ toSort[i - 1] ] + (aux[ toSort[i - 1] ] != aux[ toSort[i] ]));
        }
        return toSort;
    }

    int LCP(int a, int b)
    {
        int ans = 0, step = LOG - 1;
        while(step >= 0) {
            if( classes[step][a] == classes[step][b] && a + (1 << step) <= N && b + (1 << step) <= N ) {
                ans += (1 << step);
                a += (1 << step);
                b += (1 << step);
            }
            --step;
        }
        return ans;
    }
};
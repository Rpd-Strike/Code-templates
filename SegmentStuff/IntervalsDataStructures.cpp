#include <bits/stdc++.h>

using namespace std;

/*
*  IntervalCover <Type>
*
*  Keeps a list of compact closed intervals on integers
*  Creation:     O(1)
*  Add / Remove: O(nr_intervals * log(Size))
*
*  Tested: Some Codeforces Problems
*/

template <class T>
class IntervalCover
{
public:
    static const T INF;

private:
    set<pair<T, T>> s;

    typename set<pair<T, T>>::iterator Get_upper_touching_interval(T st, T dr)
    {
        /// returns the rightmost interval containing something from [st, dr]
        auto it = s.upper_bound({dr, INF});
        if( it == s.begin() )
            return s.end();
        --it;
        if( max(st, it->first) <= min(dr, it->second) )
            return it;
        return s.end();
    }

public:
    vector<pair<T, T>> Add_interval(T st, T dr)
    {
        /// returns the closed intervals needed to complete [st, dr]
        vector<pair<T, T>> touch, to_add;
        auto it = s.end();
        while( (it = Get_upper_touching_interval(st - 1, dr + 1)) != s.end() )
        {
            touch.push_back(*it);
            s.erase(it);
        }
        reverse(touch.begin(), touch.end());

        if( touch.empty() ) {
            s.insert({st, dr});

            to_add.push_back({st, dr});
            return to_add;
        }

        if( touch.begin()->first > st )
            to_add.push_back({
                st,
                touch[0].first - 1
            });

        for( unsigned int i = 1;  i < touch.size();  ++i )
            assert(touch[i - 1].second + 1 <= touch[i].first - 1),
            to_add.push_back({
                touch[i - 1].second + 1,
                touch[i].first - 1
            });

        if( touch.back().second < dr )
            to_add.push_back({
                touch.back().second + 1,
                dr
            });

        T back_st = min(st, touch.begin()->first);
        T back_dr = max(dr, touch.back().second);
        s.insert({back_st, back_dr});

        return to_add;
    }

    vector<pair<T, T>> Remove_interval(T st, T dr)
    {
        /// returns the closed intervals needed to remove [st, dr]
        vector<pair<T, T>> touch, to_remove;
        auto it = s.end();
        while( (it = get_upper_touching_interval(st, dr)) != s.end() )
        {
            touch.push_back(*it);
            s.erase(it);
        }
        reverse(touch.begin(), touch.end());

        if( touch.empty() )
            return to_remove;

        if( touch.begin()->first < st )
            s.insert({
                touch.begin()->first,
                st - 1
            });

        if( touch.back().second > dr )
            s.insert({
                dr + 1,
                touch.back().second
            });

        for( auto pp : touch )
            assert(max(st, pp.first) <= min(dr, pp.second)),
            to_remove.push_back({
                max(st, pp.first),
                min(dr, pp.second)
            });

        return to_remove;
    }
};

/// default constants, you can specialize them
template <class T>
const T IntervalCover<T>::INF = 1e9;

/// specialized constants
template <>
const long long IntervalCover<long long>::INF = 1e18;

template <>
const int IntervalCover<int>::INF = 1e9;



/*
*  LinearFuncSumLazySegmentTree <Type, Type Modulo>
*
*  Lazy segment tree
*  Update: Apply a deg 1 polynomial to numbers in range
*  Query:  Return sum of numbers in range
*  Complexity:  Creation: O(N)
*               Update:   O(logN)
*               Query:    O(logN)
*
*  Note: Sadly it doesn't work for any order,
*        as you need to keep polynomials of order
*        Degree ^ (nr of updates) worst case
*
*  Tested on some Codeforces Problem
*/

template <class T, T Modulo>
class LinearFuncSumLazySegmentTree
{
public:
    static const T NullElement;
    static const T IdentityElement;
    static const T MOD = Modulo;
private:
    int N;
    vector<T> tree;
    vector<pair<T, T>> lazy;  /// first = x^1 , second = x^0

    void Propagate(int nod, int st, int dr)
    {
        /// Propagate the pending update to lower level
        tree[nod] = (lazy[nod].first * tree[nod] + lazy[nod].second * (dr - st + 1)) % MOD;
        if( st < dr ) {
            lazy[2 * nod] = Combine(lazy[nod], lazy[2 * nod]);
            lazy[2 * nod + 1] = Combine(lazy[nod], lazy[2 * nod + 1]);
        }
        lazy[nod] = {IdentityElement, NullElement};
    }

    pair<T, T> Combine(const pair<T, T> &a, const pair<T, T> &b)
    {
        /// Returns the transformation f(x) = a(b(x))
        return {
            (a.first * b.first) % MOD,
            (a.first * b.second + a.second) % MOD
        };
    }

public:
    LinearFuncSumLazySegmentTree(int N) :
        N(N),
        tree(vector<T>(4 * N + 2, NullElement)),
        lazy(vector<pair<T, T>>(4 * N + 2, {IdentityElement, NullElement}))
        {}

    void Update(int nod, int st, int dr, int x, int y, const pair<T, T> &poly)
    {
        /// Apply the poly to [x, y]
        Propagate(nod, st, dr);
        if( y < st || dr < x ) return ;

        if( x <= st && dr <= y ) {
            lazy[nod] = poly;
            // Propagate(nod, st, dr);
            return ;
        }

        int m = (st + dr) / 2;
        Update(2 * nod, st, m, x, y, poly);
        Update(2 * nod + 1, m + 1, dr, x, y, poly);

        tree[nod] = tree[2 * nod] + tree[2 * nod + 1];
        if( MOD && tree[nod] >= MOD )
            tree[nod] -= MOD;
    }

    T Query(int nod, int st, int dr, int x, int y)
    {
        /// Find sum of elements of [x, y]
        Propagate(nod, st, dr);
        if( y < st || dr < x )
            return NullElement;

        if( x <= st && dr <= y )
            return tree[nod];

        int m = (st + dr) / 2;
        T left_sum = Query(2 * nod, st, m, x, y);
        T right_sum = Query(2 * nod + 1, m + 1, dr, x, y);

        return (left_sum + right_sum) % MOD;
    }
};

/// default constants, you can specialize them
template<class T, T Modulo>
const T LinearFuncSumLazySegmentTree<T, Modulo>::NullElement = 0;
template<class T, T Modulo>
const T LinearFuncSumLazySegmentTree<T, Modulo>::IdentityElement = 1;

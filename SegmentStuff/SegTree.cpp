#include <bits/stdc++.h>

using namespace std;

struct SegTree
{
    #define fS (2 * nod)
    #define fD (2 * nod + 1)
    #define mid ((st + dr) / 2)

    struct Atom
    {
        // TODO:  What does it hold?
        int val;

        // important for uninitialized values
        // TODO:  Default values? (Ideally you want a Neutral Element)
        Atom(int v = 0) 
        {
            val = v;
        }

        Atom operator + (const Atom& oth)
        {
            // TODO:  How do they combine?
            auto combine = [](const Atom& lef, const Atom& rig)
            {
                Atom r = {min(lef.val, rig.val)};
                return r;
            };

            return combine(*this, oth);
        }
    };

    int n;
    vector<Atom> aint;

    SegTree(int _size) : n(_size), aint(4 * _size + 5)
    {
        // maybe build it in a specific way here ?
    }

    void Poz_update(int poz, Atom val)
    {
        _poz_update(1, 1, n, poz, val);
    }

    Atom Interv_query(int x, int y)
    {
        return _interv_query(1, 1, n, x, y);
    }

    void _poz_update(int nod, int st, int dr, int poz, Atom val)
    {
        if (dr < poz || poz < st)
            return;
        if (st == dr) {
            aint[nod] = val;
            return ;
        }
        _poz_update(fS, st, mid, poz, val);
        _poz_update(fD, mid + 1, dr, poz, val);
        aint[nod] = aint[fS] + aint[fD];
    }

    Atom _interv_query(int nod, int st, int dr, int x, int y)
    {
        assert(!(dr < x || y < st));

        if (x <= st && dr <= y)
            return aint[nod];
        if (y <= mid)
            return _interv_query(fS, st, mid, x, y);
        if (mid < x)
            return _interv_query(fD, mid + 1, dr, x, y);

        Atom lef = _interv_query(fS, st, mid, x, y);
        Atom rig = _interv_query(fD, mid + 1, dr, x, y);
        return lef + rig;
    }

    int cbin(int nod, int st, int dr, Atom val)
    {
        if (st == dr)
            return st;
        
        if (prop(tree[fS], val)) // if (prop(tree[fS], val))
            // ne uitam in stanga
            return cbin(fS, st, mid, val);
        
        return cbin(fD, mid + 1, dr, val);

        
    }

    #undef fS
    #undef fD
    #undef mid
};

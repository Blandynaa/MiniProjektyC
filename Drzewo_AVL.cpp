#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

string cr, cl, cp;

struct Wezel
{
    Wezel* up, * left, * right;
    int key, bf;    //bf - współczynnik równowagi (balance factor)
};

void drukuj_drzewo(string sp, string sn, Wezel* v)
{
    string s;
    if (v) {
        s = sp;
        if (sn == cr) s[s.length() - 2] = ' ';
        drukuj_drzewo(s + cp, cr, v->right);
        s = s.substr(0, sp.length() - 2);
        cout << s << sn << v->key << ":" << v->bf << endl;
        s = sp;
        if (sn == cl) s[s.length() - 2] = ' ';
        drukuj_drzewo(s + cp, cl, v->left);
    }
}

void usun_drzewo(Wezel* v)
{
    if (v) {
        usun_drzewo(v->left); 
        usun_drzewo(v->right);
        delete v;          
    }
}

void rotacja_RR(Wezel*& root, Wezel* A)
{
    Wezel* B = A->right, * p = A->up;
    A->right = B->left;
    if (A->right) A->right->up = A;
    B->left = A;
    B->up = p;
    A->up = B;
    if (p){
        if (p->left == A) p->left = B; else p->right = B;
    }
    else root = B;
    if (B->bf == -1) A->bf = B->bf = 0;
    else {
        A->bf = -1; B->bf = 1;
    }
}

void rotacja_LL(Wezel*& root, Wezel* A)
{
    Wezel* B = A->left, * p = A->up;
    A->left = B->right;
    if (A->left) A->left->up = A;
    B->right = A;
    B->up = p;
    A->up = B;
    if (p) {
        if (p->left == A) p->left = B; else p->right = B;
    }
    else root = B;
    if (B->bf == 1) A->bf = B->bf = 0;
    else {
        A->bf = 1; B->bf = -1;
    }
}

void rotacja_RL(Wezel*& root, Wezel* A)
{
    Wezel* B = A->right, * C = B->left, * p = A->up;
    B->left = C->right;
    if (B->left) B->left->up = B;
    A->right = C->left;
    if (A->right) A->right->up = A;
    C->left = A;
    C->right = B;
    A->up = B->up = C;
    C->up = p;
    if (p) {
        if (p->left == A) p->left = C; else p->right = C;
    }
    else root = C;
    if (C->bf == -1) A->bf = 1; else A->bf = 0;
    if (C->bf == 1) B->bf = -1; else B->bf = 0;
    C->bf = 0;
}

void rotacja_LR(Wezel*& root, Wezel* A)
{
    Wezel* B = A->left, * C = B->right, * p = A->up;
    B->right = C->left;
    if (B->right) B->right->up = B;
    A->left = C->right;
    if (A->left) A->left->up = A;
    C->right = A;
    C->left = B;
    A->up = B->up = C;
    C->up = p;
    if (p) {
        if (p->left == A) p->left = C; else p->right = C;
    }
    else root = C;
    if (C->bf == 1) A->bf = -1; else A->bf = 0;
    if (C->bf == -1) B->bf = 1; else B->bf = 0;
    C->bf = 0;
}

void dodaj_wezel(Wezel*& root, int k)
{
    Wezel* w, * p, * r;
    bool t;
    w = new Wezel; 
    w->left = w->right = w->up = NULL;
    w->key = k;
    w->bf = 0;
    p = root;
    if (!p) root = w; 
    else {  
        while (true) {
            if (k < p->key) {
                if (!p->left) {
                    p->left = w;  
                    break;      
                }
                p = p->left;   
            }
            else {
                if (!p->right) {
                    p->right = w; 
                    break;        
                }
                p = p->right;   
            }
        }
        w->up = p;     
        if (p->bf) p->bf = 0;
        else {
            if (p->left == w) p->bf = 1;
            else p->bf = -1;
            r = p->up;        
            t = false;
            while (r) {
                if (r->bf) {
                    t = true;     
                    break;    
                };
                if (r->left == p) r->bf = 1;
                else r->bf = -1;
                p = r;         
                r = r->up;
            }
            if (t) {              
                if (r->bf == 1) {
                    if (r->right == p) r->bf = 0; 
                    else if (p->bf == -1) rotacja_LR(root, r);
                           else rotacja_LL(root, r);
                }
                else {            
                    if (r->left == p) r->bf = 0;  
                    else if (p->bf == 1) rotacja_RL(root, r);
                        else rotacja_RR(root, r);
                }
            }
        }
    }
}

Wezel* poprzednik(Wezel* p)
{
    Wezel* r;
    if (p) {
        if (p->left) {
            p = p->left;
            while (p->right) p = p->right;
        }
        else
            do {
                r = p;
                p = p->up;
            } while (p && p->right != r);
    }
    return p;
}

Wezel* szukaj_wezel(Wezel* p, int k)
{
    while (p && p->key != k)
        p = (k < p->key) ? p->left : p->right;
    return p;
}

Wezel* usun_wezel(Wezel*& root, Wezel* x)
{
    Wezel* t, * y, * z;
    bool nest;
    if (x->left && x->right) {
        y = usun_wezel(root, poprzednik(x));
        nest = false;
    }
    else {
        if (x->left) {
            y = x->left; x->left = NULL;
        }
        else {
            y = x->right; x->right = NULL;
        }
        x->bf = 0;
        nest = true;
    }

    if (y) {
        y->up = x->up;
        y->left = x->left;  if (y->left)   y->left->up = y;
        y->right = x->right; if (y->right)  y->right->up = y;
        y->bf = x->bf;
    }

    if (x->up) {
        if (x->up->left == x) x->up->left = y; else x->up->right = y;
    }
    else root = y;

    if (nest) {
        z = y;
        y = x->up;
        while (y) {
            if (!y->bf) { 
                if (y->left == z)  y->bf = -1; else y->bf = 1;
                break;
            }
            else {
                if (((y->bf == 1) && (y->left == z)) || ((y->bf == -1) && (y->right == z))) {
                    y->bf = 0;
                    z = y; y = y->up;
                }
                else {
                    if (y->left == z)  t = y->right; 
                    else t = y->left;
                    if (!t->bf) {      
                        if (y->bf == 1) rotacja_LL(root, y); 
                        else rotacja_RR(root, y);
                        break;
                    }
                    else if (y->bf == t->bf) { 
                        if (y->bf == 1) rotacja_LL(root, y); 
                        else rotacja_RR(root, y);
                        z = t; y = t->up;
                    }
                    else {  
                        if (y->bf == 1) rotacja_LR(root, y); 
                        else rotacja_RL(root, y);
                        z = y->up; y = z->up;
                    }
                }
            }
        }
    }
    return x;
}

int main()
{
    srand(time(NULL));
    int Tk[30];
    int i, x, i1, i2;
    Wezel* root = NULL;

    cr = cl = cp = "  ";
    cr[0] = 218; cr[1] = 196;
    cl[0] = 192; cl[1] = 196;
    cp[0] = 179;

    cout << "DRZEWO AVL\n";
    cout << "Wybierz odpowiednia cyfre:\n1 Automatyczne dodanie 30 liczb do drzewa nastepnie usuniecie 15\n2 Reczne dodanie oraz usuniecie wybranej ilosci liczb\n";
    cin >> i;
    if (i == 1) {
        for (i = 0; i < 30; i++)
        {
            Tk[i] = rand() % 30;
            cout << Tk[i] << " ";
            dodaj_wezel(root, Tk[i]);
        }
        cout << endl << endl;
        drukuj_drzewo("", "", root);
        cout << endl << endl;
        for (i = 0; i < 300; i++) {
            i1 = rand() % 30; i2 = rand() % 30;
            x = Tk[i1]; Tk[i1] = Tk[i2]; Tk[i2] = x;
        }
        for (i = 0; i < 15; i++) {
            i1 = rand() % 30;
            while (Tk[i1] == -1)
            {
                i1 = rand() % 30;
            }
            cout << Tk[i1] << " ";
            usun_wezel(root, szukaj_wezel(root, Tk[i1]));
            Tk[i1] = -1;
        }
        cout << endl << endl;
        drukuj_drzewo("", "", root);
        usun_drzewo(root);
    }
    else if (i == 2) {
        cout << "Ile liczb chcesz dodac?\n";
        cin >> i;
        cout << "Podaj liczby:\n";
        for (i; i > 0; i--) {
            cin >> x;
            dodaj_wezel(root, x);
        }
        drukuj_drzewo("", "", root);
        cout << "Ile liczb chcesz usunac?\n";
        cin >> i;
        cout << "Podaj wybrane liczby z wczesniej dodanych:\n";
        for (i; i > 0; i--) {
            cin >> x;
            usun_wezel(root, szukaj_wezel(root, x));
        }
        drukuj_drzewo("", "", root);
        usun_drzewo(root);
    }
    else cout << "Niepoprawny wybór :(\n\n";

    return 0;
}



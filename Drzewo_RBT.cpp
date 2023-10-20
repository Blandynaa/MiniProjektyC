#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAXN = 30; 

struct Wezel
{
    Wezel* up;
    Wezel* left;
    Wezel* right;
    int wartosc;
    char kolor;
};

Wezel S;             
Wezel* root;        
string cr, cl, cp;     

void usun_drzewo(Wezel* p)
{
    if (p != &S)
    {
        usun_drzewo(p->left);  
        usun_drzewo(p->right); 
        delete p;
    }
}

void drukuj_drzewo(string sp, string sn, Wezel* p)
{
    string t;
    if (p != &S) {
        t = sp;
        if (sn == cr) t[t.length() - 2] = ' ';
        drukuj_drzewo(t + cp, cr, p->right);
        t = t.substr(0, sp.length() - 2);
        cout << t << sn << p->kolor << ":" << p->wartosc << endl;
        t = sp;
        if (sn == cl) t[t.length() - 2] = ' ';
        drukuj_drzewo(t + cp, cl, p->left);
    }
}

Wezel* szukaj_wezel(int k)
{
    Wezel* p = root;
    while ((p != &S) && (p->wartosc != k))
        if (k < p->wartosc) p = p->left;
        else           p = p->right;
    if (p == &S) return NULL;
    return p;
}

Wezel* min_wezel(Wezel* p)
{
    if (p != &S)
        while (p->left != &S) p = p->left;
    return p;
}

Wezel* wezel_nastepnik(Wezel* p)
{
    Wezel* r;
    if (p != &S) {
        if (p->right != &S) return min_wezel(p->right);
        else {
            r = p->up;
            while ((r != &S) && (p == r->right)) {
                p = r;
                r = r->up;
            }
            return r;
        }
    }
    return &S;
}

void rotacja_lewo(Wezel* A)
{
    Wezel* B, * p;
    B = A->right;
    if (B != &S) {
        p = A->up;
        A->right = B->left;
        if (A->right != &S) A->right->up = A;
        B->left = A;
        B->up = p;
        A->up = B;
        if (p != &S) {
            if (p->left == A) p->left = B; 
            else p->right = B;
        }
        else root = B;
    }
}

void rotacja_prawo(Wezel* A)
{
    Wezel* B, * p;
    B = A->left;
    if (B != &S) {
        p = A->up;
        A->left = B->right;
        if (A->left != &S) A->left->up = A;
        B->right = A;
        B->up = p;
        A->up = B;
        if (p != &S) {
            if (p->left == A) p->left = B; else p->right = B;
        }
        else root = B;
    }
}

void dodaj_wezel(int k)
{
    Wezel* X, * Y;

    X = new Wezel;        
    X->left = &S;          
    X->right = &S;
    X->up = root;
    X->wartosc = k;
    if (X->up == &S) root = X; 
    else
        while (true) {         
            if (k < X->up->wartosc) {
                if (X->up->left == &S) {
                    X->up->left = X;  
                    break;
                }
                X->up = X->up->left;
            }
            else {
                if (X->up->right == &S) {
                    X->up->right = X; 
                    break;
                }
                X->up = X->up->right;
            }
        }
    X->kolor = 'R';         
    while ((X != root) && (X->up->kolor == 'R')) {
        if (X->up == X->up->up->left) {
            Y = X->up->up->right;    
            if (Y->kolor == 'R') {   
                X->up->kolor = 'B';
                Y->kolor = 'B';
                X->up->up->kolor = 'R';
                X = X->up->up;
                continue;
            }
            if (X == X->up->right) { 
                X = X->up;
                rotacja_lewo(X);
            }
            X->up->kolor = 'B'; 
            X->up->up->kolor = 'R';
            rotacja_prawo(X->up->up);
            break;
        }
        else {                  
            Y = X->up->up->left;
            if (Y->kolor == 'R') { 
                X->up->kolor = 'B';
                Y->kolor = 'B';
                X->up->up->kolor = 'R';
                X = X->up->up;
                continue;
            }
            if (X == X->up->left) { 
                X = X->up;
                rotacja_prawo(X);
            }
            X->up->kolor = 'B'; 
            X->up->up->kolor = 'R';
            rotacja_lewo(X->up->up);
            break;
        }
    }
    root->kolor = 'B';
}

void usun_wezel(Wezel* X)
{
    Wezel* W, * Y, * Z;
    if ((X->left == &S) || (X->right == &S)) Y = X;
    else Y = wezel_nastepnik(X);
    if (Y->left != &S) Z = Y->left;
    else Z = Y->right;
    Z->up = Y->up;
    if (Y->up == &S) root = Z;
    else if (Y == Y->up->left) Y->up->left = Z;
    else Y->up->right = Z;
    if (Y != X) X->wartosc = Y->wartosc;
    if (Y->kolor == 'B')   
        while ((Z != root) && (Z->kolor == 'B'))
            if (Z == Z->up->left) {
                W = Z->up->right;
                if (W->kolor == 'R') {              
                    W->kolor = 'B';
                    Z->up->kolor = 'R';
                    rotacja_lewo(Z->up);
                    W = Z->up->right;
                }
                if ((W->left->kolor == 'B') && (W->right->kolor == 'B')) {              
                    W->kolor = 'R';
                    Z = Z->up;
                    continue;
                }
                if (W->right->kolor == 'B')
                {              
                    W->left->kolor = 'B';
                    W->kolor = 'R';
                    rotacja_prawo(W);
                    W = Z->up->right;
                }
                W->kolor = Z->up->kolor; 
                Z->up->kolor = 'B';
                W->right->kolor = 'B';
                rotacja_lewo(Z->up);
                Z = root;      
            }
            else {                
                W = Z->up->left;
                if (W->kolor == 'R') {              
                    W->kolor = 'B';
                    Z->up->kolor = 'R';
                    rotacja_prawo(Z->up);
                    W = Z->up->left;
                }
                if ((W->left->kolor == 'B') && (W->right->kolor == 'B')) {              
                    W->kolor = 'R';
                    Z = Z->up;
                    continue;
                }
                if (W->left->kolor == 'B'){             
                    W->right->kolor = 'B';
                    W->kolor = 'R';
                    rotacja_lewo(W);
                    W = Z->up->left;
                }
                W->kolor = Z->up->kolor;  
                Z->up->kolor = 'B';
                W->left->kolor = 'B';
                rotacja_prawo(Z->up);
                Z = root;      
            }
    Z->kolor = 'B';
    delete Y;
}

int main()
{
    srand(time(NULL)); 
    int tab[MAXN];       
    int i, x, i1, i2;

    cr = cl = cp = "  ";
    cr[0] = 218; cr[1] = 196;
    cl[0] = 192; cl[1] = 196;
    cp[0] = 179;
    S.kolor = 'B';             
    S.up = &S;
    S.left = &S;
    S.right = &S;
    root = &S;

    cout << "DRZEWO RBT\n";
    cout << "Wybierz odpowiednia cyfre:\n1 Automatyczne dodanie 30 liczb do drzewa nastepnie usuniecie 15\n2 Reczne dodanie oraz usuniecie wybranej ilosci liczb w drzewie\n";
    cin >> i;
    if (i == 1) {
        for (i = 0; i < MAXN; i++) 
        tab[i] = i + 1;
        for (i = 0; i < 300; i++) {
        i1 = rand() % MAXN; 
        i2 = rand() % MAXN;
        x = tab[i1];       
        tab[i1] = tab[i2];
        tab[i2] = x;
        }
        for (i = 0; i < MAXN; i++) {
        cout << tab[i] << " ";
        dodaj_wezel(tab[i]);
        }
        cout << endl << endl;
        drukuj_drzewo("", "", root);
        cout << endl << endl;
        for (i = 0; i < 300; i++) 
        {
        i1 = rand() % MAXN; i2 = rand() % MAXN;
            x = tab[i1]; tab[i1] = tab[i2]; tab[i2] = x;
        }
        for (i = 0; i < MAXN >> 1; i++) 
        {
        cout << tab[i] << " ";
        usun_wezel(szukaj_wezel(tab[i]));
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
            dodaj_wezel(x);
        }
        drukuj_drzewo("", "", root);
        cout << "Ile liczb chcesz usunac?\n";
        cin >> i;
        cout << "Podaj wybrane liczby z wczesniej dodanych:\n";
        for (i; i > 0; i--) {
            cin >> x;
            usun_wezel(szukaj_wezel(x));
        }
        drukuj_drzewo("", "", root);
        usun_drzewo(root);
    }
    

    return 0;
}
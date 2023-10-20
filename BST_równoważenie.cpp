#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct Wezel
{
    Wezel * up, * left, * right;
    int wartosc;
};

void wypisanie_drzewka(Wezel * v)
{
    if (v) {
        wypisanie_drzewka(v->left);
        cout<<v->wartosc<<" ";
        wypisanie_drzewka(v->right);
    }
}

void usuwanie_drzewka(Wezel* v)
{
    if (v) {
        usuwanie_drzewka(v->left);
        usuwanie_drzewka(v->right);
        delete v;
    }
}

void wstawianie_do_drzewka(Wezel* &root, int k)
{
    Wezel* w, * p;
    w = new Wezel;         
    w->left = w->right = NULL; 
    w->wartosc = k;              
    p = root;               
    if (!p) root = w;              
    else
        while (1)           
            if (k < p->wartosc)       
            {                     
                if (!p->left)      
                {
                    p->left = w;      
                    break;           
                }
                else p = p->left;
            }
            else
            {
                if (!p->right)     
                {
                    p->right = w;   
                    break;         
                }
                else p = p->right;
            }
    w->up = p;         
}

Wezel* przeszukiwanie_drzewka(Wezel* p, int k)
{
    while (p && p->wartosc != k)
        p = (k < p->wartosc) ? p->left : p->right;
    return p;
}

Wezel* minimum_drzewka(Wezel* p)
{
    if (p) {
    	while (p->left) 
		p = p->left;
	}
    return p;
}

Wezel* succBST(Wezel* p)
{
    Wezel* r;
    if (p)
    {
        if (p->right) return minimum_drzewka(p->right);
        else
        {
            r = p->up;
            while (r && (p == r->right))
            {
                p = r;
                r = r->up;
            }
            return r;
        }
    }
    return p;
}

void usuniecie_elementu_z_drzewka(Wezel* &root, Wezel* X)
{
    Wezel* Y, * Z;
    if (X)
    {
        Y = !X->left || !X->right ? X : succBST(X);
        Z = Y->left ? Y->left : Y->right;
        if (Z) Z->up = Y->up;
        if (!Y->up) root = Z;
        else if (Y == Y->up->left) Y->up->left = Z;
        else Y->up->right = Z;
        if (Y != X) X->wartosc = Y->wartosc;
        delete Y;
    }
}

void rot_L ( Wezel * & root, Wezel * A )
{
  Wezel * B = A->right, * p = A->up;
  if( B ) {
    A->right = B->left;
    if( A->right ) A->right->up = A;
    B->left = A;
    B->up = p;
    A->up = B;
    if( p ) {
      if( p->left == A ) p->left = B; else p->right = B;
    }
    else root = B;
  }
}

void rot_R ( Wezel * & root, Wezel * A )
{
  Wezel * B = A->left, * p = A->up;
  if( B )
  {
    A->left = B->right;
    if( A->left ) A->left->up = A;
    B->right = A;
    B->up = p;
    A->up = B;
    if( p )
    {
      if( p->left == A ) p->left = B; 
	  else p->right = B;
    }
    else root = B;
  }
}

unsigned log2 ( unsigned x )
{
  unsigned y = 1;
  while( ( x >>= 1 ) > 0 ) y <<= 1;
  return y;
}

void rebalanceDSW ( Wezel * & root )
{
  unsigned n, i, s;
  Wezel * p;
  n = 0;                          
  p = root;           
  while( p )   
    if( p->left ) {
       rot_R ( root, p );  
       p = p->up;
    }
    else {
      n++;               
      p = p->right;  
    }
  s = n + 1 - log2 ( n + 1 ); 
  p = root;                       
  for( i = 0; i < s; i++ ) {
    rot_L ( root, p );
    p = p->up->right;
  }
  n -= s;   
  while( n > 1 ) {
    n >>= 1;  
    p = root; 
    for( i = 0; i < n; i++ ) {
      rot_L ( root, p );
      p = p->up->right;
    }
  }
}

int main() {
	
    int i, x, ile;
    Wezel* root = NULL;
    int option = 0;
    cout<<"Drzewko BST:\n";
    cout<<"--------------------------------------\n";
    cout<<"[0] Wyjscie z programu\n";
    cout<<"[1] Dodaj liczbe do drzewka\n";
    cout<<"[2] Wyszukaj liczbe w drzewku\n";
    cout<<"[3] Usun liczbe z drzewka\n";
    cout<<"[4] Wypisz wszystkie wezly drzewka (LVR-in-order)\n";
    cout<<"--------------------------------------\n\n";
    cout<<"Wybrana komenda (numer 0-4):\n";
    cin >> option;
    while (option != 0) {
        switch (option) {
        case 1:
            cout << "Podaj ile liczb chcesz dodac do drzewka:" << endl;
            cin >> ile;
            cout << "Podaj zadeklarowana ilosc liczb:" << endl;
            for (i = 0; i < ile; i++) {
                cin >> x;
                wstawianie_do_drzewka(root, x);
            }
            cout << "Dodano elementy do drzewka :)";
            break;
        case 2:
            cout << "Podaj liczbe ktora chcesz znalezc:" << endl;
            cin >> x;
            if (przeszukiwanie_drzewka(root, x) == NULL)
                cout << "Nie znaleziono elementu :(" << endl;
            else
                cout << "Znaleziono element " << x << " :)" << endl;
            break;
        case 3: 
            cout << "Podaj liczbe ktora chcesz usunac:" << endl;
            cin >> x;
            if (przeszukiwanie_drzewka(root, x)==NULL) cout << "Nie znaleziono podanego elementu :(" << endl;
            else {
                usuniecie_elementu_z_drzewka(root, przeszukiwanie_drzewka(root, x));
                cout << "Usunieto element o wartosci " << x << " :)" << endl;
            }
            break;
        case 4:
        	cout<<"Elementy drzewa LVR in-order:\n";
            wypisanie_drzewka(root);
            break;
        default:
        	cout<<"Niepoprawna komenda\n";
            break;
        }
        cout<<"\n\nWybrana komenda:\n";
        cin >> option;
    }
    cout<<"Rownowazenie drzewa binarnego:\n";
    rebalanceDSW ( root );
    wypisanie_drzewka(root);
    cout<<endl;
    usuwanie_drzewka(root);
    cout << "Zwolnienie pamieci - usuniecie drzewa BST" << endl;

    return 0;
}

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Element
{
    Element* next;
    string wyraz;
};

int haszowanie(string s)
{
    int h = 0, i;
    for (i = 0; i < s.length(); i++)    h = 31 * h + s[i] - 65;
    return h % 10;
}

void dodaj_element(string s, Element *tab[10], bool w) {
    Element* p, * r;
    int j, h;
    bool t;
    if (w) {
        s = "";
        for (j = 0; j < 3; j++) s += 65 + (rand() % 26);
    }
    else {
        cout << "Podaj element:\n";
        cin >> s;
    }
    h = haszowanie(s);
    p = tab[h];
    t = true;
    if (p)
        while (true)
        {
            if (p->wyraz == s) {
                t = false;
                break;
            }
            if (!p->next) break;
            p = p->next;
        }
    if (t) {
        r = new Element;
        r->wyraz = s;
        r->next = NULL;
        if (!p) tab[h] = r;
        else p->next = r;
    }
}

void wypisz_tablice(Element *tab[10]) {
    Element* p;
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        cout << "tab [ " << i << " ] = ";
        p = tab[i];
        while (p)
        {
            cout << p->wyraz << " ";
            p = p->next;
        }
        cout << endl;
    }
    cout << endl;
}

void szukaj_elementu(Element *tab[10], string ss) {
    Element* p;
    int h;
    h = haszowanie(ss);
    p = tab[h];
    while (p && (p->wyraz != ss)) {
        p = p->next;
    }
    if (p) cout << "Znaleziono element "<< ss << endl;
    else cout << "Nie znaleziono elementu.";
    cout << endl;
}

void usuwanie_elementu(Element* tab[10], string ss) {
    Element* p, * r;
    int h;
    h = haszowanie(ss);
    p = tab[h];
    if (p) r = p->next;
    else return;
    if ((p->wyraz == ss)) {
        tab[h] = r;
        delete p;
        return;
    }
    while (r && (r->wyraz != ss)) {
        p = r;
        r = r->next;
    }
    if (p) {
        if (r->next) p->next = r->next;
        else p->next = NULL;
        delete r;

        cout << "Usunieto " << ss << endl;
    }
    else cout << "Nie znaleziono elementu do usuniecia.";
    cout << endl;
}


void usuwanie_tablicy(Element *tab[10]) {
    Element* p;
    int i = 0;
    for (i = 0; i < 10; i++)
        while ((p = tab[i]))
        {
            tab[i] = p->next;
            p->wyraz = "";
            delete p;
        }
}

int main()
{
    srand(time(NULL));

    Element* tab[10];
    int i;
    unsigned int n;
    string s, ss;
    bool w;
    for (i = 0; i < 10; i++) tab[i] = NULL;
    cout << "TABLICA HASZOWA WYRAZOW 3-LITEROWYCH\n\n";
    cout << "Ile elementow chcesz dodac do tablicy?\n";
    cin >> n;
    cout << "Czy chcesz dodac elementy recznie ? \n";
    cout << "(Wybierz 0 jesli tak, 1 lub inna liczba zeby dodac automatycznie)\n";
    cin >> i;
    if (i == 0) w = false;
    else w = true;
    for (i = 0; i < n; i++) dodaj_element(s, tab, w);
    cout << "Wypisanie tablicy:\n";
    wypisz_tablice(tab);
    cout << "Podaj element jaki chcesz znalezc:\n";
    cin >> ss;
    szukaj_elementu(tab,ss);
    cout << "Podaj element jaki chcesz usunac:\n";
    cin >> ss;
    usuwanie_elementu(tab, ss);
    wypisz_tablice(tab);
    usuwanie_tablicy(tab);
    return 0;
}
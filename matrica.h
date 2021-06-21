#ifndef MATRICA_H
#define MATRICA_H
#include<iostream>
#include<vector>
#include<stdexcept>
#include<algorithm>
using namespace std;

struct Clan{
    int kolona;                                                                                 //indeks kolone clana u matrici
    double vrijednost;                                                                          //sadrzaj clana u matrici koji se nalazi u odredjenoj koloni
    Clan *sljedeci;                                                                             //pokazivac na sljedeci clan u matrici
};

struct Red{
    int indeks;                                                                                 //indeks elementa u redu matrice
    Clan *clanovi;                                                                              //pokazivac na clanove reda
    Clan *trenutni=nullptr;                                                                     //pokazivac na trenutni clan u redu
    Red *sljedeci;                                                                              //pokazivac na sljedeci red u matrici
};
//definisemo klasu matrica koju æemo u konaènici koristit za rad sa rijetkim matricama
class Matrica{
private:
    int brojRedova;                                                                             //broj redova u matrici
    int brojKolona;                                                                             //broj kolona u matrici
    Red *matrica=nullptr;                                                                       //pokazivac na prvi red u matrici, nullptr iz razloga sto pretpostavljamo da na pocetku u matrici nema clanova
    void kreirajMatricuBrzo(const vector<pair<pair<int,int>,double>> &clanovi);                 //pod brzo podrazumijevamo da imamo sortirane parove indeksa
    void dodajClanUMatricu(int indeksReda, int indeksKolone, double clan);                      //dodavanje clana u matricu ovisno od toga koji je uslov zadovoljen
    static Clan *ubaciClanURed(Red *trenutniRed, Clan *clanNaKojiDodajemo, Clan *noviClan);     //funkcija koja vraæa pokazivac na clanove
    Red *dodajRedUMatricu(Red *prviRed, Red *red, bool minus);                                  //pomocna funkcija koja vraca pokazivac na red koji dodajemo u matricu
    Red *spojiRedUMatrici(Red *redMatrice, Red *novi);                                          //pomocna funkcija pomoæu koje spajamo postojeci red u matrici sa novim redom, ukoliko redMatrice vec postoji u matrici
    static Matrica SabiranjeOduzimanje(const Matrica &matrica1, const Matrica &matrica2,char operacija);//funkcija sa ogranicenim pristupom, pozivamo je u svrhu sabiranja ili oduzimanja dvije matrice
    static double pozoviOperaciju(double operand1, double operand2, char operacija);            //provjera operacija i na osnovu toga vraca zbir ili razliku operanada
    //u public dijelu definisemo konstruktore, destruktor, operatore za rad sa matricama, funkcije za stepenovanje i transponovanje matrice
public:
    Matrica(int brojRedova, int brojKolona):brojRedova(brojRedova),brojKolona(brojKolona){}     //konstruktor koji postavlja redove i kolone kako bi osigurali velièinu matrice, moze se koristiti i sintaksa this.brojRedova=brojRedova
    Matrica(int brojRedova, int brojKolona,const vector<pair<pair<int,int>,double>>&clanovi, bool jeLiOptimizovana=true); //konstruktor pomocu kojeg obezbjeðujemo optimizovan prenos vrijednosti
    Matrica(const Matrica &matrica1);                                                           //kontruktor kopije, dozvoljava nam kopiranje vrijednosti iz jednog objekta matrice u drugi
    ~Matrica();                                                                                 //destruktor, pomoæu njega oslobaðamo memoriju koju je objekat zauzeo tokom svog postojanja u programu
    Matrica &operator=(const Matrica &matrica1);                                                //operator dodjele, provjeravamo da li zeli korisnik naseg programa da izvrsi samododjelu
    Matrica stepenovanjeMatrice(int stepen);                                                    //pomoæna funkcija koju koristimo za stepenovanje matrica
    Matrica &Pow(int stepen);                                                                             //pomoæu ove funkcije stepenujemo matricu nad kojom se poziva
    Matrica transponovanjeMatrice();                                                            //funkcija pomoæu koje transponujemo matricu
    Matrica &Transponuj();                                                                      //pomoæna funkcija na èiji poziv se transponuje objekat
    double clanPretraga(int indeksReda, int indeksKolone) const;                                //pomocna funkcija za pretragu elementa
    //ovdje definisemo operatore ispisa, oduzimanja, sabiranja, mnozenja i dijeljenja, kao i neke dodatne operatore koji se pozivaju na ove prethodno definisane
    //ove operatore definisemo kao friend operatori kako bismo im dozvolili pristup privatnim atributima klase matrica koju smo definisali
    friend ostream &operator<<(ostream &ispis, const Matrica &matrica1);                        //funkcija koja se poziva nad nekom matricom i ispisuje sadrzaj te matrice
    friend Matrica operator-(const Matrica &prva, const Matrica &druga);                        //provjeravamo da li su matrice istih dimenzija kako bi se nad njima izvrsila operacija oduzimanja matrica
    friend Matrica operator+(const Matrica &prva, const Matrica &druga);                        //provjeravamo da li dvije matrice mozemo sabrati i ukoliko je to moguæe vrsimo operaciju sabiranja matrica
    friend Matrica operator*(const Matrica &prva, const Matrica &druga);                        //funkcija pomoæu koje raèunamo proizvod dvije matrice
    friend bool operator==(const Matrica &prva, const Matrica &druga);
    //u nastavku definisemo funkcije clanice odreðenih operatora
    Matrica &operator-=(const Matrica &matrica1);                                               //oduzimanje i dodjela vrijednosti na matricu (objekat) na koju se poziva ovaj operator
    Matrica &operator+=(const Matrica &matrica1);                                               //sabiranje i dodjela vrijednosti na matricu na koju se poziva ovaj operator
    Matrica &operator*=(const Matrica &matrica1);                                               //mnozenje i dodjela vrijednosti na matricu na koju se poziva ovaj operator
    Matrica operator/(const double &konstanta);                                                 //ovaj operator dijeli matricu sa konstantom koja je proslijeðena i vraæa rezultat dijeljenja
    Matrica operator*(const double &konstanta);                                                 //ovaj operator mnozi matricu sa konstantom i vraæa rezultat tog mnozenja
    Matrica &operator*=(const double &konstanta);                                               //clanove u matrici koji se nalaze u nenultim redovima matrice mnozi sa odreðenim konstantama i vraæa rezultat tog mnoenja
    Matrica &operator/=(const double &konstanta);                                               //vrijednost clanova dijelimo sa konstantom ukoliko su ispunjeni odreðeni uslovi i shodno tome operator vraæa rezultat ili izuzetak

};


#endif //MATRICA_H


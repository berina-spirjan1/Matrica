#include<iostream>
#include<vector>
#include<stdexcept>
#include "matrica.h"
using namespace std;
int main(){
    vector<pair<pair<int,int>,double>> clanovi1{
        {{0,3},1},
        {{2,1},1},
        {{2,4},8},
        {{3,0},66},
        {{3,2},3}
    };
    vector<pair<pair<int,int>,double>> clanovi2{
        {{0,0},20},
        {{1,4},2},
        {{2,4},6},
        {{4,0},12}
    };
    vector<pair<pair<int,int>,double>> clanoviProdukt1{
        {{2,0},96},
        {{2,4},2},
        {{3,0},1320},
        {{3,4},18}
    };
    vector<pair<pair<int,int>,double>> clanovi3{
        {{0,0},5},
        {{0,1},3},
        {{0,2},7},
        {{0,3},8},
        {{1,0},9},
        {{1,1},15},
        {{1,2},7},
        {{1,3},25}
    };
    vector<pair<pair<int,int>,double>> clanovi4{
        {{0,0},2},
        {{0,1},3},
        {{0,2},4},
        {{0,3},5},
        {{1,0},6},
        {{1,1},7},
        {{1,2},8},
        {{1,3},9}
    };
    vector<pair<pair<int,int>,double>> clanoviProdukt2{
        {{0,0},7},
        {{0,1},6},
        {{0,2},11},
        {{0,3},13},
        {{1,0},15},
        {{1,1},22},
        {{1,2},15},
        {{1,3},34}
    };
     vector<pair<pair<int,int>,double>> clanovi5{
        {{0,0},12},
        {{0,2},5},
        {{1,2},9},
        {{2,1},14},
    };
     vector<pair<pair<int,int>,double>> clanovi6{
        {{0,0},3},
        {{0,2},5},
        {{1,1},14},
        {{1,2},9},
        {{2,2},89},
    };

    Matrica m1(5, 5, clanovi1, false);
    Matrica m2(5, 5, clanovi2, false);
    Matrica produkt1(5, 5, clanoviProdukt1, false);
    Matrica m3(2, 3, clanovi3);
    Matrica m4(2, 3, clanovi4);
    Matrica m5(2, 3, clanovi4);
    Matrica m6(3,3,clanovi5);
    Matrica m7(3,3,clanovi6);
    Matrica produkt2(2, 3, clanoviProdukt2);
    cout<<"Mnozenje dvije matrice:"<<endl;
    cout << m1 * m2;
    cout << endl << "Rezultujuca matrica treba biti" << produkt1;
    cout<<"Sabiranje dvije matrice: "<<endl<<m3+m4;
    cout << endl << "Rezultujuca matrica treba biti" << produkt2;
    cout<<"Preklopljeni operator== za matrice. Primjer u nastavku."<<endl;
    if(m3==m4)
        cout<<"Matrice m3 i m4 su jednake."<<endl;
    else
        cout<<"Matrice m3 i m4 nisu jednake."<<endl<<m3<<m4<<endl;
    cout<<endl;
    if(m4==m5)
        cout<<"Matrice m4 i m5 su jednake."<<endl<<m4<<m5<<endl;
    else;
    cout<<"Mnozenje matrice sa konstantom: "<<endl<<m3*2.3<<endl;
    cout<<"Dijeljenje matrice sa konstantom: "<<endl<<m3/5;
    cout<<"Transponovana matrica: "<<m7.Transponuj();
    cout<<"Stepenovanje matrice: "<<m6.Pow(2);
    //provjerimo sada validnost preklopljenih operatora
    m3-=m4;
    cout<<"Preklopljeni operator -= za matrice m3 i m4: "<<m3;
    m4+=m3*2;
    cout<<"Preklopljeni operator += za matrice m3 i m4: "<<m4;
    cout<<"Operator - za matrice m1 i m2: "<<m1-m2;
    cout<<"Operator + za matrice m1 i m2: "<<m1+m2;
    cout<<"Operator * za matrice m1 i m2: "<<m1*m2;
    cout<<"Pretraga clana u matrici, pomocna funkcija za mnozenje sto koristimo, clan na poziciji (0, 3) u matrici m1: "
    <<m1.clanPretraga(0,3);
    //operator dodjele
    Matrica a=m7;
    a*=a;
    cout<<"Preklopljeni operator *= za matricu a: "<<a;
    return 0;
}

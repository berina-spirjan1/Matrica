#ifndef MATRICA_CPP
#define MATRICA_CPP
#include <cmath>
#include <iomanip>
#include "matrica.h"

#define EPSILON 0.0001

double Matrica::clanPretraga(int indeksReda, int indeksKolone) const {
    if (indeksKolone > brojKolona - 1 || indeksKolone < 0)
        throw domain_error("Nepostojeci indeks kolone!");
    if (indeksReda > brojRedova - 1 || indeksReda < 0)
        throw domain_error("Nepostojeci indeks reda!");
    Red *red = nullptr;
    for (Red *i = matrica; i != nullptr; i = i->sljedeci) {
        if (i->indeks == indeksReda) {
            red = i;
            break;
        }
    }
    if (red) {
        Clan *kolona = nullptr;
        for (Clan *i = red->clanovi; i != nullptr; i = i->sljedeci) {
            if (i->kolona == indeksKolone) {
                kolona = i;
                break;
            }
        }
        if (kolona) {
            return kolona->vrijednost;
        }
        return 0;
    }
    return 0;
}

void Matrica::dodajClanUMatricu(int indeksReda, int indeksKolone, double clan) {
    if (indeksKolone > brojKolona - 1 || indeksKolone < 0)
        throw domain_error("Nepostojeci indeks kolone!");
    if (indeksReda > brojRedova - 1 || indeksReda < 0)
        throw domain_error("Nepostojeci indeks reda!");
    if (matrica == nullptr) {
        Clan *novi = new Clan{indeksKolone, clan};
        matrica = new Red{indeksReda, nullptr, nullptr};
        matrica->clanovi = novi;
        return;
    }
    Red *prethodniRed = nullptr;
    Red *trenutniRed = matrica;
    // while petlja pronalazi red koji je odmah prije novog(prethodniRed) i odmah poslije novog(trenutniRed) po indeksu
    // osim ako taj red vec postoji tada je trenutni red trazeni red
    while (trenutniRed != nullptr && trenutniRed->indeks < indeksReda) {
        if (prethodniRed != nullptr)
            prethodniRed = prethodniRed->sljedeci;
        else
            prethodniRed = matrica;
        trenutniRed = trenutniRed->sljedeci;
    }
    if (trenutniRed == nullptr || trenutniRed->indeks > indeksReda) {
        Red *noviRed = new Red{indeksReda, nullptr, nullptr};
        if (prethodniRed != nullptr) {
            prethodniRed->sljedeci = noviRed;
        } else
            matrica = noviRed;
        noviRed->sljedeci = trenutniRed;
        trenutniRed = noviRed;
    }
    Clan *trenutniClan = trenutniRed->clanovi;
    Clan *noviClan = new Clan{indeksKolone, clan, nullptr};
    if (trenutniClan != nullptr) {
        Clan *prethodniClan = nullptr;
        while (trenutniClan != nullptr && trenutniClan->kolona < indeksKolone) {
            if (prethodniClan != nullptr)
                prethodniClan = prethodniClan->sljedeci;
            else
                prethodniClan = trenutniClan;
            trenutniClan = trenutniClan->sljedeci;
        }
        if (trenutniClan != nullptr && trenutniClan->kolona == indeksKolone) {
            trenutniClan->vrijednost = clan;
            delete noviClan;
        } else if (trenutniClan == nullptr || trenutniClan->kolona > indeksKolone) {
            if (prethodniClan != nullptr) {
                prethodniClan->sljedeci = noviClan;
            } else {
                trenutniRed->clanovi = noviClan;
            }
            noviClan->sljedeci = trenutniClan;
        } else {
            throw logic_error("Imamo definisanje clana matrice više puta!");
        }
    } else
        trenutniRed->clanovi = noviClan;
}


Matrica operator*(const Matrica &prva, const Matrica &druga) {
    if (prva.brojKolona != druga.brojRedova)
        throw domain_error("Matrice nisu saglasne za mnozenje!");
    Matrica M(prva.brojRedova, druga.brojKolona);
    for (int i = 0; i < prva.brojRedova; i++) {
        for (int j = 0; j < druga.brojKolona; j++) {
            double zbir = 0;
            for (int k = 0; k < prva.brojKolona; k++) {
                zbir += prva.clanPretraga(i, k) * druga.clanPretraga(k, j);
            }
            if (zbir != 0)
                M.dodajClanUMatricu(i, j, zbir);
        }
    }
    return M;
}

void Matrica::kreirajMatricuBrzo(const vector<pair<pair<int, int>, double>> &clanovi) {
    if (clanovi.size()!=0) {
        Red *red = matrica;
        int j = 0;
        for (int i = 0; i < brojRedova; i++) {
            if (clanovi[j].first.first != i)
                continue;

            Red *noviRed = new Red{i, nullptr, nullptr};
            Clan *clan = noviRed->clanovi;

            while (clanovi[j].first.first == i && j < clanovi.size()) {
                Clan *novi = new Clan{clanovi[j].first.second, clanovi[j].second, nullptr};
                if (clan != nullptr) {
                    clan->sljedeci = novi;
                    clan = clan->sljedeci;
                } else {
                    noviRed->clanovi = novi;
                    clan = novi;
                }

                j++;
            }

            if (red != nullptr) {
                red->sljedeci = noviRed;
                red = red->sljedeci;
            } else {
                matrica = noviRed;
                red = noviRed;
            }
        }
    } else
        return;
}

Clan *Matrica::ubaciClanURed(Red *trenutniRed, Clan *clanNaKojiDodajemo, Clan *noviClan) {
    if (clanNaKojiDodajemo != nullptr) {
        clanNaKojiDodajemo->sljedeci = noviClan;
        clanNaKojiDodajemo = clanNaKojiDodajemo->sljedeci;
    } else {
        trenutniRed->clanovi = noviClan;
        clanNaKojiDodajemo = noviClan;
    }
    return clanNaKojiDodajemo;
}

Red *Matrica::spojiRedUMatrici(Red *redMatrice, Red *novi) {
    if (redMatrice != nullptr) {
        redMatrice->sljedeci = novi;
        redMatrice = novi;
    } else {
        matrica = novi;
        redMatrice = novi;
    }
    return redMatrice;
}

Red *Matrica::dodajRedUMatricu(Red *prviRed, Red *red, bool minus = false) {
    Red *noviRed = new Red{prviRed->indeks, nullptr, nullptr};
    red = spojiRedUMatrici(red, noviRed);
    Clan *clan = prviRed->clanovi;
    Clan *clanDodavanje = red->clanovi;
    while (clan != nullptr) {
        double novaVrijednost = clan->vrijednost;
        if (minus) {
            novaVrijednost = Matrica::pozoviOperaciju(0, clan->vrijednost, '-');
        }
        Clan *noviClan = new Clan{clan->kolona, novaVrijednost, nullptr};
        clanDodavanje = Matrica::ubaciClanURed(red, clanDodavanje, noviClan);
        clan = clan->sljedeci;
    }
    return red;
}

Matrica Matrica::SabiranjeOduzimanje(const Matrica &matrica1, const Matrica &matrica2, char operacija) {
    Matrica M(matrica1.brojRedova, matrica1.brojKolona); //krajnja rezultujuæa matrica
    Red *redPrve = matrica1.matrica;
    Red *redDruge = matrica2.matrica;
    Red *red = M.matrica;
    while (redPrve != nullptr && redDruge != nullptr) {
        if (redPrve->indeks > redDruge->indeks) {
            red = M.dodajRedUMatricu(redDruge, red, true);
            redDruge = redDruge->sljedeci;
        } else if (redPrve->indeks < redDruge->indeks) {
            red = M.dodajRedUMatricu(redPrve, red);
            redPrve = redPrve->sljedeci;
        } else {
            Red *noviRed = new Red{redPrve->indeks, nullptr, nullptr};
            red = M.spojiRedUMatrici(red, noviRed);
            Clan *clanPrve = redPrve->clanovi;
            Clan *clanDruge = redDruge->clanovi;
            Clan *clan = red->clanovi;
            while (clanPrve != nullptr && clanDruge != nullptr) {
                if (clanPrve->kolona > clanDruge->kolona) {
                    Clan *noviClan = new Clan{clanDruge->kolona,
                                              Matrica::pozoviOperaciju(0, clanDruge->vrijednost, operacija), nullptr};
                    clan = Matrica::ubaciClanURed(red, clan, noviClan);
                    clanDruge = clanDruge->sljedeci;
                } else if (clanPrve->kolona < clanDruge->kolona) {
                    Clan *noviClan = new Clan{clanPrve->kolona, clanPrve->vrijednost, nullptr};
                    clan = Matrica::ubaciClanURed(red, clan, noviClan);
                    clanPrve = clanPrve->sljedeci;
                } else {
                    Clan *noviClan = new Clan{clanPrve->kolona,
                                              Matrica::pozoviOperaciju(clanPrve->vrijednost, clanDruge->vrijednost,
                                                                       operacija), nullptr};
                    clan = Matrica::ubaciClanURed(red, clan, noviClan);
                    clanPrve = clanPrve->sljedeci;
                    clanDruge = clanDruge->sljedeci;
                }
            }
            while (clanPrve != nullptr) {
                Clan *noviClan = new Clan{clanPrve->kolona, clanPrve->vrijednost, nullptr};
                clan = Matrica::ubaciClanURed(red, clan, noviClan);
                clanPrve = clanPrve->sljedeci;
            }
            while (clanDruge != nullptr) {
                Clan *noviClan = new Clan{clanDruge->kolona,
                                          Matrica::pozoviOperaciju(0, clanDruge->vrijednost, operacija), nullptr};
                clan = Matrica::ubaciClanURed(red, clan, noviClan);
                clanDruge = clanDruge->sljedeci;
            }
            redPrve = redPrve->sljedeci;
            redDruge = redDruge->sljedeci;
        }
    }
    while (redPrve != nullptr) {
        Red *noviRed = new Red{redPrve->indeks, nullptr, nullptr};
        red = M.spojiRedUMatrici(red, noviRed);
        Clan *clanPrve = redPrve->clanovi;
        Clan *clan = red->clanovi;
        while (clanPrve != nullptr) {
            Clan *noviClan = new Clan{clanPrve->kolona, clanPrve->vrijednost, nullptr};
            clan = Matrica::ubaciClanURed(red, clan, noviClan);
            clanPrve = clanPrve->sljedeci;
            redPrve = redPrve->sljedeci;
        }
        while (redDruge != nullptr) {
            Red *noviRed = new Red{redDruge->indeks, nullptr, nullptr};
            red = M.spojiRedUMatrici(red, noviRed);
            Clan *clanDruge = redDruge->clanovi;
            Clan *clan = red->clanovi;
            while (clanDruge != nullptr) {
                Clan *noviClan = new Clan{clanDruge->kolona,
                                          Matrica::pozoviOperaciju(0, clanDruge->vrijednost, operacija), nullptr};
                clan = Matrica::ubaciClanURed(red, clan, noviClan);
                clanDruge = clanDruge->sljedeci;
            }
            redDruge = redDruge->sljedeci;
        }
        return M;
    }
}

double Matrica::pozoviOperaciju(double operand1, double operand2, char operacija) {
    if (operacija == '-')
        return operand1 - operand2;
    else if (operacija == '+')
        return operand1 + operand2;
    else
        return 0;
}

Matrica::Matrica(int brojRedova, int brojKolona, const vector<pair<pair<int, int>, double>> &clanovi,
                 bool jeLiOptimizovana) : brojRedova(brojRedova), brojKolona(brojKolona) {
    if (jeLiOptimizovana) {
        kreirajMatricuBrzo(clanovi);
    } else {
        for (int i = 0; i < clanovi.size(); i++) {
            dodajClanUMatricu(clanovi[i].first.first, clanovi[i].first.second, clanovi[i].second);
        }
    }
}

Matrica::Matrica(const Matrica &matrica1) {
    brojRedova = matrica1.brojRedova;
    brojKolona = matrica1.brojKolona;
    Red *redKopira = matrica1.matrica;
    Red *redKreira = matrica;
    while (redKopira != nullptr) {
        Red *noviRed = new Red{redKopira->indeks, nullptr, nullptr};
        if (matrica != nullptr) {
            redKreira->sljedeci = noviRed;
            redKreira = redKreira->sljedeci;
        } else {
            matrica = noviRed;
            redKreira = noviRed;
        }
        Clan *clanovi = redKopira->clanovi;
        Clan *clanoviKreira = nullptr;
        while (clanovi != nullptr) {
            Clan *noviClan = new Clan{clanovi->kolona, clanovi->vrijednost, nullptr};
            if (clanoviKreira != nullptr) {
                clanoviKreira->sljedeci = noviClan;
                clanoviKreira = clanoviKreira->sljedeci;
            } else {
                redKreira->clanovi = noviClan;
                clanoviKreira = noviClan;
            }
            clanovi = clanovi->sljedeci;
        }
        redKopira = redKopira->sljedeci;
    }
}

Matrica::~Matrica() {
    Red *red = matrica;
    while (red != nullptr) {
        Clan *clanovi = red->clanovi;
        Red *trenutniRed = red;
        while (clanovi != nullptr) {
            Clan *trenutni = clanovi;
            Clan *sljedeci = trenutni->sljedeci;
            clanovi = sljedeci;
            trenutni->sljedeci = nullptr;
            delete trenutni;
        }
        red = red->sljedeci;
        delete trenutniRed;
    }
}

Matrica &Matrica::operator=(const Matrica &matrica1) {
    if (this != &matrica1) {
        Red *red = matrica;
        while (red != nullptr) {
            Clan *clanovi = red->clanovi;
            Red *trenutniRed = red;
            while (clanovi != nullptr) {
                Clan *trenutni = clanovi;
                Clan *sljedeci = trenutni->sljedeci;
                clanovi = sljedeci;
                trenutni->sljedeci = nullptr;
                delete trenutni;
            }
            red = red->sljedeci;
            delete trenutniRed;
        }
        matrica = nullptr;
        brojRedova = matrica1.brojRedova;
        brojKolona = matrica1.brojKolona;
        red = matrica1.matrica;
        Red *redProlaz = matrica;
        while (red != nullptr) {
            Red *noviRed = new Red{red->indeks, nullptr, nullptr};
            if (matrica != nullptr) {
                redProlaz->sljedeci = noviRed;
                redProlaz = redProlaz->sljedeci;
            } else {
                matrica = noviRed;
                redProlaz = noviRed;
            }
            Clan *clanovi = red->clanovi;
            Clan *clanoviProlaz = nullptr;
            while (clanovi != nullptr) {
                Clan *noviClan = new Clan{clanovi->kolona, clanovi->vrijednost, nullptr};
                if (clanoviProlaz != nullptr) {
                    clanoviProlaz->sljedeci = noviClan;
                    clanoviProlaz = clanoviProlaz->sljedeci;
                } else {
                    redProlaz->clanovi = noviClan;
                    clanoviProlaz = noviClan;
                }
                clanovi = clanovi->sljedeci;
            }
            red = red->sljedeci;
        }
        return *this;
    } else
        return *this;
}


Matrica Matrica::stepenovanjeMatrice(int stepen) {
    if (stepen < 0)
        throw domain_error("Stepen je negativan! Potrebno je omoguciti nenegativan!");
    Matrica rezultujucaMatrica(*this), dodatnaMatrica(*this);
    if(stepen%2 == 1) {
        dodatnaMatrica *= rezultujucaMatrica;
        for(int i = 1; i<stepen/2; i++){
            rezultujucaMatrica *= dodatnaMatrica;
        }
    } else {
        dodatnaMatrica *= rezultujucaMatrica;
        for(int i = 1; i<stepen/2 - 1; i++){
            rezultujucaMatrica *= dodatnaMatrica;
        }
        rezultujucaMatrica *= (*this);
    }
    return rezultujucaMatrica;
}

Matrica &Matrica::Pow(int stepen) {
    *this = stepenovanjeMatrice(stepen);
    return *this;
}


Matrica Matrica::transponovanjeMatrice() {
    Matrica transponovanaMatrica(brojKolona, brojRedova);
    Red *red = matrica;
    while (red != nullptr) {
        Clan *clanovi = red->clanovi;
        while (clanovi != nullptr) {
            transponovanaMatrica.dodajClanUMatricu(clanovi->kolona, red->indeks, clanovi->vrijednost);
            clanovi = clanovi->sljedeci;
        }
        red = red->sljedeci;
    }
    return transponovanaMatrica;
}

Matrica &Matrica::Transponuj() {
    *this = transponovanjeMatrice();
    return *this;
}

ostream &operator<<(ostream &ispis, const Matrica &matrica1) {
    int width = 5;
    ispis << endl << "Matrica (" << matrica1.brojRedova << ", " << matrica1.brojKolona << ")" <<  endl;
    Clan *trenutniClan = nullptr;
    Red *trenutniRed = matrica1.matrica;
    int indeksReda(0);

    while (trenutniRed != nullptr) {
        if (indeksReda != trenutniRed->indeks) {
            for (int i = 0; i < matrica1.brojKolona; i++) {
                ispis << setw(width) << 0;
            }
        } else {
            trenutniClan = trenutniRed->clanovi;
            int indeksKolone(0);
            while (trenutniClan != nullptr) {
                if (indeksKolone != trenutniClan->kolona)
                    ispis << setw(width) << 0;
                else {
                    ispis << setw(width) << trenutniClan->vrijednost;
                    trenutniClan = trenutniClan->sljedeci;
                }
                indeksKolone++;
            }
            if (indeksKolone != matrica1.brojKolona) {
                for (int j = indeksKolone; j < matrica1.brojKolona; j++) {
                    ispis << setw(width) << 0;
                }
            }
            trenutniRed = trenutniRed->sljedeci;
        }
        ispis << endl;
        indeksReda++;
    }
    if (indeksReda != matrica1.brojRedova) {
        for (int i = indeksReda; i < matrica1.brojRedova; i++) {
            for (int j = 0; j < matrica1.brojKolona; j++) {
                ispis << setw(width) << 0;
            }
            ispis << endl;
        }
    }
    return ispis;
}

Matrica operator-(const Matrica &prva, const Matrica &druga) {
    if (prva.brojKolona != druga.brojKolona || prva.brojRedova != druga.brojRedova)
        throw domain_error("Matrice nisu istih dimenzija, pa ih ne možemo oduzeti!");
    return Matrica::SabiranjeOduzimanje(prva, druga, '-');
}

Matrica operator+(const Matrica &prva, const Matrica &druga) {
    if (prva.brojKolona != druga.brojKolona || prva.brojRedova != druga.brojRedova)
        throw domain_error("Matrice nisu istih dimenzija, pa ih ne možemo sabrati");
    return Matrica::SabiranjeOduzimanje(prva, druga, '+');
}

Matrica &Matrica::operator-=(const Matrica &matrica1) {
    *this = *this - matrica1;
    return *this;
}

Matrica &Matrica::operator+=(const Matrica &matrica1) {
    *this = *this + matrica1;
    return *this;
}

Matrica &Matrica::operator*=(const Matrica &matrica1) {
    *this = (*this) * matrica1;
    return *this;
}

Matrica &Matrica::operator/=(const double &konstanta) {
    if (fabs(konstanta)-EPSILON>0) {
        Red *red = matrica;
        while (red != nullptr) {
            Clan *clanovi = red->clanovi;
            while (clanovi != nullptr) {
                clanovi->vrijednost /= konstanta;
                clanovi = clanovi->sljedeci;
            }
            red = red->sljedeci;
        }
        return *this;
    } else
        throw domain_error("Konstanta je jednaka nuli, pa dijeljenje s nulom nema smisla!");
}

Matrica Matrica::operator/(const double &konstanta) {
    if (fabs(konstanta)- EPSILON<0)
        throw domain_error("Dijeljenje s nulom nije dozvoljeno!");
    Matrica konacna(*this);
    konacna /= konstanta;
    return konacna;
}

Matrica &Matrica::operator*=(const double &konstanta) {
    Red *red = matrica;
    while (red != nullptr) {
        Clan *clanovi = red->clanovi;
        while (clanovi != nullptr) {
            clanovi->vrijednost *= konstanta;
            clanovi = clanovi->sljedeci;
        }
        red = red->sljedeci;
    }
    return *this;
}

Matrica Matrica::operator*(const double &konstanta) {
    Matrica konacna(*this);
    konacna *= konstanta;
    return konacna;
}

bool operator==(const Matrica &prva, const Matrica &druga) {
    Red *redDruge = druga.matrica;
    if (prva.brojRedova != druga.brojRedova || prva.brojKolona != druga.brojKolona) {
        return false;
    }
    // Prolazimo kroz redove prve matrice, a uz to pratimo pokazivacem u drugoj matrici tako da prolazimo istim tempom kroz matricu
    // prolazimo sve dok postoji sljedeci red, a to je kada on nije nullptr
    for (Red *red = prva.matrica; red != nullptr; red = red->sljedeci) {
        if (redDruge != nullptr) {
            Clan *clanDruge = redDruge->clanovi;
            if (red->indeks != redDruge->indeks) {
                return false;
            }
            // Prolazimo kroz clanove reda prve matrice, a uz to pratimo pokazivacem u drugoj matrici tako da prolazimo
            // istim tempom kroz matricu
            // prolazimo sve dok postoji sljedeci clan u redu, a to je kada on nije nullptr
            for (Clan *clan = red->clanovi; clan != nullptr; clan = clan->sljedeci) {
                if (clanDruge != nullptr) {
                    if(clan->kolona != clanDruge->kolona || fabs(clan->vrijednost - clanDruge->vrijednost) > EPSILON) {
                        return false;
                    }
                } else {
                    return false;
                }
                clanDruge = clanDruge->sljedeci;
            }
            // ukoliko ostane elemenata kojih nema u prvoj matrici, a u drugoj ima
            if (clanDruge != nullptr) {
                return false;
            }
        } else {
            return false;
        }
        redDruge = redDruge->sljedeci;
    }
    if (redDruge != nullptr) {
        return false;
    }
    return true;
}

#endif //MATRICA_CPP

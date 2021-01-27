//
// Created by dzena on 27/01/2021.
//

#ifndef ASP_VJEZBA9_HASHMAPA_H
#define ASP_VJEZBA9_HASHMAPA_H


#include <stdexcept>
#include "../Mapa/Mapa.h"
#include "../Par/Par.h"

template <typename TipKljuc, typename TipVrijednost>
class HashMapa : public Mapa<TipKljuc, TipVrijednost> {
public:
    int brEl, maxBrEl;
    Par<TipKljuc, TipVrijednost> **par;
    unsigned int (*f) (TipKljuc, unsigned int);
    HashMapa() : brEl(0), maxBrEl(100), par(new Par<TipKljuc, TipVrijednost>*[maxBrEl]{}), f(nullptr) {}

    ~HashMapa() {
        for(int i = 0; i < maxBrEl; ++i)
            delete par[i];
        delete[] par;
    }

    HashMapa(const HashMapa<TipKljuc, TipVrijednost> &m) : brEl(m.brEl), maxBrEl(m.maxBrEl), par(new Par<TipKljuc, TipVrijednost>*[m.maxBrEl]{}), f(m.f) {
        for(int i = 0; i < maxBrEl; i++)
            if(m.par[i])
                par[i] = new Par<TipKljuc, TipVrijednost>(*m.par[i]);
            else par[i] = nullptr;
    }

    HashMapa &operator =(const HashMapa<TipKljuc, TipVrijednost> &m) {
        if(&m == this) return *this;
        for(int i = 0; i < maxBrEl; ++i)
            delete par[i];
        delete[] par;
        brEl = m.brEl; maxBrEl = m.maxBrEl;
        par = new Par<TipKljuc, TipVrijednost>*[maxBrEl]{};
        f = m.f;
        for(int i = 0; i < maxBrEl; i++)
            if(m.par[i])
                par[i] = new Par<TipKljuc, TipVrijednost>(*m.par[i]);
            else par[i] = nullptr;
        return *this;
    }

    TipVrijednost operator[](TipKljuc index) const override {
        int i = f(index, maxBrEl);
        if(i == maxBrEl - 1) i = 0;
        for(int j = i + 1; j < maxBrEl; j++) {
            if(par[j] && par[j]->kljuc == index) {
                return par[j]->vrijednost;
            }
            if(j == maxBrEl - 1)
                j = -1;
            if(j == i)
                break;
        }
        return TipVrijednost();
    }

    TipVrijednost &operator[](TipKljuc index) override {
        if(!f)
            throw std::domain_error("Hash funkcija nije definisana");
        int pozicija = f(index, maxBrEl);
        if(pozicija == maxBrEl - 1) pozicija = 0;
        for(int i = pozicija + 1; i < maxBrEl; i++) {
            if(par[i] && par[i]->kljuc == index) {
                return par[i]->vrijednost;
            }
            if(i == maxBrEl - 1)
                i = -1;
            if(i == pozicija){
                pozicija = i;
                break;
            }
        }
        if((double(brEl) / maxBrEl) * 100 > 1) {
            maxBrEl *= 10;
            Par<TipKljuc, TipVrijednost> **temp = new Par<TipKljuc, TipVrijednost>*[maxBrEl]{};
            for(int i = 0; i < maxBrEl / 10; i++) {
                if(par[i]) {
                    int novaPozicija = f(par[i]->kljuc, maxBrEl);
                    if(novaPozicija == maxBrEl - 1) novaPozicija = 0;
                    if(temp[novaPozicija]) {
                        for(int j = novaPozicija + 1; j < maxBrEl; j++) {
                            if(!temp[j]) {
                                novaPozicija = j;
                                break;
                            }
                            if(j == maxBrEl - 1)
                                j = -1;
                        }
                    }
                    temp[novaPozicija] = new Par<TipKljuc, TipVrijednost>{par[i]->kljuc, par[i]->vrijednost};
                }
                delete par[i];
            }
            delete[] par;
            int pozicija2 = f(index, maxBrEl);
            if(temp[pozicija2]) {
                if(pozicija2 == maxBrEl - 1) pozicija2 = -1;
                for(int j = pozicija2 + 1; j < maxBrEl; j++) {
                    if(!temp[pozicija2]) {
                        pozicija2 = j;
                        break;
                    }
                    if(j == maxBrEl - 1)
                        j = -1;
                }
            }
            temp[pozicija2] = new Par<TipKljuc, TipVrijednost>{index, TipVrijednost()};
            par = temp;
            brEl++;
            return temp[pozicija2]->vrijednost;
        }
        else {
            if(!par[pozicija]) {
                par[pozicija] = new Par<TipKljuc, TipVrijednost>{index, TipVrijednost()};
                brEl++;
                return par[pozicija]->vrijednost;
            }
            else {
                if(pozicija == maxBrEl - 1) pozicija = 0;
                for(int i = pozicija + 1; i < maxBrEl; i++) {
                    if(!par[i]) {
                        par[i] = new Par<TipKljuc, TipVrijednost>{index, TipVrijednost()};
                        brEl++;
                        return par[i]->vrijednost;
                    }
                    if(i == maxBrEl - 1)
                        i = -1;
                    if(i == pozicija)
                        break;
                }
            }
        }
        return par[pozicija]->vrijednost;
    }

    int brojElemenata() const override {
        return brEl;
    }

    void obrisi() override {
        for(int i = 0; i < maxBrEl; ++i)
            delete par[i];
        maxBrEl = 100;
        brEl = 0;
        delete[] par;
        par = new Par<TipKljuc, TipVrijednost>*[maxBrEl]{};
    }

    void obrisi(const TipKljuc &k) override {
        if(!f)
            throw std::domain_error("Hash funkcija nije definisana");
        int pozicija = f(k, maxBrEl);
        if(par[pozicija] && par[pozicija]->kljuc == k) {
            delete par[pozicija];
            par[pozicija] = nullptr;
        }
        else {
            if(pozicija == maxBrEl - 1) pozicija = 0;
            for(int i = pozicija + 1; i < maxBrEl; i++) {
                if(par[i] && par[i]->kljuc == k) {
                    delete par[i];
                    par[i] = nullptr;
                    break;
                    if(i == maxBrEl - 1)
                        i = -1;
                    if(i == pozicija)
                        throw std::domain_error("Nema elementa u mapi");
                }
            }
        }
        brEl--;
    }
    void definisiHashFunkciju(unsigned int (*f) (TipKljuc, unsigned int)) {
        this->f = f;
    }
};


#endif //ASP_VJEZBA9_HASHMAPA_H

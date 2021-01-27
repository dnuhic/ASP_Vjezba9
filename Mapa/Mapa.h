//
// Created by dzena on 27/01/2021.
//

#ifndef ASP_VJEZBA9_MAPA_H
#define ASP_VJEZBA9_MAPA_H

template <typename TipKljuc, typename TipVrijednost>
class Mapa {
public:
    Mapa() = default;
    virtual ~Mapa() = default;
    virtual TipVrijednost operator[](TipKljuc index) const = 0;
    virtual TipVrijednost &operator[](TipKljuc index) = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuc &k) = 0;
};


#endif //ASP_VJEZBA9_MAPA_H

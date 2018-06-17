#ifndef __QUOTIENT_HPP__
#define __QUOTIENT_HPP__

#include"algebra.hpp"
#include"prime.hpp"

namespace quotient{

typedef long long ll;

struct FracType;

struct fraction{
    static FracType const* r;
    ll en, dn;
    fraction(ll en=0): en(en), dn(1){}
    fraction(ll en, ll dn): en(en), dn(dn){}
    void reduce(){
        int alter = en>=0 ^ dn>=0;
        en = std::abs(en);
        dn = std::abs(dn);
        int g = std::__gcd(en,dn);
        en /= g, dn /= g;
        if(alter) en = -en;
    }
    fraction operator+(fraction const& r)const{
        fraction out(en*r.dn+dn*r.en,dn*r.dn);
        out.reduce();
        return out;
    }

    fraction operator-(fraction const& r)const{
        fraction out(en*r.dn-dn*r.en,dn*r.dn);
        out.reduce();
        return out;
    }

    fraction operator*(fraction const& r)const{
        fraction out(en*r.en,dn*r.dn);
        out.reduce();
        return out;
    }
    fraction operator/(fraction const& r)const{
        if(r==0) throw std::exception();
        fraction out(en*r.dn, dn*r.en);
        out.reduce();
        return out;
    }
    fraction inv()const{
        fraction out = *this;
        if(out==0) throw std::exception();
        std::swap(out.en, out.dn);
        return out;
    }

    bool operator<(fraction const& r)const{
        return en*r.dn < dn*r.en;
    }
    bool operator==(fraction r)const{
        fraction l = *this;
        l.reduce(); r.reduce();
        return l.en == r.en && l.dn == r.dn;
    }

    friend std::ostream& operator<<(std::ostream& out, fraction const& x){
        out<<x.en<<"/"<<x.dn;
        return out;
    }

};

struct FracType{
    fraction __zero__()const{
        return fraction(0);
    }
    fraction __unit__(){
        return fraction(1);
    }
} fracType;

FracType const* fraction::r = &fracType;

}

#endif // __QUOTIENT_HPP__

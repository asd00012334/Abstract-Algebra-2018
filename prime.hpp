#ifndef __PRIME_HPP__
#define __PRIME_HPP__

#include<bits/stdc++.h>
#include"algebra.hpp"

namespace prime{

typedef long long int ll;
static const int primeRange = (1<<25);

int sieve[primeRange]={};
int primeList[primeRange]={};
int primeListSize = 0;

void buildPrimeList(){
    sieve[0]=true; sieve[1]=true;
    for(int i=2;i<primeRange;++i){
        if(!sieve[i]) primeList[primeListSize++]=i;
        for(int j=0;j<primeListSize && (ll)i*primeList[j]<primeRange;++j){
            sieve[(ll)i*primeList[j]]=true;
            if(i%primeList[j]==0) break;
        }
    }
}

template<typename type>
std::pair<type,type> extGCD(type left, type right){
    if(!left && !right) throw std::logic_error("Return value does not exist.\n");
    std::pair<type,type> lCor(1,0), rCor(0,1);
    if(left<0) lCor.first = -1;
    if(right<0) rCor.second = -1;
    left = abs(left);
    right = abs(right);
    while(left){
        std::swap(lCor,rCor);
        std::swap(left,right);
        lCor.first -= left/right*rCor.first;
        lCor.second -= left/right*rCor.second;
        left %= right;
    }
    return rCor;
}

template<typename type>
type modulusInverse(type input, type mod){
    std::pair<type,type> p = extGCD(input,mod);
    if(input*p.first+mod*p.second!=1)
        throw std::logic_error("Return value does not exist.\n");
    if(p.first<0) p.first = p.first%mod+mod;
    return p.first;
}


template<typename ringElement>
inline ringElement powerMod(ringElement base, ll expo, ringElement const& m){
    ringElement out = base.canonicalType.__unit__() % m;
    for(;expo;expo>>=1, base = base*base%m){
        if(expo&1) out = out * base % m;
    }
    return out;
}

template<typename ringElement>
inline ringElement gcd(ringElement const& l, ringElement const& r){
    if(l == l.r->__zero__()) return r;
    return gcd(r%l,l);
}


struct Zp;
struct ZpElement;

struct Zp: public Field<Zp,ZpElement>{
    ll p;
    Zp(ll p);
    ZpElement __add__(ZpElement const& l, ZpElement const& r)const;
    ZpElement __sub__(ZpElement const& l, ZpElement const& r)const;
    ZpElement __mul__(ZpElement const& l, ZpElement const& r)const;
    ZpElement __inv__(ZpElement const& e)const;
    ZpElement __div__(ZpElement const& l, ZpElement const& r)const;
    bool __equal__(ZpElement const& l, ZpElement const& r)const;
    ZpElement __zero__()const;
    ZpElement __unit__()const;
    friend std::ostream& operator<<(std::ostream& out, ZpElement const& x);
};

struct ZpElement: public FieldElement<Zp,ZpElement>{
    ll v;
    ZpElement(Zp const* zp, ll v);
};

Zp::Zp(ll p):p(p){}
ZpElement Zp::__add__(ZpElement const& l, ZpElement const& r)const{
    return ZpElement(this, (l.v+r.v)%p);
}
ZpElement Zp::__sub__(ZpElement const& l, ZpElement const& r)const{
    return ZpElement(this, (l.v-r.v+p)%p);
}
ZpElement Zp::__mul__(ZpElement const& l, ZpElement const& r)const{
    return ZpElement(this, (l.v*r.v)%p);
}
ZpElement Zp::__inv__(ZpElement const& e)const{
    return ZpElement(this, modulusInverse(e.v,p));
}
ZpElement Zp::__div__(ZpElement const& l, ZpElement const& r)const{
    return __mul__(l, r.inv());
}
bool Zp::__equal__(ZpElement const& l, ZpElement const& r)const{
    return (l.v-r.v)%p == 0;
}

ZpElement Zp::__zero__()const{
    return ZpElement(this,0);
}
ZpElement Zp::__unit__()const{
    return ZpElement(this,1);
}

ZpElement::ZpElement(Zp const* zp, ll v):
    FieldElement<Zp,ZpElement>(zp),
    v( ((v % zp->p) + zp->p) % zp->p ){}

std::ostream& operator<<(std::ostream& out, ZpElement const& x){
    return out<<x.v;
}

struct IntType{
    int __zero__()const{ return 0;}
    int __unit__()const{ return 1;}
}intType;

}

#endif //__PRIME_HPP__

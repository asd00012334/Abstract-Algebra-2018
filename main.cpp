#include<bits/stdc++.h>
#include "prime.hpp"

template<typename ring, typename ringElement>
struct Polynomial;

template<typename ring, typename ringElement, typename Enabled=void>
struct PolynomialSpecialized;

template<>
struct PolynomialSpecialized<prime::Zp,prime::ZpElement>;

template<>
struct PolynomialSpecialized<prime::IntType,int>{
    Polynomial<prime::Zp,prime::ZpElement> operator%(prime::Zp const& zp)const;
};

template<>
struct PolynomialSpecialized<prime::Zp,prime::ZpElement>{
    Polynomial<prime::Zp,prime::ZpElement>
    operator%(Polynomial<prime::Zp,prime::ZpElement> p2)const;
};

template<typename ring, typename ringElement>
struct Polynomial: PolynomialSpecialized<ring,ringElement>{
    ring const* r;
    std::vector<ringElement> coef;
    Polynomial():r(NULL){}
    Polynomial(ring const* r, std::vector<ringElement> const& coef): coef(coef){}
    friend std::istream& operator>>(std::istream& in, Polynomial& f){
        std::string str;
        getline(in,str);
        std::stringstream ss(str);
        for(int tmp;ss>>tmp;)
            f.coef.push_back(tmp);
        std::reverse(f.coef.begin(),f.coef.end());
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, Polynomial const& f){
        for(int i=f.coef.size()-1;i>=0;--i){
            if(i<f.coef.size()-1) out<<" + ";
            out<<f.coef[i];
            if(i>1) out<<" x^"<<i; else if(i==1) out<<" x";
        }
        out<<"\n";
        return out;
    }
    ringElement operator()(ringElement const& x)const{
        ringElement out = r->__zero__();
        for(int i=coef.size()-1;i>=0;--i)
            out = out*x + coef[i];
        return out;
    }
    Polynomial operator*(Polynomial const& p2)const{
        Polynomial out;
        out.r = r;
        out.coef.resize(coef.size()+p2.coef.size()-1,r->__zero__());
        for(int i=0;i<coef.size();++i)
        for(int j=0;j<p2.coef.size();++j)
            out.coef[i+j] = out.coef[i+j] + coef[i] * p2.coef[j];
        return out;
    }
};

template<typename ring, typename ringElement, typename Enabled>
struct PolynomialSpecialized{};

Polynomial<prime::Zp,prime::ZpElement>
PolynomialSpecialized<prime::IntType,int>::operator%(prime::Zp const& zp)const{
    Polynomial<prime::Zp,prime::ZpElement> out;
    auto This = static_cast<Polynomial<prime::IntType,int>const*>(this);
    out.r = &zp;
    for(int i=0;i<This->coef.size();++i)
        out.coef.push_back(prime::ZpElement(&zp,This->coef[i]));
    while(out.coef.size()>1&&out.coef.back().v==0) out.coef.pop_back();
    return out;
}



Polynomial<prime::Zp,prime::ZpElement>
PolynomialSpecialized<prime::Zp,prime::ZpElement>::
    operator%(Polynomial<prime::Zp,prime::ZpElement> p2)const{

    Polynomial<prime::Zp,prime::ZpElement> out;
    auto This = static_cast<Polynomial<prime::Zp,prime::ZpElement>const*>(this);
    out.r = This->r;
    out.coef = This->coef;

    /// Reduce to monic polynomial
    for(int i=0;i<p2.coef.size();++i)
        p2.coef[i] = p2.coef[i]/p2.coef.back();

    for(int i=out.coef.size()-1;i>=p2.coef.size()-1;--i){
        for(int j=0;j<p2.coef.size();++j)
            out.coef[i-j] = out.coef[i-j] - p2.coef.rbegin()[j] * This->coef[i];
    }

    while(out.coef.size()>1&&out.coef.back().v==0) out.coef.pop_back();
    return out;

}



double SplitDegree(Polynomial<prime::IntType, int> const& f,int N){
    using namespace std;
    using namespace prime;
    for(int i=0;i<primeListSize;++i){
        int p = primeList[i];
        vector<int> coef = f.coef;
        for(int i=0;i<coef.size();++i)
            coef[i]%=p;
        auto distDeg = [&]()->bool{
        };
    }
}

int main(){
    using namespace std;
    using namespace prime;

    buildPrimeList();
    IntType intType;
    Polynomial<IntType,int> f;
    cin>>f;

    Zp z2(2);
    Polynomial<Zp,ZpElement> p = f%z2;

    return 0;
}

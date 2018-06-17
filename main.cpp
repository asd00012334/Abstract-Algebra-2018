#include<bits/stdc++.h>
#include "prime.hpp"
#include "quotient.hpp"

template<typename, typename> struct Polynomial;
template<typename, typename> struct PolynomialSpecialized;
template<typename> struct DetectDiv;
template<typename ring, typename ringElement, bool Enabled=DetectDiv<ringElement>::value>
struct DivisionSpecialized;

template<typename ring, typename ringElement>
struct Polynomial:
    public PolynomialSpecialized<ring,ringElement>,
    public DivisionSpecialized<ring,ringElement>{
    struct CanonicalType{
        ring const* scalar;
        Polynomial __unit__()const{
            Polynomial out;
            out.canonicalType.scalar = scalar;
            out.coef.push_back(scalar->__unit__());
            return out;
        }
        Polynomial __zero__()const{
            Polynomial out;
            out.canonicalType.scalar = scalar;
            out.coef.push_back(scalar->__zero__());
            return out;
        }
        CanonicalType(ring const* scalar):scalar(scalar){}

    } canonicalType;

    CanonicalType const *r;


    std::vector<ringElement> coef;
    Polynomial():canonicalType(NULL), r(&canonicalType){}
    Polynomial(ring const* scalar, std::vector<ringElement> const& coef):
        coef(coef), canonicalType(scalar), r(&canonicalType){reduce();}
    Polynomial(Polynomial const& p):
        coef(p.coef), r(&canonicalType), canonicalType(p.canonicalType){reduce();}

    Polynomial& operator=(Polynomial const& p2){
        canonicalType = p2.canonicalType;
        r = &canonicalType;
        coef = p2.coef;
        return *this;
    }

    friend std::istream& operator>>(std::istream& in, Polynomial& f){
        std::string str;
        getline(in,str);
        std::stringstream ss(str);
        for(int tmp;ss>>tmp;)
            f.coef.push_back(tmp);
        std::reverse(f.coef.begin(),f.coef.end());
        f.reduce();
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, Polynomial const& f){
        if(f.degree()==0) return out<<f.coef[0];
        for(int i=f.coef.size()-1;i>=0;--i){
            if(i<f.coef.size()-1) out<<" + ";
            out<<f.coef[i];
            if(i>1) out<<" x^"<<i; else if(i==1) out<<" x";
        }
        return out;
    }
    ringElement operator()(ringElement const& x)const{
        ringElement out = canonicalType.scalar->__zero__();
        for(int i=coef.size()-1;i>=0;--i)
            out = out*x + coef[i];
        return out;
    }
    Polynomial operator*(Polynomial const& p2)const{
        Polynomial out;
        out.canonicalType.scalar = canonicalType.scalar;
        out.coef.resize(coef.size()+p2.coef.size()-1,canonicalType.scalar->__zero__());
        for(int i=0;i<coef.size();++i)
        for(int j=0;j<p2.coef.size();++j){
            assert(i+j<out.coef.size());
            out.coef[i+j] = out.coef[i+j] + coef[i] * p2.coef[j];
        }
        out.reduce();
        return out;
    }

    Polynomial operator+(Polynomial const& p2)const{
        Polynomial out;
        out.canonicalType.scalar = canonicalType.scalar;
        int siz = std::max(coef.size(), p2.coef.size());
        out.coef.resize(siz,canonicalType.scalar->__zero__());
        for(int i=0;i<coef.size();++i) out.coef[i] = out.coef[i]+coef[i];
        for(int i=0;i<p2.coef.size();++i) out.coef[i] = out.coef[i]+p2.coef[i];
        out.reduce();
        return out;
    }

    Polynomial operator-(Polynomial const& p2)const{
        Polynomial out;
        out.canonicalType.scalar = canonicalType.scalar;
        int siz = std::max(coef.size(), p2.coef.size());
        out.coef.resize(siz,canonicalType.scalar->__zero__());
        for(int i=0;i<coef.size();++i) out.coef[i] = out.coef[i]+coef[i];
        for(int i=0;i<p2.coef.size();++i) out.coef[i] = out.coef[i]-p2.coef[i];
        out.reduce();
        return out;
    }

    int degree()const{
        for(int out = coef.size()-1;out>0;--out)
            if( !(coef[out]==canonicalType.scalar->__zero__()) )
               return out;
        return 0;
    }

    bool operator==(Polynomial const& e2)const{
        int d1 = degree(), d2 = e2.degree();
        int siz = std::min(d1+1,d2+1);
        for(int i=0;i<siz;++i)
            if(!(coef[i]==e2.coef[i]))
                return false;

        return d1==d2;
    }

    void reduce(){
        while(coef.size()>1&&coef.back()==canonicalType.scalar->__zero__())
            coef.pop_back();
    }

};

template<>
struct PolynomialSpecialized<prime::IntType,int>{
    Polynomial<prime::Zp,prime::ZpElement> operator%(prime::Zp const& zp)const;
};

template<>
struct PolynomialSpecialized<quotient::FracType,quotient::fraction>{
    Polynomial<quotient::FracType,quotient::fraction> diff()const;
    Polynomial<prime::IntType,int> toIntCoef()const;
};

template<typename ring, typename ringElement>
struct PolynomialSpecialized{};

template<typename ring, typename ringElement, bool Enabled>
struct DivisionSpecialized{};

template<typename T>
struct DetectDiv{
    typedef char accept;
    typedef int reject;
    template<typename T2> static accept tester(decltype(&T2::operator/)=nullptr);
    template<typename T2> static reject tester(...);
    static const int value = sizeof(tester<T>(nullptr))==sizeof(accept);
};

template<typename ring, typename ringElement>
struct DivisionSpecialized<ring,ringElement, true>{
    Polynomial<ring,ringElement> operator%(Polynomial<ring,ringElement> p2)const{
        Polynomial<ring,ringElement> out;
        auto This = static_cast<Polynomial<ring,ringElement>const*>(this);
        out.canonicalType.scalar = This->canonicalType.scalar;
        out.coef = This->coef;

        /// Reduce to monic polynomial
        out.reduce();
        for(int i=0;i<p2.coef.size();++i)
            p2.coef[i] = p2.coef[i]/p2.coef.back();

        for(int i=out.coef.size()-1;i>=(int)p2.coef.size()-1;--i){
            auto tmp = out.coef[i];
            for(int j=0;j<p2.coef.size();++j)
                out.coef[i-j] = out.coef[i-j] - p2.coef.rbegin()[j] * tmp;
        }
        out.reduce();
        return out;
    }

    Polynomial<ring,ringElement> operator/(Polynomial<ring,ringElement> p2)const{
        /// ?
        Polynomial<ring, ringElement> out, res;
        auto This = static_cast<Polynomial<ring,ringElement>const*>(this);
        res.canonicalType.scalar = This->canonicalType.scalar;
        out.canonicalType.scalar = This->canonicalType.scalar;

        res.coef = This->coef;
        out.coef.resize(res.coef.size(),This->r->scalar->__zero__());
        /// Reduce to monic polynomial

        auto cinv = p2.coef.back().inv();
        for(int i=0;i<p2.coef.size();++i)
            p2.coef[i] = p2.coef[i]*cinv;

        for(int i=res.coef.size()-1;i>=(int)p2.coef.size()-1;--i){
            auto tmp = res.coef[i];
            out.coef[i-p2.coef.size()+1] = tmp;
            for(int j=0;j<p2.coef.size();++j)
                res.coef[i-j] = res.coef[i-j] - p2.coef.rbegin()[j] * tmp;
        }
        for(int i=0;i<out.coef.size();++i)
            out.coef[i] = out.coef[i] * cinv;

        out.reduce();
        return out;
    }
};


Polynomial<prime::Zp,prime::ZpElement>
PolynomialSpecialized<prime::IntType,int>::operator%(prime::Zp const& zp)const{
    Polynomial<prime::Zp,prime::ZpElement> out;
    auto This = static_cast<Polynomial<prime::IntType,int>const*>(this);
    out.canonicalType.scalar = &zp;

    for(int i=0;i<This->coef.size();++i)
        out.coef.push_back(prime::ZpElement(&zp,This->coef[i]));
    out.reduce();
    return out;
}


Polynomial<quotient::FracType,quotient::fraction>
PolynomialSpecialized<quotient::FracType,quotient::fraction>::diff()const{
    Polynomial<quotient::FracType,quotient::fraction> out;
    auto This = static_cast<Polynomial<quotient::FracType,quotient::fraction>const*>(this);
    out.canonicalType.scalar = This->canonicalType.scalar;
    out.coef.resize(This->coef.size()-1,0);
    for(int i=This->coef.size()-1;i>0;--i)
        out.coef[i-1] = This->coef[i]*i;
    out.reduce();
    return out;
}

Polynomial<prime::IntType,int>
PolynomialSpecialized<quotient::FracType,quotient::fraction>::toIntCoef()const{
    auto This = static_cast<Polynomial<quotient::FracType,quotient::fraction>const*>(this);
    Polynomial<prime::IntType,int> out;
    out.canonicalType.scalar = &prime::intType;
    out.coef.resize(This->coef.size(),0 );

    auto lcm = [](long long l, long long r)->long long{
        return l*r/std::__gcd(l,r);
    };
    long long multi=1;
    for(int i=0;i<This->coef.size();++i)
        multi = lcm(multi,This->coef[i].dn);
    for(int i=0;i<out.coef.size();++i){
        out.coef[i] = This->coef[i].en*multi/This->coef[i].dn;
    }
    out.reduce();
    return out;
}

struct DegreeList: public std::vector<int>{
    friend std::ostream& operator<<(std::ostream& out, DegreeList const& x){
        out<<"[ ";
        for(int i=0;i<x.size();++i){
            if(i&&i<x.size()) out<<", ";
            out<<x[i];
        }
        out<<" ]";
        return out;
    }

    void reduce(){
        while(size()&&back()==0) pop_back();
    }
    int degree()const{
        int i;
        for(i=size()-1;i>0;--i)
            if(back()!=0) break;
        return i;
    }
    bool operator<(DegreeList r)const{
        int ld = degree(), rd = r.degree();
        int num = std::min(ld,rd);
        for(int i=0;i<num;++i){
            if(at(i)==r[i]) continue;
            return at(i)<r[i];
        }
        return ld<rd;
    }
    DegreeList(): std::vector<int>(){}
    DegreeList(std::vector<int>const& r): std::vector<int>(r){}
};

std::map<DegreeList,int> SplitDegree(Polynomial<prime::IntType, int> const& f,int N){
    using namespace std;
    using namespace prime;
    std::map<DegreeList, int> out;
    IntType intType;
    Polynomial<prime::IntType, int> x(&intType,vector<int>{0,1});
    for(int i=0;i<N;++i){
        int p = primeList[i];
        Zp zp(p);
        Polynomial<Zp,ZpElement> g = f % zp, xp = x % zp;
        Polynomial<Zp,ZpElement> nonDeterminant = x % zp;
        DegreeList a;
        int fdegree = f.degree(), totalDegree=0;
        for(int deg=1;totalDegree<fdegree;++deg){
            /// Distinct degree factorization
            /// x^p^deg mod g
            nonDeterminant = powerMod(nonDeterminant,p,g);
            a.push_back(0);
            while(totalDegree<fdegree){
                auto factor = gcd(nonDeterminant-xp,g);
                //std::cout<<"gcd( "<<nonDeterminant-xp<<", "<<g<<" ) = "<<factor<<"\n";
                g = g / factor;
                int partDegree = factor.degree();
                //std::cout<<partDegree<<"partDegree\n";
                if(partDegree == 0) break;
                totalDegree += partDegree;
                a.back()+=partDegree/deg;
            }
        }
        if(out.count(a)) out[a]+=1; else out[a] = 0;
    }
    int id = out[std::vector<int>{f.degree()}];
    for(auto &e: out) e.second=round((double)e.second/id);
    return out;
}

void testMod(){
    using namespace std;
    using namespace prime;
    Polynomial<prime::IntType, int> x(
        &intType,
        vector<int>{2,0,0,0,0,1}
    );
    Polynomial<prime::IntType, int> y(
        &intType,
        vector<int>{0,0,0,1}
    );
    Zp zp(7);
    auto xp = x % zp, yp = y % zp;
    auto z = gcd(xp,yp);
    std::cout<<"("<<xp<<") / ("<<yp<<") = ("<<z<<")\n";


}

void testFraction(){
    using namespace std;
    using namespace quotient;
    Polynomial<FracType,fraction> pfrac1, pfrac2;
    pfrac1.canonicalType.scalar = &fracType;
    pfrac2.canonicalType.scalar = &fracType;
    cin>>pfrac1;
    cout<<prime::gcd(pfrac1.diff(),pfrac1)<<"\n";

}

struct SubS5{
    std::string name;
    int order;
    std::map<DegreeList,int> cycle;
};

std::vector<SubS5> loader(){
    using namespace std;
    fstream fin("CycleType.import",ios::in);
    vector<SubS5> out;
    while(1){
        SubS5 tmp;
        string op; fin>>op;
        if(op==".") break;
        do{
            if(op=="."){
                out.push_back(tmp);
                break;
            }
            if(op=="name") fin>>tmp.name;
            else if(op=="order") fin>>tmp.order;
            else if(op=="cycle"){
                int accum = 0;
                while(accum<tmp.order){
                    int cnt; fin>>cnt;
                    accum += cnt;
                    DegreeList dlist;
                    for(int deg=0, i=1;deg<5;++i){
                        int val;
                        fin>>val;
                        deg+=val*i;
                        dlist.push_back(val);
                    }
                    tmp.cycle[dlist] = cnt;
                }
            }
            op.clear(); fin>>op;
        }while(1);
    }
    fin.close();
    return out;
}

int main(){
    using namespace std;
    using namespace prime;

    buildPrimeList();
    Polynomial<IntType,int> fint;
    Polynomial<quotient::FracType,quotient::fraction> fq;
    fint.canonicalType.scalar = &intType;
    fq.canonicalType.scalar = &quotient::fracType;

    cin>>fq;
    fq = fq / gcd(fq.diff(),fq);
    fint = fq.toIntCoef();

    std::cout << fint <<"\n";

    auto table = SplitDegree(fint,20000);
    for(auto e: table)
        cout<<e.first<<": "<<e.second<<"\n";

    int order = 0;
    for(auto &e: table) order += e.second;
    std::cout<<"Order: "<<order<<"\n";
    auto subS5List = loader();
    queue<int> orderMatch;
    int best = INT_MAX;
    for(int i=0;i<subS5List.size();++i){
        int dist = abs(order-subS5List[i].order);
        if(dist<best){
            while(orderMatch.size())
                orderMatch.pop();
            orderMatch.push(i);
            best = dist;
        } else if(dist==best){
            orderMatch.push(i);
        }
    }
    if(orderMatch.size()==1){
        std::cout<<subS5List[orderMatch.front()].name<<"\n";
    } else if(orderMatch.size()>1){
        auto mapDist = [](
            map<DegreeList,int> l,
            map<DegreeList,int>const& r)->int{

            for(auto &e: r){
                if(l.count(e.first))
                    l[e.first]-=e.second;
                else l[e.first] = e.second;
            }
            int out = 0;
            for(auto &e: l) out+=abs(e.second);
            return out;
        };
        int best = INT_MAX;
        int bestIdx=-1;
        for(;orderMatch.size();orderMatch.pop()){
            int idx = orderMatch.front();
            int dist = mapDist(table,subS5List[idx].cycle);
            if(dist<best) best = dist, bestIdx = idx;
        }
        std::cout<<subS5List[bestIdx].name<<"\n";
    } else puts("Unknown");

    //testMod();
    //testFraction();


    return 0;
}

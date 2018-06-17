#ifndef __ALGEBRA_HPP__
#define __ALGEBRA_HPP__

#include<bits/stdc++.h>

template<typename ring, typename ringElement>
struct Ring{
    ringElement __zero__()const{
        return static_cast<ring const*>(this)->__zero__();
    }
    ringElement __add__(ringElement const& l, ringElement const& r)const{
        return static_cast<ring const*>(this)->__add__(l, r);
    }
    ringElement __sub__(ringElement const& l, ringElement const& r)const{
        return static_cast<ring const*>(this)->__sub__(l, r);
    }
    ringElement __mul__(ringElement const& l, ringElement const& r)const{
        return static_cast<ring const*>(this)->__mul__(l, r);
    }
    bool __equal__(ringElement const& l, ringElement const& r)const{
        return static_cast<ring const*>(this)->__equal__(l, r);
    }
};

template<typename field, typename fieldElement>
struct Field: public Ring<field,fieldElement>{
    fieldElement __inv__(fieldElement const& e)const{
        return static_cast<field const*>(this)->__inv__(e);
    }
    fieldElement __unit__(fieldElement const& e)const{
        return static_cast<field const*>(this)->__unit__(e);
    };
    fieldElement __div__(fieldElement const& l, fieldElement const& r)const{
        return static_cast<field const*>(this)->__div__(l, r);
    };
};

template<typename ring, typename ringElement>
struct RingElement{
    ring const* r;       /// the ring it belongs to
    RingElement(ring const* r): r(r){}
    ringElement operator+(ringElement const& e2)const{
        if(r != e2.r) throw std::exception();
        return r->__add__((ringElement const&)*this, e2);
    }
    ringElement operator-(ringElement const& e2)const{
        if(r != e2.r) throw std::exception();
        return r->__sub__((ringElement const&)*this, e2);
    }
    ringElement operator*(ringElement const& e2)const{
        if(r != e2.r) throw std::exception();
        return r->__mul__((ringElement const&)*this, e2);
    }
    bool operator==(ringElement const& e2)const{
        if(r != e2.r) throw std::exception();
        return r->__equal__((ringElement const&)*this, e2);
    }
};


template<typename field, typename fieldElement>
struct FieldElement: public RingElement<field,fieldElement>{
    field const* f;
    FieldElement(field const* f): f(f),RingElement<field,fieldElement>(f){}
    fieldElement operator/(fieldElement const& e2)const{
        if(f != e2.f) throw std::exception();
        return f->__div__((fieldElement const&)*this, e2);
    }
    fieldElement inv()const{
        return f->__inv__((fieldElement const&)*this);
    }
};


#endif // __ALGEBRA_HPP__

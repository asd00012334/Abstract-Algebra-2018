# Abstract-Algebra-2018
The final project of course Abstract Algebra in 2018.

## Component

### Type Heirachy
Type syetem is divided into interfaces and instaces, for the former:
- `struct Ring`
- `struct Field`
- `struct RingElement`
- `struct FieldElement`
where `Field`, `FieldElement` inherit from `Ring`, `RingElement`

Derived class is mainly instances of algebraic structure, there are:
- `Zp`, `ZpElement` represent modular p
- `IntType` represent integer
- `FracType`, `fraction` represent rational numbers
### Polynomial
Especially, `struct Polynomial` is defined as an template, with type arguement `ring`, `ringElement` representing coefficient type. The polynomials are specialized differently with different coefficient type.
- Field: specialized with `operator/`, `operator%`
- Integer: modulo p
- Fraction: differential

## Usage
- First, use `cin>>f` to accept input as coefficient
```c++
Polynomial<IntType,int> f;
f.canonicalType.scalar = &intType;
std::cin>>f;
```
- After differentiation, use `SplitDegree(f, iteration)` with specified iteration number, it will return a `std::map<DegreeList,int>` type lookup table that represents number of different cycle structure
    - For instance [2, 0, 1] -> 3 represents Galois group with 3 $(1 2 3)(4)(5)$ cycles.

- Next, the `loader()` load information of S5 subgroup from CycleType.import
- Final, define the information distance between group statistics, compare order and cycle structure and decide the closest one as the final choice.

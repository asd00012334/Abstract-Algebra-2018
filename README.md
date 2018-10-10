# Abstract-Algebra-2018
The final project of course Abstract Algebra in 2018.

Check [this note](https://hackmd.io/s/ryvpRjYeX) for more detail (in Mandarin).

Goal: to find Galois group of the splitting field of a quintic polynomial over Q.

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
### Build and Test
- Build
```shell
make all
```
- or Compile Directly
```shell
g++ -std=c++11 main.cpp -o main
```
- Run test
```shell
make test
```

### Source
- First, use `cin>>f` to accept input as coefficient
```c++
Polynomial<IntType,int> f;
f.canonicalType.scalar = &intType;
std::cin>>f;
```
- After differentiation, use `SplitDegree(f, iteration)` with specified iteration number, it will return a `std::map<DegreeList,int>` type lookup table that represents number of different cycle structure
    - For instance [2, 0, 1] -> 3 represents Galois group with 3 elements of (1 2 3)(4)(5) cycles.

- Next, the `loader()` load information of S5 subgroup from CycleType.import
- Finally, define the information distance between group statistics, compare order and cycle structure and decide the closest one as the final choice.

### Result and Conclusion
Below shows result of x^5+2x+6, for more testing result please refer to [test.out](https://github.com/asd00012334/Abstract-Algebra-2018/blob/master/test.out)
```
> 1 0 0 0 2 6, exp: S5
1 x^5 + 0 x^4 + 0 x^3 + 0 x^2 + 2 x + 6
[ 5 ]: 1
[ 0, 0, 0, 0, 1 ]: 23
[ 0, 1, 1 ]: 20
[ 1, 2 ]: 15
[ 1, 0, 0, 1 ]: 29
[ 2, 0, 1 ]: 19
[ 3, 1 ]: 10
Order: 117
S5
```
There is slight error in the result, after classification the result is as expected (S5). We use 20000 as iteration number of p. Time consumption ranges from ten to tens of seconds.

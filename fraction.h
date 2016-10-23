#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <cassert>
using namespace std;
typedef int fvtype;
class FractionException{};
class InvalidFractionException:FractionException{};
class Fraction
{
    fvtype a,b;
public:
    Fraction();
    Fraction(int x);
    Fraction(int a,int b);
    Fraction(string str);
    void simplified();
    bool zero();
    string toHTML();
    Fraction reciprocal()const;
    Fraction operator +=(const Fraction &ff);
    Fraction operator -=(const Fraction &ff);
    Fraction operator *=(const Fraction &ff);
    Fraction operator /=(const Fraction &ff);
    bool operator() ();
    friend bool isFraction(string str);
    friend Fraction abs(const Fraction &ff);
    friend Fraction simplified(Fraction ff);
    friend Fraction operator +(const Fraction& fa,const Fraction& fb);
    friend Fraction operator -(const Fraction& fa,const Fraction& fb);
    friend Fraction operator *(const Fraction& fa,const Fraction& fb);
    friend Fraction operator /(const Fraction& fa,const Fraction& fb);
    friend ostream& operator <<(ostream& out,const Fraction &mm);
    friend istream& operator >>(istream &in,Fraction &mm);
};
bool isFraction(string str);

#endif // FRACTION_H

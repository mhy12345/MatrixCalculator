#include "fraction.h"

fvtype gcd(fvtype x,fvtype y)
{
    return (x%y==0)?y:gcd(y,x%y);
}

Fraction::Fraction()
{
    a=0;
    b=1;
}
Fraction::Fraction(int x)
{
    a=x;b=1;
}
Fraction::Fraction(int a,int b):a(a),b(b)
{
}
Fraction::Fraction(string str)
{
    if (str.find("/")!=-1)
    {
        sscanf(str.c_str(),"%d/%d",&a,&b);
    }else if (str.find(".")!=-1)
    {
        a=0;
        b=1;
        bool flag=false;
        for (int i=0;i<str.size();i++)
        {
            if (str[i]=='.')
            {
                flag=true;
            }else
            {
                a=a*10+str[i]-'0';
                if (flag)b=b*10;
            }
        }
    }else
    {
        sscanf(str.c_str(),"%d",&a);
        b=1;
    }
}
bool isFraction(string str)
{
    int cd,cs;
    cd=cs=0;
    for (int i=0;i<str.length();i++)
    {
        if (str[i]=='.')
            cd++;
        else if (str[i]=='/')
            cs++;
        else if (str[i]<'0' || str[i]>'9')
            return false;
    }
    if (cd+cs<=1)
        return true;
    return false;
}

void Fraction::simplified()
{
    int t=gcd(a,b);
    a/=t;b/=t;
}
Fraction simplified(Fraction ff)
{
    ff.simplified();
    return ff;
}

Fraction abs(const Fraction &ff)
{
    return Fraction(abs(ff.a),abs(ff.b));
}

Fraction Fraction::operator +=(const Fraction &ff)
{
    (*this)=(*this)+ff;
}
Fraction Fraction::operator -=(const Fraction &ff)
{
    (*this)=(*this)-ff;
}
Fraction Fraction::operator *=(const Fraction &ff)
{
    (*this)=(*this)*ff;
}
Fraction Fraction::operator /=(const Fraction &ff)
{
    (*this)=(*this)/ff;
}
bool Fraction::operator ()()
{
    return a;
}
bool Fraction::zero()
{
    return !a;
}
string Fraction::toHTML()
{
    assert(false);
}

Fraction Fraction::reciprocal()const
{
    return Fraction(b,a);
}

Fraction operator+(const Fraction &fa,const Fraction &fb)
{
    return simplified(Fraction(fa.a*fb.b + fb.a*fa.b,fa.b*fb.b));
}
Fraction operator-(const Fraction &fa,const Fraction &fb)
{
    return simplified(Fraction(fa.a*fb.b - fb.a*fa.b,fa.b*fb.b));
}
Fraction operator*(const Fraction &fa,const Fraction &fb)
{
    return simplified(Fraction(fa.a*fb.a,fa.b*fb.b));
}
Fraction operator/(const Fraction &fa,const Fraction &fb)
{
    return simplified(fa*fb.reciprocal());
}

ostream& operator <<(ostream& out,const Fraction &mm)
{
    if (mm.b!=1)
        out<<mm.a<<"/"<<mm.b;
    else
        out<<mm.a;
    return out;
}
istream& operator >>(istream &in,Fraction &mm)
{
    string str;
    in>>str;
    mm=Fraction(str);
    return in;
}

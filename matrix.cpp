#include "Matrix.h"
using namespace std;
const double Matrix::eps = 1e-7;

Matrix::Matrix()
{
    n=m=0;
}
Matrix::Matrix(int n,int m):n(n),m(m)
{
}
void Matrix::appendInput(string line)
{
    stringstream ss;
    ss<<line;
    Fraction x;
    int t=0;
    while (ss>>x)
    {
        a[n][t++]=x;
    }
    if (m==0)m=t;
    n++;
}

void Matrix::makeUnit()
{
    assert(n==m);
    for (int i=0;i<n;i++)
        a[i][i]=1;
}

string Matrix::getAbstract(int len)
{
    stringstream ss;
    string res,t;
    ss<<"(matrix<"<<n<<","<<m<<">)";
    ss>>res;
    ss.clear();
    for (int i=0;i<n;i++)
    {
        res+="{";
        for (int j=0;j<m;j++)
        {
            ss<<a[i][j];ss>>t;ss.clear();
            res+=t;
            if (j!=m-1)res+=",";
            if (j!=m-1 && res.size()+8>len)
            {
                res+="...";
                break;
            }
        }
        res+="}";
        if (i!=n-1)res+=",";
        if (i!=n-1 && res.size()+8>len)
        {
            res+="...";
            break;
        }
    }
    res+="}";
    return res;
}

string Matrix::toHTML()
{
    string res,t;
    stringstream ss;
    res+="<font style='font-size:30px'>";
    res+="<table style='margin:auto;table-layout:fixed' cellpadding='10'>";
    for (int i=0;i<n;i++)
    {
        res+="<tr>";
        for (int j=0;j<m;j++)
        {
            res+="<td>";
            res+="<span style='margin:auto'>";
            ss<<a[i][j];ss>>t;ss.clear();res+=t;
            res+="</span>";
            res+="</td>";
        }
        res+="</tr>";
    }
    res+="</table>";
    res+="</font>";
    return res;
}

Matrix operator *(Matrix m1,Matrix m2)
{
    assert(m1.m==m2.n);
    Matrix res(m1.n,m2.m);
    for (int i=0;i<m1.n;i++)
        for (int j=0;j<m2.m;j++)
            for (int k=0;k<m1.m;k++)
                res.a[i][j]+=m1.a[i][k]*m2.a[k][j];
    return res;
}

Matrix operator *(Fraction v,const Matrix& mm)
{
    Matrix res=mm;
    for (int i=0;i<res.n;i++)
        for (int j=0;j<res.m;j++)
            res.a[i][j]*=v;
    return res;
}
Matrix operator +(const Matrix& m1,const Matrix& m2)
{
    Matrix res;
    assert(m1.n==m2.n && m1.m==m2.m);
    res.n=m1.n;
    res.m=m1.m;
    for (int i=0;i<res.n;i++)
        for (int j=0;j<res.m;j++)
            res.a[i][j]=m1.a[i][j]+m2.a[i][j];
    return res;
}

Matrix operator *(const Matrix& mm,Fraction v)
{
    Matrix res=mm;
    for (int i=0;i<res.n;i++)
        for (int j=0;j<res.m;j++)
            res.a[i][j]*=v;
    return res;
}
ostream& operator <<(ostream& out,const Matrix &mm)
{
    for (int i=0;i<mm.n;i++)
    {
        for (int j=0;j<mm.m;j++)
            out<<mm.a[i][j]<<"\t";
        out<<endl;
    }
    out<<endl;
    return out;
}
istream& operator >>(istream &in,Matrix &mm)
{
    int last=-1;
    string str;
    mm=Matrix();
    mm.m=-1;
    while(getline(in,str))
    {
        if (str[0]=='\n' || str[0]=='\0')break;
        stringstream ss;
        ss<<str;
        last=mm.m;
        mm.m=0;
        while (ss>>mm.a[mm.n][mm.m++]);
        mm.m--;
        mm.n++;
        assert(last==-1 || last==mm.m);
    }
    return in;
}
Matrix Matrix::transport()
{
    Matrix mm=*this;
    swap(mm.n,mm.m);
    for (int i=0;i<mm.n;i++)
        for (int j=0;j<mm.m;j++)
            mm.a[j][i]=a[i][j];
    return mm;
}
Matrix Matrix::inverse()
{

    Matrix mm=(*this);
        Matrix mt;
        int n=mm.n;
        mt.m=mt.n=n;
        mt.makeUnit();
        for (int i=0;i<n;i++)
        {
                int x=-1;
                for (int j=i;j<n;j++)
                        if (!mm.a[j][i].zero())
                                {x=j;break;}
                if (x==-1)
                    throw UnableEliminateException();
                assert(~x);
                for (int j=0;j<n;j++)
                        swap(mm.a[x][j],mm.a[i][j]),
                                swap(mt.a[x][j],mt.a[i][j]);
                for (int j=i+1;j<n;j++)
                {
                        Fraction v = mm.a[j][i]/mm.a[i][i];
                        for (int k=0;k<n;k++)
                        {
                                mm.a[j][k]-=mm.a[i][k]*v;
                                mt.a[j][k]-=mt.a[i][k]*v;
                        }
                }
        }
        for (int i=n-2;i>=0;i--)
        {
                for (int j=n-1;j>i;j--)
                {
                        Fraction v = mm.a[i][j] / mm.a[j][j];
                        for (int k=0;k<n;k++)
                        {
                                mm.a[i][k]-=v*mm.a[j][k];
                                mt.a[i][k]-=v*mt.a[j][k];
                        }
                }
        }
        for (int i=0;i<n;i++)
        {
                Fraction v=mm.a[i][i];
                for (int j=0;j<n;j++)
                {
                        mm.a[i][j]/=v;
                        mt.a[i][j]/=v;
                }
        }
        return mt;
}

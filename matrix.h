#ifndef MATRIX_H
#define MATRIX_H
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cassert>
#include<string>
#include<sstream>
#include<cmath>
#include<map>
#include "fraction.h"
#define MAXN 30
using namespace std;
class MatrixCalculationExceptions{};
class InvalidMatrixException:MatrixCalculationExceptions{};
class UnableEliminateException:MatrixCalculationExceptions{};
class Matrix
{
    static const double eps;
private:
    Fraction a[MAXN][MAXN];
    int n,m;
public:
    Matrix();
    Matrix(int n,int m);
    void makeUnit();
    void appendInput(string line);
    string getAbstract(int len=30);
    string toHTML();
    Matrix inverse();
    Matrix transport();
    friend Matrix operator +(const Matrix& m1,const Matrix& m2);
    friend Matrix operator *(Matrix m1,Matrix m2);
    friend Matrix operator *(Fraction v,const Matrix &mm);
    friend Matrix operator *(const Matrix &mm,Fraction v);
    friend ostream& operator <<(ostream& out,const Matrix &mm);
    friend istream& operator >>(istream &in,Matrix &mm);
};

#endif

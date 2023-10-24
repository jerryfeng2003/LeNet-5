#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <math.h>

using namespace std;

template <typename T>
class Mat
{
public:
    int col, row;
    vector<T> para;

    Mat(int n, int m) : col(n), row(m)
    {
        para.resize(n * m);
        fill(para.begin(), para.end(), 0);
    }

    Mat():col(1),row(1)
    {
        para[0]=0;
    }

    Mat(int n, int m, bool guass) : col(n), row(m)
    {
        para.resize(n * m);
        if (guass == true)
        {
            for (auto i = para.begin(); i != para.end(); i++)
                *i = distr(gen);
        }
        else
            fill(para.begin(), para.end(), 0);
    }

    Mat(const Mat &a) : col(a.col), row(a.row)
    {
        para.resize(a.col * a.row);
        for (auto i = para.begin(), j = a.para.begin(); i != para.end(); i++, j++)
            *i = *j;
    }

    void print_size()
    {
        cout << col << " " << row;
    }

    void print_mat();

    Mat add(const Mat &a);
    Mat add(double a);
    Mat minus(const Mat &a);
    Mat minus(double a);
    Mat dot(const Mat &a);
    Mat n_dot(const Mat &a);
    Mat n_dot(double a);
};

template <typename T>
void Mat<T>::print_mat()
{
    auto temp = para.begin();
    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            cout << *temp << "\t";
            temp++;
        }
        cout << endl;
    }
}

template<typename T>
Mat<T> Mat<T>::add(const Mat &a)
{
    if(col!=a.col||row!=a.row)
    {
        cout<<"Wrong size!"<<endl;
        exit(-1);
    }
    Mat temp(col,row);
    for(auto i=temp.para.begin(),j=a.para.begin(),k=para.begin();i!=temp.para.end();i++,j++,k++)
    {
        *i=*j+*k;
    }
    return temp;
}

template<typename T>
Mat<T> Mat<T>::add(double a)
{
    Mat temp(col,row);
    for(auto i=temp.para.begin(),k=para.begin();i!=temp.para.end();i++,k++)
    {
        *i=a+*k;
    }
    return temp;
}

template<typename T>
Mat<T> Mat<T>::minus(const Mat &a)
{
    if(col!=a.col||row!=a.row)
    {
        cout<<"Wrong size!"<<endl;
        exit(-1);
    }
    Mat temp(col,row);
    for(auto i=temp.para.begin(),j=a.para.begin(),k=para.begin();i!=temp.para.end();i++,j++,k++)
    {
        *i=-*j+*k;
    }
    return temp;
}

template<typename T>
Mat<T> Mat<T>::minus(double a)
{
    Mat temp(col,row);
    for(auto i=temp.para.begin(),k=para.begin();i!=temp.para.end();i++,k++)
    {
        *i=*k-a;
    }
    return temp;
}

template<typename T>
Mat<T> Mat<T>::n_dot(const Mat &a)
{
    if(col!=a.col||row!=a.row)
    {
        cout<<"Wrong size!"<<endl;
        exit(-1);
    }
    Mat temp(col,row);
    for(auto i=temp.para.begin(),j=a.para.begin(),k=para.begin();i!=temp.para.end();i++,j++,k++)
    {
        *i=(*j)*(*k);
    }
    return temp;
}

template<typename T>
Mat<T> Mat<T>::n_dot(double a)
{
    Mat temp(col,row);
    for(auto i=temp.para.begin(),k=para.begin();i!=temp.para.end();i++,k++)
    {
        *i=a*(*k);
    }
    return temp;
}

template<typename T>
Mat<T> Mat<T>::dot(const Mat &a)
{
    if(row!=a.col)
    {
        cout<<"Wrong size!"<<endl;
        exit(-1);
    }
    Mat temp(col,row);
    auto i=para.begin(),j=a.para.begin(),k=temp.para.begin();
    for(int h=0;h<col)
}

int main()
{
    Mat<float> a(2, 2, true);
    a.print_size();
    cout << endl;
    a.print_mat();
    Mat<float> b=a;
    cout<<endl;
    b.print_mat();
    cout<<endl;
    b=a.n_dot(b);
    b.print_mat();
    cout<<endl;
}
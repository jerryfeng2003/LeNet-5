//
// Created by Hyper on 23/10/2023.
// Modify by Jerry on 25/10/2023.

#ifndef LENET_5_MATRIX_H
#define LENET_5_MATRIX_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <random>

using namespace std;
default_random_engine gen(114514 * time(0));
normal_distribution<double> distr(0.0, 1.0);

typedef vector<vector<float>> Mat;
using namespace std;

class Matrix
{
public:
    Mat mat;
    size_t row, col;
    // 构造函数
    Matrix() : row(1), col(1)
    {
        mat[0][0] = 0;
    }
    Matrix(size_t row, size_t col) : row(row), col(col)
    {
        if (row <= 0 || col <= 0)
            cout << "row or col is illegal" << endl, exit(-1);
        mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            mat[i].resize(col); // 设置列数
        }
    }
    Matrix(size_t row, size_t col, bool Guass) : row(row), col(col)
    {
        if (row <= 0 || col <= 0)
            cout << "row or col is illegal" << endl, exit(-1);
        mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            mat[i].resize(col); // 设置列数
        }
        if (Guass)
            Gaussian_init();
    }

    Matrix(const Matrix &a)
    {
        mat.resize(a.row);
        for (int i = 0; i < mat.size(); ++i)
        {
            mat[i].resize(a.row); // 设置列数
            for (int j = 0; j < a.col; j++)
            {
                mat[i][j] = a.mat[i][j];
            }
        }
    }

    void resize(size_t n, size_t m)
    {
        if (n * m != row * col)
            cout << "row and col is illegal" << endl, exit(-1);
        vector<float> temp(n * m);
        auto i_temp = temp.begin();
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                *i_temp = mat[i][j];
                i_temp++;
            }
        }
        row = n, col = m;
        i_temp = temp.begin();
        mat.resize(row);
        for (int i = 0; i < row; ++i)
        {
            mat[i].resize(col);
            for (int j = 0; j < col; ++j)
            {
                mat[i][j] = *i_temp;
                i_temp++;
            }
        }
    }

    void Flatten()
    {
        resize(1, row * col);
    }

    // Initial the value through Gaussian distribution
    void Gaussian_init()
    {
        for (auto i = mat.begin(); i != mat.end(); i++)
        {
            for (auto j = i->begin(); j != i->end(); j++)
            {
                *j = distr(gen);
            }
        }
    }
    // 矩阵转置
    Matrix MatrixTranspose()
    {
        Matrix R(row, col);
        for (int i = 0; i < col; ++i)
        {
            for (int j = 0; j < row; ++j)
            {
                R.mat[i][j] = mat[j][i];
            }
        }
        return R;
    }
    // 矩阵旋转180度
    Matrix MatrixRotate180()
    {
        Matrix R(row, col);
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                R.mat[i][j] = mat[row - i - 1][col - j - 1];
            }
        }
        return R;
    }
    // 求和
    double MatrixSum()
    {
        double sum = 0;
        for (int i = 0; i < mat.size(); i++)
        {
            for (int j = 0; j < mat[0].size(); j++)
            {
                sum += mat[i][j];
            }
        }
        return sum;
    }
    // 打印矩阵
    virtual void MatrixPrint()
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }
    static Matrix conv2d(const Mat &A, const Mat &kernel);
    Matrix MatrixAdd(const Matrix &B);
    Matrix MatrixAdd(double val);
    Matrix MatrixMinus(const Matrix &B);
    Matrix MatrixMinus(double val)
    {
        return MatrixAdd(-val);
    }
    Matrix MatrixMul(const Matrix &B);
    Matrix MatrixMul(double val);
    Matrix MatrixDotMul(const Matrix &B);
    Matrix MatrixPool(int pool_size, int stride);

    Matrix operator+(const Matrix &B)
    {
        return MatrixAdd(B);
    }
    Matrix operator+(double val)
    {
        return MatrixAdd(val);
    }
    Matrix operator-(const Matrix &B)
    {
        return MatrixMinus(B);
    }
    Matrix operator-(double val)
    {
        return MatrixMinus(val);
    }
    Matrix operator*(const Matrix &B)
    {
        return MatrixMul(B);
    }
    Matrix operator*(double val)
    {
        return MatrixMul(val);
    }
    Matrix operator^(const Matrix &B)
    {
        return MatrixDotMul(B);
    }
};

// 矩阵二维卷积
Matrix Matrix::conv2d(const Mat &A, const Mat &kernel)
{
    if (kernel.size() > A.size() || kernel[0].size() > A[0].size())
        cout << "kernel size is illegal" << endl, exit(-1);
    int row = A.size();
    int col = A[0].size();
    int kernel_row = kernel.size();
    int kernel_col = kernel[0].size();
    int out_row = row - kernel_row + 1; // 输出矩阵的行列数
    int out_col = col - kernel_col + 1; // 输出矩阵的行列数
    Matrix out(out_row, out_col);
    for (int i = 0; i < out_row; i++)
    {
        for (int j = 0; j < out_col; j++)
        {
            for (int m = 0; m < kernel_row; m++)
            {
                for (int n = 0; n < kernel_col; n++)
                {
                    out.mat[i][j] += A[i + m][j + n] * kernel[m][n];
                }
            }
        }
    }
    return out;
}
// 矩阵加法,A加上B,结果存储在add中
Matrix Matrix::MatrixAdd(const Matrix &B)
{
    if (row != B.row || col != B.col)
        cout << "MatrixAdd: Matrix size is not equal" << endl, exit(-1);
    Matrix add(row, col);
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            add.mat[i][j] = mat[i][j] + B.mat[i][j];
        }
    }
    return add;
}
// 广播加法
Matrix Matrix::MatrixAdd(double val)
{
    Matrix A(row, col);
    for (int i = 0; i < A.mat.size(); i++)
    {
        for (int j = 0; j < A.mat[0].size(); j++)
        {
            A.mat[i][j] = mat[i][j] + val;
        }
    }
    return A;
}

// 矩阵减法,A减去B,结果存储在sub中
Matrix Matrix::MatrixMinus(const Matrix &B)
{
    if (row != B.row || col != B.col)
        cout << "MatrixAdd: Matrix size is not equal" << endl, exit(-1);
    Matrix R(row, col);
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            R.mat[i][j] = mat[i][j] - B.mat[i][j];
        }
    }
    return R;
}
// 矩阵间乘法
Matrix Matrix::MatrixMul(const Matrix &B)
{
    if (row != B.row || col != B.col)
        cout << "MatrixAdd: Matrix size is not equal" << endl, exit(-1);
    Matrix mul(row, col);
    int col = B.mat[0].size();
    int mid = mat[0].size();
    mul.mat.resize(row); // 设置行数
    for (int i = 0; i < row; ++i)
    {
        mul.mat[i].resize(col); // 设置列数
        for (int j = 0; j < col; ++j)
        {
            mul.mat[i][j] = 0;
            for (int k = 0; k < mid; ++k)
            {
                mul.mat[i][j] += mat[i][k] * B.mat[k][j];
            }
        }
    }
    return mul;
}
// 矩阵与标量相乘
Matrix Matrix::MatrixMul(double val)
{
    Matrix result(row, col);
    for (int i = 0; i < mat.size(); i++)
    {
        for (int j = 0; j < mat[0].size(); j++)
        {
            result.mat[i][j] = mat[i][j] * val;
        }
    }
    return result;
}
// 矩阵点乘
Matrix Matrix::MatrixDotMul(const Matrix &B)
{
    if (row != B.row || col != B.col)
        cout << "MatrixAdd: Matrix size is not equal" << endl, exit(-1);
    Matrix R(row, col);
    R.mat.resize(row); // 设置行数
    for (int i = 0; i < row; ++i)
    {
        R.mat[i].resize(col); // 设置列数
        for (int j = 0; j < col; ++j)
        {
            R.mat[i][j] = mat[i][j] * B.mat[i][j];
        }
    }
    return R;
}
// 矩阵池化-最大池化
// pool_size:池化核大小
// stride:步长
Matrix Matrix::MatrixPool(int pool_size, int stride)
{
    int out_row = (row - pool_size) / stride + 1; // 输出矩阵的行列数
    int out_col = (col - pool_size) / stride + 1; // 输出矩阵的行列数
    Matrix R(out_row, out_col);
    for (int i = 0; i < out_row; ++i)
    {
        for (int j = 0; j < out_col; ++j)
        {
            R.mat[i][j] = 0;
            for (int m = 0; m < pool_size; ++m)
            {
                for (int n = 0; n < pool_size; ++n)
                {
                    R.mat[i][j] = max(R.mat[i][j], mat[i * stride + m][j * stride + n]);
                }
            }
        }
    }
    return R;
}

//交叉熵损失函数
double Cross_entropy(const Matrix &y, const Matrix &t)
{
    if (y.row != 1 || t.row != 1)
        cout << "Not a vector!" << endl, exit(-1);
    if (y.row != t.row)
        cout << "The two vector dosen't fit!" << endl, exit(-1);
    double loss = 0.;
    size_t m = y.col;
    for (auto i = 0; i < m; ++i)
        loss += -t.mat[0][i] * log(y.mat[0][i]) - (1 - t.mat[0][i]) * log(1 - y.mat[0][i]);
    return loss;
}

float relu(float x)
{
    return x ? 0 : x > 0;
}

Matrix Softmax(const Matrix &x)
{
    if (x.row != 1)
        cout << "Not a vector!" << endl, exit(-1);
    Matrix temp(1,x.col);
    double sum = 0;
    for (int i = 0; i < x.col; i++)
    {
        sum += exp(x.mat[0][i]);
    }
    for (int i = 0; i < x.col; i++)
    {
        temp.mat[0][i] = exp(x.mat[0][i]) / sum;
    }
    return temp;
}

Matrix Relu(const Matrix &x)
{
    if (x.row != 1)
        cout << "Not a vector!" << endl, exit(-1);
    Matrix temp(1,x.col);
    for_each(temp.mat[0].begin(), temp.mat[0].end(), relu);
    return temp;
}

//交叉熵损失函数对输出层的求导
Matrix d_Cross_entrophy(const Matrix &y, const Matrix &t)
{
    if (y.row != 1 || t.row != 1)
        cout << "Not a vector!" << endl, exit(-1);
    if (y.row != t.row)
        cout << "The two vector dosen't fit!" << endl, exit(-1);
    Matrix temp(1,y.col);
    for (int i = 0; i < y.col; i++)
    {
        temp.mat[0][i] = -t.mat[0][i] / y.mat[0][i] + (1 - t.mat[0][i]) / (1 - y.mat[0][i]);
    }
    return temp;
}

#endif // LENET_5_MATRIX_H

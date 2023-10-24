//
// Created by Hyper on 23/10/2023.
//

#ifndef LENET_5_MATRIX_H
#define LENET_5_MATRIX_H
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

// 矩阵的类实现
/*
 *函数：
 */

using namespace std;
default_random_engine gen(114514 * time(0));
normal_distribution<double> distr(0.0, 1.0);

typedef vector<vector<float>> Mat;
using namespace std;
class Matrix
{
private:
    Mat mat;

public:
    // 构造函数
    Matrix(size_t row, size_t col)
    {
        if (row <= 0 || col <= 0)
            cout << "row or col is illegal" << endl;
        mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            mat[i].resize(col); // 设置列数
        }
    }
    // 赋值构造函数
    Matrix(size_t row, size_t col, double val)
    {
        if (row <= 0 || col <= 0)
            cout << "row or col is illegal" << endl;
        mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            mat[i].resize(col); // 设置列数
            for (int j = 0; j < col; ++j)
            {
                mat[i][j] = val;
            }
        }
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
    // 矩阵二维卷积
    static Matrix conv2d(const Mat &A, const Mat &kernel)
    {
        if (kernel.size() > A.size() || kernel[0].size() > A[0].size())
            cout << "kernel size is illegal" << endl;
        int row = A.size();
        int col = A[0].size();
        int kernel_row = kernel.size();
        int kernel_col = kernel[0].size();
        int out_row = row - kernel_row + 1; // 输出矩阵的行列数
        int out_col = col - kernel_col + 1; // 输出矩阵的行列数
        Matrix out(out_row, out_col, 0);
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
    void MatrixAdd(const Matrix &A, const Matrix &B, Matrix &add)
    {
        if (A.mat.size() != B.mat.size() || A.mat[0].size() != B.mat[0].size())
            cout << "MatrixAdd: Matrix size is not equal" << endl;
        int row = A.mat.size();
        int col = A.mat[0].size();
        add.mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            add.mat[i].resize(col); // 设置列数
            for (int j = 0; j < col; ++j)
            {
                add.mat[i][j] = A.mat[i][j] + B.mat[i][j];
            }
        }
    }
    // 广播加法
    void MatrixAdd(const Matrix &A, double val)
    {
        for (int i = 0; i < A.mat.size(); i++)
        {
            for (int j = 0; j < A.mat[0].size(); j++)
            {
                mat[i][j] = A.mat[i][j] + val;
            }
        }
    }
    // 加法重载，将B加到A中
    void MatrixAdd(Matrix &A, const Matrix &B)
    {
        if (A.mat.size() != B.mat.size() || A.mat[0].size() != B.mat[0].size())
            cout << "MatrixAdd: Matrix size is not equal" << endl;
        int row = A.mat.size();
        int col = A.mat[0].size();
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                A.mat[i][j] += B.mat[i][j];
            }
        }
    }
    // 矩阵减法,A减去B,结果存储在sub中
    void MatrixMinus(const Matrix &A, const Matrix &B, Matrix &R)
    {
        if (A.mat.size() != B.mat.size() || A.mat[0].size() != B.mat[0].size())
            cout << "MatrixMinus: Matrix size is not equal" << endl;
        int row = A.mat.size();
        int col = A.mat[0].size();
        R.mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            R.mat[i].resize(col); // 设置列数
            for (int j = 0; j < col; ++j)
            {
                R.mat[i][j] = A.mat[i][j] - B.mat[i][j];
            }
        }
    }
    // 矩阵间乘法
    void MatrixMul(const Matrix &A, const Matrix &B, Matrix &mul)
    {
        if (A.mat[0].size() != B.mat.size())
            cout << "MatrixMul: Matrix size is not equal" << endl;
        int row = A.mat.size();
        int col = B.mat[0].size();
        int mid = A.mat[0].size();
        mul.mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            mul.mat[i].resize(col); // 设置列数
            for (int j = 0; j < col; ++j)
            {
                mul.mat[i][j] = 0;
                for (int k = 0; k < mid; ++k)
                {
                    mul.mat[i][j] += A.mat[i][k] * B.mat[k][j];
                }
            }
        }
    }
    // 矩阵与标量相乘
    void MatrixMul(Matrix &A, double val)
    {
        for (int i = 0; i < A.mat.size(); i++)
        {
            for (int j = 0; j < A.mat[0].size(); j++)
            {
                A.mat[i][j] *= val;
            }
        }
    }
    // 矩阵与标量相乘
    void MatrixMul(const Matrix &A, double val, Matrix &R)
    {
        R.mat.resize(A.mat.size()); // 设置行数
        for (int i = 0; i < A.mat.size(); ++i)
        {
            R.mat[i].resize(A.mat[0].size()); // 设置列数
            for (int j = 0; j < A.mat[0].size(); ++j)
            {
                R.mat[i][j] = A.mat[i][j] * val;
            }
        }
    }
    // 矩阵点乘
    void MatrixDotMul(const Matrix &A, const Matrix &B, Matrix &R)
    {
        if (A.mat.size() != B.mat.size() || A.mat[0].size() != B.mat[0].size())
            cout << "MatrixDotMul: Matrix size is not equal" << endl;
        int row = A.mat.size();
        int col = A.mat[0].size();
        R.mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            R.mat[i].resize(col); // 设置列数
            for (int j = 0; j < col; ++j)
            {
                R.mat[i][j] = A.mat[i][j] * B.mat[i][j];
            }
        }
    }
    // 矩阵池化-最大池化
    // pool_size:池化核大小
    // stride:步长
    void MatrixPool(const Matrix &A, Matrix &R, int pool_size, int stride)
    {
        int row = A.mat.size();
        int col = A.mat[0].size();
        int out_row = (row - pool_size) / stride + 1; // 输出矩阵的行列数
        int out_col = (col - pool_size) / stride + 1; // 输出矩阵的行列数
        R.mat.resize(out_row);                        // 设置行数
        for (int i = 0; i < out_row; ++i)
        {
            R.mat[i].resize(out_col); // 设置列数
            for (int j = 0; j < out_col; ++j)
            {
                R.mat[i][j] = 0;
                for (int m = 0; m < pool_size; ++m)
                {
                    for (int n = 0; n < pool_size; ++n)
                    {
                        R.mat[i][j] = max(R.mat[i][j], A.mat[i * stride + m][j * stride + n]);
                    }
                }
            }
        }
    }
    // 矩阵转置
    void MatrixTranspose(const Matrix &A, Matrix &R)
    {
        int row = A.mat.size();
        int col = A.mat[0].size();
        R.mat.resize(col); // 设置行数
        for (int i = 0; i < col; ++i)
        {
            R.mat[i].resize(row); // 设置列数
            for (int j = 0; j < row; ++j)
            {
                R.mat[i][j] = A.mat[j][i];
            }
        }
    }
    // 矩阵旋转180度
    void MatrixRotate180(const Matrix &A, Matrix &R)
    {
        int row = A.mat.size();
        int col = A.mat[0].size();
        R.mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            R.mat[i].resize(col); // 设置列数
            for (int j = 0; j < col; ++j)
            {
                R.mat[i][j] = A.mat[row - i - 1][col - j - 1];
            }
        }
    }
    // 求和
    double MatrixSum(const Matrix &A)
    {
        double sum = 0;
        for (int i = 0; i < A.mat.size(); i++)
        {
            for (int j = 0; j < A.mat[0].size(); j++)
            {
                sum += A.mat[i][j];
            }
        }
        return sum;
    }
    // 打印矩阵
    void MatrixPrint()
    {
        for (int i = 0; i < mat.size(); i++)
        {
            for (int j = 0; j < mat[0].size(); j++)
            {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }
};
#endif // LENET_5_MATRIX_H

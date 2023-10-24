//
// Created by Hyper on 23/10/2023.
//

#ifndef LENET_5_MATRIX_H
#define LENET_5_MATRIX_H
#include <iostream>
#include <vector>
#include <cmath>

//矩阵的类实现
/*
 *函数：
 */

using namespace std;

typedef vector<vector<double>> Mat;
using namespace std;
class Matrix
{
private:
    Mat mat;
public:
    //构造函数
    Matrix(size_t row, size_t col)
    {
        if(row <= 0 || col <= 0)
            cout<< "row or col is illegal" << endl;
        mat.resize(row);//设置行数
        for (int i = 0; i < row; ++i)
        {
            mat[i].resize(col);//设置列数
        }
    }
    //赋值构造函数
    Matrix(size_t row, size_t col, double val)
    {
        if(row <= 0 || col <= 0)
            cout<< "row or col is illegal" << endl;
        mat.resize(row);//设置行数
        for (int i = 0; i < row; ++i)
        {
            mat[i].resize(col);//设置列数
            for (int j = 0; j < col; ++j)
            {
                mat[i][j] = val;
            }
        }
    }
    //矩阵二维卷积
    static Matrix conv2d(const Mat &A, const Mat &kernel)
    {
        if(kernel.size() > A.size() || kernel[0].size() > A[0].size())
            cout << "kernel size is illegal" << endl;
        int row = A.size();
        int col = A[0].size();
        int kernel_row = kernel.size();
        int kernel_col = kernel[0].size();
        int out_row = row- kernel_row + 1;//输出矩阵的行列数
        int out_col = col - kernel_col + 1;//输出矩阵的行列数
        Matrix out(out_row, out_col, 0);
        for(int i = 0; i< out_row; i++){
            for(int j = 0; j< out_col; j++){
                for(int m = 0; m< kernel_row; m++){
                    for(int n = 0; n< kernel_col; n++){
                        out.mat[i][j] += A[i+m][j+n] * kernel[m][n];
                    }
                }
            }
        }
        return out;
    }
    //矩阵加法,A加上B,结果存储在Mat中
    static Matrix MatrixAdd(const Matrix &A, const Matrix &B, Matrix &add)
    {
        if(A.mat.size() != B.mat.size() || A.mat[0].size() != B.mat[0].size())
            cout << "MatrixAdd: Matrix size is not equal" << endl;
        int row = A.mat.size();
        int col = A.mat[0].size();
        add.mat.resize(row);//设置行数
        for (int i = 0; i < row; ++i)
        {
            add.mat[i].resize(col);//设置列数
            for (int j = 0; j < col; ++j)
            {
                add.mat[i][j] = A.mat[i][j] + B.mat[i][j];
            }
        }
    }
    //广播加法
    void MatrixAdd(const Matrix &A, double val)
    {
        for(int i = 0; i< A.mat.size(); i++){
            for(int j = 0; j< A.mat[0].size(); j++){
                mat[i][j] = A.mat[i][j] + val;
            }
        }
    }
};
#endif //LENET_5_MATRIX_H

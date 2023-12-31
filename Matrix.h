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
const double Inf = 3.40282e+6;

typedef vector<vector<float>> Mat;
using namespace std;

class Matrix
{
public:
    Mat mat;
    int row, col;
    // 构造函数
    Matrix()
    {
    }
    ~Matrix()
    {
    }
    Matrix(int row, int col) : row(row), col(col)
    {
        if (row <= 0 || col <= 0)
            cout << "row or col is illegal" << endl, exit(-1);
        mat.resize(row); // 设置行数
        for (int i = 0; i < row; ++i)
        {
            mat[i].resize(col); // 设置列数
            for (int j = 0; j < col; j++)
                mat[i][j] = 0;
        }
    }
    Matrix(int row, int col, bool Guass) : row(row), col(col)
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
        else
            for (auto i = mat.begin(); i != mat.end(); i++)
            {
                for (auto j = i->begin(); j != i->end(); j++)
                {
                    *j = 0;
                }
            }
    }

    Matrix(const Matrix &a)
    {
        row = a.row, col = a.col;
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

    void Normalise()
    {
        double mean = MatrixSum() / (row * col), variance = 0;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                variance += pow(mat[i][j] - mean, 2);
            }
        }
        variance = variance / (row * col);
        double sq_var = sqrt(variance);
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                mat[i][j] = (mat[i][j] - mean) / sq_var;
            }
        }
    }

    void NAN_Gaussian_init()
    {
        for (auto i = mat.begin(); i != mat.end(); i++)
        {
            for (auto j = i->begin(); j != i->end(); j++)
            {
                if (isnan(*j) || isinf(abs(*j)))
                    *j = distr(gen);
            }
        }
    }

    void resize(int n, int m)
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

    int find_vector_aumax()
    {
        if (col != 1)
            cout << "Not a vector!\n"
                 << endl,
                exit(0);
        int flag = 0;
        float temp_max = 0;
        for (int i = 0; i < row; i++)
        {
            if (mat[i][0] > temp_max)
                flag = i, temp_max = mat[i][0];
        }
        return flag;
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

    void fill_zeros()
    {
        for (auto i = mat.begin(); i != mat.end(); i++)
        {
            for (auto j = i->begin(); j != i->end(); j++)
            {
                *j = 0;
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
    Matrix MatrixPool(size_t height, size_t width);
    void ReshapeMat(size_t t_row, size_t t_col);

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

// 矩阵均匀池化
Matrix Matrix::MatrixPool(const size_t height, const size_t width)
{
    if (mat.size() % height != 0 || mat[0].size() % width != 0)
        cout << "src size is illegal" << endl, exit(-1);
    Matrix res = Matrix(mat.size() / height, mat[0].size() / width);
    for (int i = 0; i < res.row; i++)
    {
        for (int j = 0; j < res.col; j++)
        {
            res.mat[i][j] = 0;
            int row_b = i * height;     // 起始行
            int col_b = j * width;      // 起始列
            int row_e = row_b + height; // 结束行
            int col_e = col_b + width;  // 结束列
            for (int m = row_b; m < row_e; m++)
            {
                for (int n = col_b; n < col_e; n++)
                {
                    res.mat[i][j] += mat[m][n];
                }
            }
        }
    }
    return res;
}

// 矩阵二维卷积
Matrix conv2d(const Mat &A, const Mat &kernel)
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
    if (col != B.row)
        cout << "MatrixMul: Matrix size is not fit" << endl, exit(-1);
    Matrix mul(row, B.col);
    int col = B.mat[0].size();
    int mid = mat[0].size();
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

double Cross_entropy(const Matrix &y, const Matrix &t)
{
    if (y.col != 1 || t.col != 1)
        cout << "Not a vector!" << endl, exit(-1);
    if (y.row != t.row)
        cout << "The two vector dosen't fit!" << endl, exit(-1);
    double loss = 0.;
    int m = y.row;
    for (auto i = 0; i < m; ++i)
        loss += -t.mat[i][0] * log(y.mat[i][0]);
    if (isnan(loss) || isinf(loss))
        loss = 0;
    return loss;
}

float relu(float x)
{
    return x ? 0 : x > 0;
}

Matrix Softmax(const Matrix &x)
{
    if (x.col != 1)
        cout << "Not a vector!" << endl, exit(-1);
    Matrix temp(x.row, 1);
    double sum = 0;
    for (int i = 0; i < x.row; i++)
    {
        sum += exp(x.mat[i][0]);
    }
    for (int i = 0; i < x.row; i++)
    {
        temp.mat[i][0] = exp(x.mat[i][0]) / sum;
    }
    return temp;
}

Matrix Softmax(const Matrix &x, float sum)
{
    if (x.col != 1)
        cout << "Not a vector!" << endl, exit(-1);
    Matrix temp(x.row, 1);
    int count = 0;
    for (int i = 0; i < x.row; i++)
    {
        temp.mat[i][0] = exp(x.mat[i][0]) / sum;
        if (isinf(exp(x.mat[i][0])))
            temp.mat[i][0] = 1, count++;
    }
    if (count != 0)
    {
        for (int i = 0; i < x.row; i++)
            temp.mat[i][0] /= count;
    }
    return temp;
}

Matrix Relu(const Matrix &x)
{
    Matrix temp(x.row, x.col);
    temp = x;
    for (int i = 0; i < x.row; i++)
        for_each(temp.mat[i].begin(), temp.mat[i].end(), relu);
    return temp;
}

// 交叉熵损失函数对输出层的求导
double d_Cross_entrophy(const Matrix &y, const Matrix &t)
{
    double loss;
    if (y.col != 1 || t.col != 1)
        cout << "Not a vector!" << endl, exit(-1);
    if (y.row != t.row)
        cout << "The two vector dosen't fit!" << endl, exit(-1);
    Matrix temp(y.row, 1);
    for (int i = 0; i < y.row; i++)
    {
        temp.mat[i][0] = -t.mat[i][0] / y.mat[i][0] + (1 - t.mat[i][0]) / (1 - y.mat[i][0]);
    }
    loss = temp.MatrixSum();
    return loss;
}

void Matrix::ReshapeMat(size_t t_row, size_t t_col)
{
    if (t_row <= 0 || t_col <= 0)
        throw "reshapeMatrix: row <= 0 || col <= 0";
    row = t_row, col = t_col;
    mat.resize(row);
    for (int i = 0; i < row; i++)
        mat[i].resize(col);
}

#endif // LENET_5_MATRIX_H

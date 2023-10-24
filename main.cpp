#include <iostream>
#include "Matrix.h"

int main() {
    // 创建两个矩阵 A 和 B
    Matrix A(3, 3, 1.0);
    Matrix B(3, 3, 2.0);

    // 打印矩阵 A 和 B
    cout << "Matrix A:" << endl;
    A.MatrixPrint();
    cout << "Matrix B:" << endl;
    B.MatrixPrint();

    // 创建一个结果矩阵 add
    Matrix add(3, 3);

    // 调用矩阵加法函数
    add.MatrixAdd(A, B, add);

    // 打印矩阵 add
    cout << "Matrix A + B:" << endl;
    add.MatrixPrint();

    //创建一个乘法矩阵
    Matrix C(3, 3);

    //调用矩阵乘法函数
    C.MatrixMul(A, B, C);

    //打印矩阵C
    cout << "Matrix A * B:" << endl;
    C.MatrixPrint();

    // 创建一个结果矩阵 conv_result
    Matrix conv_result(2, 2);

    // 创建两个输入矩阵 input 和 kernel
    Mat input = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    Mat kernel = {{1, 0}, {0, -1}};

    // 调用矩阵卷积函数
    conv_result = Matrix::conv2d(input, kernel);

    // 打印卷积结果
    cout << "Convolution Result:" << endl;
    conv_result.MatrixPrint();

    //池化层
    Matrix pool_result(2, 2);

    //调用矩阵池化函数
    pool_result.Matrix::MatrixPool(A, pool_result, 2 , 1);

    //打印池化结果
    cout << "Pooling Result:" << endl;
    pool_result.MatrixPrint();

    //标量相乘
    pool_result.MatrixMul(pool_result, 1.2);

    //打印标量相乘结果
    cout << "Scalar Multiplication Result:" << endl;
    pool_result.MatrixPrint();

    //广播操作
    Matrix broadcast_result(3, 3);
    broadcast_result.MatrixAdd(A,  22);

    //打印广播操作结果
    cout << "Broadcast Result:" << endl;
    broadcast_result.MatrixPrint();

    //Gaussian initual
    A.Gaussian_init();
    A.MatrixPrint();

    return 0;
}

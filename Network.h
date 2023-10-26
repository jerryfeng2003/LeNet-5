//
// Created by Hyper on 26/10/2023.
//

#ifndef LENET_5_NETWORK_H
#define LENET_5_NETWORK_H

#include "Matrix.h"
#include "DataInput.h"

Matrix Input;//输入层

vector<Matrix> Conv1_Core;      //卷积层1的卷积核  5*5@6
vector<double> Conv1_Bias;      //卷积层1的偏置   6*1
vector<Matrix> Conv1_Output;    //卷积层1的输出
vector<Matrix> Der_Conv1_Core;  //卷积层1的卷积核的导数   5*5@6
vector<double> Der_Conv1_Bias;  //卷积层1的偏置的导数    6*1
vector<Matrix> Conv1_Delta;     //卷积层1的误差项

const bool C3_connect[16][6] =
        {
                {true, true, true, false, false, false},
                {false, true, true, true, false, false},
                {false, false, true, true, true, false},
                {false, false, false, true, true, true},
                {true, false, false, false, true, true},
                {true, true, false, false, false, true},
                {true, true, true, true, false, false},
                {false, true, true, true, true, false},
                {false, false, true, true, true, true},
                {true, false, false, true, true, true},
                {true, true, false, false, true, true},
                {true, true, true, false, false, true},
                {true, true, false, true, true, false},
                {false, true, true, false, true, true},
                {true, false, true, true, false, true},
                {true, true, true, true, true, true}
        };
vector<vector<Matrix>> Conv3_Core;      //卷积层3的卷积核  5*5@16;
vector<double> Conv3_Bias;              //卷积层3的偏置   16*1
vector<vector<Matrix>> Conv3_Output;    //卷积层3的输出   16*10*10
vector<vector<Matrix>> Der_Conv3_Core;  //卷积层3的卷积核的导数   5*5@16
vector<double> Der_Conv3_Bias;          //卷积层3的偏置的导数    16*1
vector<vector<Matrix>> Conv3_Delta;     //卷积层3的误差项

vector<vector<Matrix>> Conv5_Core;      //卷积层5的卷积核  5*5@120;
vector<double> Conv5_Bias;              //卷积层5的偏置   120*1
vector<vector<Matrix>> Conv5_Output;    //卷积层5的输出   120*1*1
vector<vector<Matrix>> Der_Conv5_Core;  //卷积层5的卷积核的导数   5*5@120
vector<double> Der_Conv5_Bias;          //卷积层5的偏置的导数    120*1
vector<vector<Matrix>> Conv5_Delta;     //卷积层5的误差项

Matrix Full6_Weight;                    //全连接层6的权重  84*120
Matrix Full6_Bias;                      //全连接层6的偏置  84*1
Matrix Full6_Output;                    //全连接层6的输出  84*1
Matrix Der_Full6_Weight;                //全连接层6的权重的导数  84*120
Matrix Der_Full6_Bias;                  //全连接层6的偏置的导数  84*1
Matrix Full6_Delta;                     //全连接层6的误差项

Matrix OUTPUT_Weight;                   //输出层的权重  10*84
Matrix OUTPUT_Bias;                     //输出层的偏置  10*1
Matrix OUTPUT_Output;                   //输出层的输出  10*1
Matrix Der_OUTPUT_Weight;               //输出层的权重的导数  10*84
Matrix Der_OUTPUT_Bias;                 //输出层的偏置的导数  10*1
Matrix OUTPUT_Delta;                    //输出层的误差项

void Network_init(void);                        //初始化网络
void ForwardPropagation(const Point& point);    //前向传播
void BackPropagation(const Point& point);       //反向传播
void Clear_Gradient(void);                      //清空梯度
void Update_Weight(double rate);                //更新权重
void Check_Gradient(const Point& point);        //检查梯度
void Train(vector<Point>& Train);               //训练
void Test(vector<Point>& Test);                 //测试
#endif //LENET_5_NETWORK_H

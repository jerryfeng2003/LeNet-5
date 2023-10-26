//
// Created by Hyper on 26/10/2023.
//
#include "Network.h"

void Network_init(void)
{
    //初始化卷积层1
    Conv1_Core.resize(6);
    Conv1_Bias.resize(6, 0);
    Conv1_Output.resize(6);
    Conv1_Delta.resize(6);
    Der_Conv1_Core.resize(6);
    Der_Conv1_Bias.resize(6, 0);

    for(int i = 0; i < 6; i++)
    {
        Conv1_Core[i] = Matrix(5, 5, true);
        Conv1_Output[i] = Matrix(28, 28);
        Conv1_Delta[i] = Matrix(28, 28);
        Der_Conv1_Core[i] = Matrix(5, 5);
    }
    
    //初始化池化层2
    SubSamp2_Params.resize(6);

    for (int i = 0; i < 6; i++)
    {
        SubSamp2_Params[i] = distr(gen);
    }
    
    SubSamp2_Bias.resize(6, 0);
    SubSamp2_Active.resize(6);
    SubSamp2_Output.resize(6);
    SubSamp2_Delta.resize(6);
    Der_SubSamp2_Params.resize(6);
    Der_SubSamp2_Bias.resize(6, 0);
    
    //初始化卷积层3
    Conv3_Core.resize(16);
    Conv3_Bias.resize(16, 0);
    Conv3_Output.resize(16);
    Conv3_Delta.resize(16);
    Der_Conv3_Core.resize(16);
    Der_Conv3_Bias.resize(16, 0);

    for(int i = 0; i < 16; i++)
    {
        Conv3_Core[i].resize(6);
        Der_Conv3_Core[i].resize(6);
        Conv1_Output[i] = Matrix(5, 5);
        for(int j = 0; j < 6; j++)
        {
            if( C3_connect[i][j] )
            {
                Conv3_Core[i][j] = Matrix(5, 5, true);
                Der_Conv3_Core[i][j] = Matrix(5, 5);
            }
        }
    }
    
    //初始化池化层4
    SubSamp4_Params.resize(16, 1);

    for(int i = 0; i < 16; i++)
    {
        SubSamp4_Params[i] = distr(gen);
    }

    SubSamp4_Bias.resize(16, 0);
    SubSamp4_Active.resize(16);
    SubSamp4_Output.resize(16);
    SubSamp4_Delta.resize(16);
    Der_SubSamp4_Params.resize(16);
    Der_SubSamp4_Bias.resize(16, 0);
    
    //初始化卷积层5
    Conv5_Core.resize(120);
    Conv5_Bias.resize(120, 0);
    Conv5_Output = Matrix(120, 1, 0);
    Conv5_Delta.resize(120);
    Der_Conv5_Core.resize(120);
    Der_Conv5_Bias.resize(120, 0);

    for(int i = 0; i < 120; i++)
    {
        Conv5_Core[i].resize(16);
        Der_Conv5_Core[i].resize(16);
        for(int j = 0; j < 16; j++)
        {
            Conv5_Core[i][j] = Matrix(5, 5, true);
            Der_Conv5_Core[i][j] = Matrix(5, 5);
        }
    }

    //初始化全连接层6
    Full6_Weight = Matrix(84, 120, true);
    Full6_Bias = Matrix(84, 1, 0);
    Full6_Output = Matrix(84, 1);
    Full6_Delta = Matrix(84, 1);
    Der_Full6_Weight = Matrix(84, 120);
    Der_Full6_Bias = Matrix(84, 1, 0);

    //初始化输出层
    OUTPUT_Weight = Matrix(10, 84, true);
    OUTPUT_Bias = Matrix(10, 1, 0);
    OUTPUT_Output = Matrix(10, 1);
    OUTPUT_Delta = Matrix(10, 1);
    Der_OUTPUT_Weight = Matrix(10, 84);
    Der_OUTPUT_Bias = Matrix(10, 1, 0);
}

void ForwardPropagation(const Point& point)
{
    //输入
    Input = point.image;

    //卷积层1
    for(int i = 0; i < 6; i++)
    {
        Conv1_Output[i] = Matrix::conv2d(Input.mat, Conv1_Core[i].mat) + Conv1_Bias[i];
    }

    //池化层2
    for(int i = 0; i < 6; i++)
    {
        SubSamp2_Active[i] = Conv1_Output[i].MatrixPool( 2, 2);
        SubSamp2_Output[i] = SubSamp2_Active[i] * SubSamp2_Params[i] + SubSamp2_Bias[i];
        SubSamp2_Output[i] = Relu(SubSamp2_Output[i]);
    }

    //卷积层3
    for(int i = 0; i < 16; i++)
    {
        Conv3_Output[i] = Matrix(10, 10, 0);
        for(int j = 0; j < 6; j++)
        {
            if( C3_connect[i][j] )
            {
                Conv3_Output[i] = Conv3_Output[i] + Matrix::conv2d(SubSamp2_Output[j].mat, Conv3_Core[i][j].mat);
            }
        }
        Conv3_Output[i] = Conv3_Output[i] + Conv3_Bias[i];
    }

    //池化层4
    for (int i = 0; i < 16; ++i)
    {
        SubSamp4_Active[i] = Conv3_Output[i].MatrixPool( 2, 2);
        SubSamp4_Output[i] = SubSamp4_Active[i] * SubSamp4_Params[i] + SubSamp4_Bias[i];
        SubSamp4_Output[i] = Relu(SubSamp4_Output[i]);
    }

    //卷积层5
    for (int i = 0; i < 120; ++i)
    {
        Conv5_Output.mat[i][0] = Conv5_Bias[i];
        Matrix temp = Matrix(1, 1, 0);
        for(int j = 0; j < 16; j++)
        {
            temp = temp + Matrix::conv2d(SubSamp4_Output[j].mat, Conv5_Core[i][j].mat);
        }
        Conv5_Output = Conv5_Output + temp;
    }

    //全连接层6
    Full6_Output = Conv5_Output * Full6_Weight + Full6_Bias;
    Full6_Output = Relu(Full6_Output);

    //输出层
    OUTPUT_Output = Full6_Output * OUTPUT_Weight + OUTPUT_Bias;
    OUTPUT_Output = Softmax(OUTPUT_Output);
}

int GetLabel(const Point &point)
{
    for(int i = 0; i < 10; i++)
    {
        if(point.label.mat[i][0] == 1)
        {
            return i;
        }
    }
    return -1;
}

void BackPropagation(const Point& point)
{
    //计算OUTPUT_Delta
    OUTPUT_Delta = OUTPUT_Output - point.label;

    //计算Der_OUTPUT_Weight
    Matrix OUTPUT_weight_T = OUTPUT_Weight.MatrixTranspose();
    Matrix temp = OUTPUT_weight_T * OUTPUT_Delta ;
    Full6_Delta.resize(84, 1);

    //计算Full_Delta
    for(int i = 0; i < 84; i++)
    {
        Full6_Delta.mat[i][0] = temp.mat[i][0] * (1.0 - Full6_Output.mat[i][0]) * (1 + Full6_Output.mat[i][0]);
    }

    //
}
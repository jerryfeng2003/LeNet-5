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

    //初始化卷积层5
    Conv5_Core.resize(120);
    Conv5_Bias.resize(120, 0);
    Conv5_Output.resize(120);
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
    Full6_Bias = Matrix(84, 1);
    Full6_Output = Matrix(84, 1);
    Full6_Delta = Matrix(84, 1);
    Der_Full6_Weight = Matrix(84, 120);
    Der_Full6_Bias = Matrix(84, 1);

    //初始化输出层
    OUTPUT_Weight = Matrix(10, 84, true);
    OUTPUT_Bias = Matrix(10, 1);
    OUTPUT_Output = Matrix(10, 1);
    OUTPUT_Delta = Matrix(10, 1);
    Der_OUTPUT_Weight = Matrix(10, 84);
    Der_OUTPUT_Bias = Matrix(10, 1);
}
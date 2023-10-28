//
// Created by Hyper on 26/10/2023.
//
#include "Network.h"

void LeNet_5::Network_init()
{
    // 初始化卷积层1
    Conv1_Core.resize(6);
    Conv1_Bias.resize(6, 0);
    Conv1_Output.resize(6);
    Conv1_Delta.resize(6);
    Der_Conv1_Core.resize(6);
    Der_Conv1_Bias.resize(6, 0);

    for (int i = 0; i < 6; i++)
    {
        Conv1_Core[i] = Matrix(5, 5, true);
        Conv1_Output[i] = Matrix(28, 28);
        Conv1_Delta[i] = Matrix(28, 28);
        Der_Conv1_Core[i] = Matrix(5, 5);
    }

    // 初始化池化层2
    SubSamp2_Params.resize(6);

    for (int i = 0; i < 6; i++)
    {
        SubSamp2_Params[i] = distr(gen);
    }

    SubSamp2_Bias.resize(6, 0);
    SubSamp2_Active.resize(6);
    SubSamp2_Output.resize(6);
    SubSamp2_inside.resize(6);
    SubSamp2_Delta.resize(6);
    Der_SubSamp2_Params.resize(6);
    Der_SubSamp2_Bias.resize(6, 0);

    // 初始化卷积层3
    Conv3_Core.resize(16);
    Conv3_Bias.resize(16, 0);
    Conv3_Output.resize(16);
    Conv3_Delta.resize(16);
    Der_Conv3_Core.resize(16);
    Der_Conv3_Bias.resize(16, 0);

    for (int i = 0; i < 16; i++)
    {
        Conv3_Core[i].resize(6);
        Der_Conv3_Core[i].resize(6);
        Conv1_Output[i] = Matrix(5, 5);
        for (int j = 0; j < 6; j++)
        {
            if (C3_connect[i][j])
            {
                Conv3_Core[i][j] = Matrix(5, 5, true);
                Der_Conv3_Core[i][j] = Matrix(5, 5);
            }
        }
    }

    // 初始化池化层4
    SubSamp4_Params.resize(16, 1);

    for (int i = 0; i < 16; i++)
    {
        SubSamp4_Params[i] = distr(gen);
    }

    SubSamp4_Bias.resize(16, 0);
    SubSamp4_Active.resize(16);
    SubSamp4_Output.resize(16);
    SubSamp4_inside.resize(16);
    SubSamp4_Delta.resize(16);
    Der_SubSamp4_Params.resize(16);
    Der_SubSamp4_Bias.resize(16, 0);

    // 初始化卷积层5
    Conv5_Core.resize(120);
    Conv5_Bias.resize(120, 0);
    Conv5_Output = Matrix(120, 1, 0);
    Conv5_Delta.resize(120);
    Der_Conv5_Core.resize(120);
    Der_Conv5_Bias.resize(120, 0);

    for (int i = 0; i < 120; i++)
    {
        Conv5_Core[i].resize(16);
        Der_Conv5_Core[i].resize(16);
        for (int j = 0; j < 16; j++)
        {
            Conv5_Core[i][j] = Matrix(5, 5, true);
            Der_Conv5_Core[i][j] = Matrix(5, 5);
        }
    }

    // 初始化全连接层6
    Full6_Weight = Matrix(84, 120, true);
    Full6_Bias = Matrix(84, 1, 0);
    Full6_Output = Matrix(84, 1);
    FUll6_inside = Matrix(84, 1);
    Full6_Delta = Matrix(84, 1);
    Der_Full6_Weight = Matrix(84, 120);
    Der_Full6_Bias = Matrix(84, 1, 0);

    // 初始化输出层
    OUTPUT_Weight = Matrix(10, 84, true);
    OUTPUT_Bias = Matrix(10, 1, 0);
    OUTPUT_Output = Matrix(10, 1);
    OUTPUT_inside = Matrix(10, 1);
    Der_OUTPUT_Weight = Matrix(10, 84);
    Der_OUTPUT_Bias = Matrix(10, 1, 0);
}

void LeNet_5::ForwardPropagation(const Point &point)
{
    // 输入
    Input = point.image;

    // 卷积层1
    for (int i = 0; i < 6; i++)
    {
        Conv1_Output[i] = Matrix::conv2d(Input.mat, Conv1_Core[i].mat) + Conv1_Bias[i];
    }

    // 池化层2
    for (int i = 0; i < 6; i++)
    {
        SubSamp2_Active[i] = Conv1_Output[i].MatrixPool(2, 2);
        SubSamp2_inside[i] = SubSamp2_Active[i] * SubSamp2_Params[i] + SubSamp2_Bias[i];
        SubSamp2_Output[i] = Relu(SubSamp2_inside[i]);
    }

    // 卷积层3
    for (int i = 0; i < 16; i++)
    {
        Conv3_Output[i] = Matrix(10, 10, 0);
        for (int j = 0; j < 6; j++)
        {
            if (C3_connect[i][j])
            {
                Conv3_Output[i] = Conv3_Output[i] + Matrix::conv2d(SubSamp2_Output[j].mat, Conv3_Core[i][j].mat);
            }
        }
        Conv3_Output[i] = Conv3_Output[i] + Conv3_Bias[i];
    }

    // 池化层4
    for (int i = 0; i < 16; ++i)
    {
        SubSamp4_Active[i] = Conv3_Output[i].MatrixPool(2, 2);
        SubSamp4_inside[i] = SubSamp4_Active[i] * SubSamp4_Params[i] + SubSamp4_Bias[i];
        SubSamp4_Output[i] = Relu(SubSamp4_inside[i]);
    }

    // 卷积层5
    for (int i = 0; i < 120; ++i)
    {
        Conv5_Output.mat[i][0] = Conv5_Bias[i];
        Matrix temp = Matrix(1, 1, 0);
        for (int j = 0; j < 16; j++)
        {
            temp = temp + Matrix::conv2d(SubSamp4_Output[j].mat, Conv5_Core[i][j].mat);
        }
        Conv5_Output.mat[i][0] = Conv5_Output.mat[i][0] + temp.mat[0][0];
    }

    // 全连接层6
    FUll6_inside = Conv5_Output * Full6_Weight + Full6_Bias;
    Full6_Output = Relu(FUll6_inside);

    // 输出层
    OUTPUT_inside = Full6_Output * OUTPUT_Weight + OUTPUT_Bias;
    Exp_output.resize(OUTPUT_inside.row);
    for (int i = 0; i < OUTPUT_inside.row; i++)
    {
        Exp_output[i] = exp(OUTPUT_inside.mat[i][0]);
    }
    OUTPUT_Output = Softmax(OUTPUT_inside, accumulate(Exp_output.begin(), Exp_output.end(), 0.0));
}

int LeNet_5::GetLabel(const Point &point)
{
    for (int i = 0; i < 10; i++)
    {
        if (point.label.mat[i][0] == 1)
        {
            return i;
        }
    }
    return -1;
}

void LeNet_5::BackPropagation(const Point &point)
{
    Matrix Label = point.label;
    float Loss = Cross_entropy(OUTPUT_Output, Label);
    float Exp_sum = accumulate(Exp_output.begin(), Exp_output.end(), 0.0);
    for (int i = 0; i < Label.row; i++)
    {
        Der_OUTPUT_Bias.mat[i][0] = (-Label.mat[i][0] / OUTPUT_Output.mat[i][0]) * (Exp_sum - Exp_output[i]) * Exp_output[i] / (pow(Exp_sum, 2));
        for (int j = 0; j < Full6_Weight.col; j++)
        {
            Der_OUTPUT_Weight.mat[i][j] = Der_OUTPUT_Bias.mat[i][0] * OUTPUT_inside.mat[i][j];
        }
    }

    for (int j = 0; j < Full6_Output.row; j++)
    {
        for (int i = 0; i < Label.row; i++)
        {
            Der_Full6_Bias.mat[j][0] += Der_OUTPUT_Bias.mat[i][0] * OUTPUT_Weight.mat[i][j];
        }
        Der_Full6_Bias.mat[j][0] *= (FUll6_inside.mat[j][0] > 0);
        for (int k = 0; k < Conv5_Output.row; k++)
        {
            Der_Full6_Weight.mat[j][k] = Der_Full6_Bias.mat[j][0] * Conv5_Output.mat[k][0];
        }
    }

    for (int j = 0; j < Conv5_Output.row; j++)
    {
        for (int i = 0; i < Full6_Output.row; i++)
        {
            Der_Conv5_Bias[j] += Der_Full6_Bias.mat[i][0] * Full6_Weight.mat[i][j];
        }
        Der_Conv5_Bias[j] = Der_Conv5_Bias[j] * (Conv5_Output.mat[j][0] > 0);
        for (int i = 0; i < Conv5_Core[0].size(); i++)
        {
            for (int k = 0; k < 5; k++)
            {
                for (int m = 0; m < 5; m++)
                {
                    Der_Conv5_Core[j][i].mat[k][m] = Der_Conv5_Bias[j] * SubSamp4_Output[i].mat[k][m];
                }
            }
        }
    }

    for (int j = 0; j < SubSamp4_Output.size(); j++)
    {
        double sum(0), sum_x(0);
        for (int k = 0; k < 5; k++)
        {
            for (int m = 0; m < 5; m++)
            {
                if (SubSamp4_inside[j].mat[k][m] > 0)
                    sum++;
                sum_x += SubSamp2_Active[j].mat[k][m];
            }
        }
        for (int i = 0; i < Conv5_Output.row; i++)
        {
            Der_SubSamp4_Bias[j] += Der_Conv5_Bias[i];
        }
        Der_SubSamp4_Bias[j] *= sum;
        Der_SubSamp4_Params[j] = Der_SubSamp2_Bias[j] * sum_x;
    }

    for (int j = 0; j < Conv3_Output.size(); j++)
    {
        Der_Conv3_Bias[j] = Der_SubSamp4_Bias[j] * SubSamp4_Params[j];
        for (int i = 0; i < SubSamp2_Output.size(); i++)
        {
            if (C3_connect[j][i])
            {
                for (int k = 0; k < 5; k++)
                {
                    for (int m = 0; m < 5; m++)
                    {
                        for (int s = k; s < 14 - (5 - k); s++)
                        {
                            for (int t = m; t < 14 - (5 - m); t++)
                            {
                                Der_Conv3_Core[j][i].mat[k][m] += Der_Conv3_Bias[j] * SubSamp2_Output[i].mat[s][t] / 4;
                            }
                        }
                    }
                }
            }
        }
    }

    for (int j = 0; j < SubSamp2_Output.size(); j++)
    {
        double sum_der_cov(0), sum_poi(0), sum_x(0);
        for (int i = 0; i < SubSamp2_Output.size(); i++)
        {
            for (int k = 0; k < Conv3_Output.size(); k++)
            {
                if (C3_connect[k][i])
                {
                    for (int m = 0; m < 5; m++)
                    {
                        for (int t = 0; t < 5; t++)
                        {
                            sum_der_cov += Conv3_Core[k][i].mat[m][t];
                        }
                    }
                    sum_der_cov *= pow(14 - 5 + 1, 2);
                }
            }
        }
        for (int i = 0; i < Der_Conv3_Bias.size(); i++)
        {
            Der_SubSamp2_Bias[j] += Der_Conv1_Bias[i];
        }
        Der_SubSamp2_Bias[j] *= sum_der_cov;
        for (int i = 0; i < 14; i++)
        {
            for (int k = 0; k < 14; k++)
            {
                sum_poi += SubSamp2_inside[j].mat[i][k] > 0;
                sum_x += SubSamp2_Active[j].mat[i][k];
            }
        }
        Der_SubSamp2_Bias[j] *= sum_poi;
        Der_SubSamp2_Params[j] = Der_SubSamp2_Bias[j] * sum_x;
    }

    for (int j = 0; j < Conv1_Output.size(); j++)
    {
        Der_Conv1_Bias[j] = Der_SubSamp2_Bias[j] * SubSamp2_Params[j];
        for (int k = 0; k < 5; k++)
        {
            for (int m = 0; m < 5; m++)
            {
                for (int s = k; s < 32 - (5 - k); s++)
                {
                    for (int t = m; t < 32 - (5 - m); t++)
                    {
                        Der_Conv1_Core[j].mat[k][m] += Der_Conv1_Bias[j] * Input.mat[s][t] / 4;
                    }
                }
            }
        }
    }
}

void LeNet_5::Clear_Gradient()
{
    for (int i = 0; i < Der_Conv1_Core.size(); i++)
    {
        Der_Conv1_Core[i].fill_zeros();
        Der_Conv1_Bias[i] = 0;
    }

    fill(Der_SubSamp2_Bias.begin(), Der_SubSamp2_Bias.end(), 0);
    fill(Der_SubSamp2_Params.begin(), Der_SubSamp2_Params.end(), 0);

    for (int i = 0; i < Der_Conv3_Core.size(); i++)
    {
        for (int j = 0; j < Der_Conv3_Core[0].size(); j++)
        {
            Der_Conv3_Core[i][j].fill_zeros();
        }
        Der_Conv3_Bias[i] = 0;
    }

    fill(Der_SubSamp4_Bias.begin(), Der_SubSamp4_Bias.end(), 0);
    fill(Der_SubSamp4_Params.begin(), Der_SubSamp4_Params.end(), 0);

    for (int i = 0; i < Der_Conv5_Core.size(); i++)
    {
        for (int j = 0; j < Der_Conv5_Core[0].size(); j++)
        {
            Der_Conv5_Core[i][j].fill_zeros();
        }
        Der_Conv5_Bias[i] = 0;
    }

    Der_Full6_Bias.fill_zeros();
    Der_Full6_Weight.fill_zeros();

    Der_OUTPUT_Bias.fill_zeros();
    Der_OUTPUT_Weight.fill_zeros();
}

void LeNet_5::Update_Weight(double rate)
{
    double lambda = 1e-4;
    for (int i = 0; i < Conv1_Core.size(); i++)
    {
        Conv1_Core[i] = Conv1_Core[i] - (Der_Conv1_Core[i] + Conv1_Core[i] * lambda) * rate;
        Conv1_Bias[i] -= rate * (Der_Conv1_Bias[i] + lambda * Conv1_Bias[i]);
    }

    for (int i = 0; i < SubSamp2_Params.size(); i++)
    {
        SubSamp2_Params[i] -= rate * (Der_SubSamp2_Params[i] + lambda * SubSamp2_Params[i]);
        SubSamp2_Bias[i] -= rate * (Der_SubSamp2_Bias[i] + lambda * SubSamp2_Bias[i]);
    }

    for (int i = 0; i < Conv3_Core.size(); i++)
    {
        for (int j = 0; j < Conv3_Core[0].size(); j++)
        {
            Conv3_Core[i][j] = Conv3_Core[i][j] - (Der_Conv3_Core[i][j] + Conv3_Core[i][j] * lambda) * rate;
        }
        Conv3_Bias[i] -= rate * (Der_Conv3_Bias[i] + lambda * Conv3_Bias[i]);
    }

    for (int i = 0; i < SubSamp4_Params.size(); i++)
    {
        SubSamp4_Params[i] -= rate * (Der_SubSamp4_Params[i] + lambda * SubSamp4_Params[i]);
        SubSamp4_Bias[i] -= rate * (Der_SubSamp4_Bias[i] + lambda * SubSamp4_Bias[i]);
    }

    for (int i = 0; i < Conv5_Core.size(); i++)
    {
        for (int j = 0; j < Conv5_Core[0].size(); j++)
        {
            Conv5_Core[i][j] = Conv5_Core[i][j] - (Der_Conv5_Core[i][j] + Conv5_Core[i][j] * lambda) * rate;
        }
        Conv5_Bias[i] -= rate * (Der_Conv5_Bias[i] + lambda * Conv5_Bias[i]);
    }

    Full6_Weight = Full6_Weight - (Der_Full6_Weight + Full6_Weight * lambda) * rate;
    Full6_Bias = Full6_Bias - (Der_Full6_Bias + Full6_Bias * lambda) * rate;

    OUTPUT_Weight = OUTPUT_Weight - (Der_OUTPUT_Weight + OUTPUT_Weight * lambda) * rate;
    OUTPUT_Bias = OUTPUT_Bias - (Der_OUTPUT_Bias + OUTPUT_Bias * lambda) * rate;
}


void Train(vector<Point> &Train)
{
    
}
//
// Created by Hyper on 24/10/2023.
//

#ifndef LENET_5_DATAINPUT_H
#define LENET_5_DATAINPUT_H
#include "Matrix.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>

using namespace std;

//反转int
int ReverseInt(int i)
{
    unsigned char *split = (unsigned char *) &i;
    return (int) split[0] << 24 | (int) split[1] << 16 | (int) split[2] << 8 | (int) split[3];
}

class Imagedatas
{
public:
    vector<Matrix> train_data;
    vector<Matrix> train_label;
    vector<Matrix> test_data;
    vector<Matrix> test_label;

    int train_image_num;
    int train_label_num;
    int test_image_num;
    int test_label_num;

    void read_image_datas(bool train)
    {
        FILE *fp = NULL;
        int errNum = 0;
        if(train)
            fp = fopen("C:\\Users\\Hyper\\CLionProjects\\LeNet-5\\train-images.idx3-ubyte", "rb");
        else
            fp = fopen("t10k-images.idx3-ubyte", "rb");

        if(fp == NULL)
        {
            errNum = errno;
            cout << "open fail errno = "<<errNum<<endl;
            cout << "open train-images.idx3-ubyte failed!" << endl;
            exit(-1);
        }

        int rubbish;
        int num_of_images;
        int num_of_rows;
        int num_of_cols;
        //清除垃圾值
        fread(&rubbish, sizeof(int), 1, fp);

        //读取图片数量
        fread(&num_of_images, sizeof(int), 1, fp);
        num_of_images = ReverseInt(num_of_images);
        //读取行数
        fread(&num_of_rows, sizeof(int), 1, fp);
        num_of_rows = ReverseInt(num_of_rows);
        //读取列数
        fread(&num_of_cols, sizeof(int), 1, fp);
        num_of_cols = ReverseInt(num_of_cols);
        //获取第i张图片
        int i, r, c;

        for(i = 0; i < num_of_images; ++i)
        {
            Matrix temp(num_of_rows, num_of_cols);
            for(r = 0; r < num_of_rows; ++r)
            {
                for(c = 0; c < num_of_cols; ++c)
                {
                    unsigned char temp_pixel = 0;
                    fread(&temp_pixel, sizeof(unsigned char), 1, fp);
                    temp.mat[0][r * 28 + c] = (float) temp_pixel / 255;
                }
            }
            if(train)
                train_data.push_back(temp);
            else
                test_data.push_back(temp);
        }
        fclose(fp);
    }

    void read_label_datas(bool train)
    {
        FILE *fp = NULL;
        if(train)
            fp = fopen("train-labels.idx1-ubyte", "rb");
        else
            fp = fopen("t10k-labels.idx1-ubyte", "rb");

        if(fp == NULL)
        {
            cout << "open train-labels.idx1-ubyte failed!" << endl;
            exit(-1);
        }

        int rubbish;
        int num_of_label;
        //清除垃圾值
        fread(&rubbish, sizeof(int), 1, fp);

        //读取图片数量
        fread(&num_of_label, sizeof(int), 1, fp);
        num_of_label = ReverseInt(num_of_label);
        //获取第i个标签
        int i;

        for(i = 0; i < num_of_label; ++i)
        {
            Matrix temp(10, 1);
            unsigned char temp_label = 0;
            fread(&temp_label, sizeof(unsigned char), 1, fp);
            temp.mat[0][temp_label] = 1;
            if(train)
                train_label.push_back(temp);
            else
                test_label.push_back(temp);
        }
        fclose(fp);
    }
};


#endif //LENET_5_DATAINPUT_H

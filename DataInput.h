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

#define INPUT_HEIGHT 32
#define INPUT_WIDTH  32//输入图片的长宽
#define MNISI_HEIGHT 28
#define MNISI_WIDTH  28//MNIST图片的长宽
#define PADDING_SIZE 2//填充大小
#define OUTPUT_SIZE 10//输出大小
#define TRAIN_SIZE 60000//训练集大小
#define TEST_SIZE  10000//测试集大小

using namespace std;

struct Point
        {
    Matrix image;
    Matrix label;
    //构造函数
    Point(int num, string URL, int flag);
};

//flag: 0表示训练集，1表示测试集
Point:: Point(int num, const string URL, int flag):image(INPUT_HEIGHT, INPUT_WIDTH, 0), label(OUTPUT_SIZE, 1)
{
    uint32_t image_number[4];   //魔数, 图片数量, 行数, 列数
    uint32_t label_number[2];   //魔数, 标签数量
    string image_url;
    string label_url;
    if (flag == 0)
    {
        image_url = URL + "train-images.idx3-ubyte";
        label_url = URL + "train-labels.idx1-ubyte";
        if(num < 0 || num > TRAIN_SIZE)
        {
            cout<<"num error!"<<endl;
            exit(0);
        }
    }
    else if (flag == 1)
    {
        image_url = URL + "t10k-images.idx3-ubyte";
        label_url = URL + "t10k-labels.idx1-ubyte";
        if(num < 0 || num > TEST_SIZE)
        {
            cout<<"num error!"<<endl;
            exit(0);
        }
    }
    else
    {
        cout<<"flag error!"<<endl;
        exit(0);
    }

    ifstream train_image(image_url, ios::binary | ios::in );
    ifstream train_label(label_url, ios::binary | ios::in );

    if(!train_image.is_open() || !train_label.is_open())//打开文件失败
    {
        cout<<"Open file failed!"<<endl;
        exit(0);
    }

    train_image.read((char*)&image_number, sizeof (image_number));
    train_label.read((char*)&label_number, sizeof (label_number));

    char *temp_image = new char[MNISI_HEIGHT*MNISI_WIDTH];
    //移动文件指针，读取第n张图片
    train_image.seekg(num*MNISI_HEIGHT*MNISI_WIDTH, ios::cur);
    train_image.read(temp_image, MNISI_HEIGHT*MNISI_WIDTH);
    char temp_label;
    //移动文件指针，读取第n个标签
    train_label.seekg(num, ios::cur);
    train_label.read(&temp_label, 1);//标签的字节数为1
    //存入Train
    this->image = Matrix(INPUT_HEIGHT, INPUT_WIDTH);
    for(int i = 0; i < MNISI_HEIGHT; i++)
    {
        for(int j = 0; j < MNISI_HEIGHT; j++)
        {
            this->image.mat[i+PADDING_SIZE][j+PADDING_SIZE] =(uint8_t)temp_image[i*MNISI_HEIGHT+j];
        }
    }
    this->label = Matrix(OUTPUT_SIZE, 1);
    this->label.mat[(uint8_t)temp_label][0] = 1;
}
#endif //LENET_5_DATAINPUT_H

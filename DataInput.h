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
#define TRAIN_NUM 3000//训练集大小
#define TEST_NUM  100//测试集大小
using namespace std;

struct Point
        {
    Matrix image;
    Matrix label;
    Point() { ; }
    Point(const char * image, uint8_t num )
    {
        this->image = Matrix(INPUT_HEIGHT, INPUT_WIDTH);
        for(int i = 0; i < INPUT_HEIGHT; i++)
        {
            for(int j = 0; j < INPUT_WIDTH; j++)
            {
                this->image.mat[i+PADDING_SIZE][j+PADDING_SIZE] =(uint8_t) image[i*MNISI_HEIGHT+j];
            }
        }
        this->label = Matrix(OUTPUT_SIZE, 1);
        label.mat[num][0] = 1;
    }
};

inline void ReadData(vector<Point>&Train, vector<Point>&Test)
{
    char rubbish[16];//不用的数据，包括图片数量，图片长宽等
    ifstream train_image("./Datas/train-images.idx3-ubyte", ios::binary | ios::in );
    ifstream train_label("./Datas/train-labels.idx1-ubyte", ios::binary | ios::in);

    train_image.read(rubbish, 16);
    train_label.read(rubbish, 8);

    for(int i = 0; i < TRAIN_NUM; i++)
    {
        char image[MNISI_HEIGHT*MNISI_WIDTH];
        uint8_t num;
        train_image.read(image, MNISI_HEIGHT*MNISI_WIDTH);
        train_label.read((char*)&num, 1);//标签的字节数为1
        Train.push_back(Point(image, num));
    }

    ifstream test_image("./Datas/t10k-images.idx3-ubyte", ios::binary | ios::in );
    ifstream test_label("./Datas/t10k-labels.idx1-ubyte", ios::binary | ios::in );

    test_image.read(rubbish, 16);
    test_label.read(rubbish, 8);

    for(int i = 0; i < TEST_NUM; i++)
    {
        char image[MNISI_HEIGHT*MNISI_WIDTH];
        uint8_t num;
        test_image.read(image, MNISI_HEIGHT*MNISI_WIDTH);
        test_label.read((char*)&num, 1);//标签的字节数为1
        Test.push_back(Point(image, num));
    }
}

#endif //LENET_5_DATAINPUT_H

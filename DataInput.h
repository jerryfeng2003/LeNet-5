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
#define TRAIN_NUM 20//训练集大小
#define TEST_NUM  20//测试集大小
using namespace std;

struct Point
        {
    Matrix image;
    Matrix label;
    //构造函数
    Point(const char * Image, uint8_t num):image(INPUT_HEIGHT, INPUT_WIDTH, 0), label(OUTPUT_SIZE, 1)
    {
        this->image = Matrix(INPUT_HEIGHT, INPUT_WIDTH);
        for(int i = 0; i < MNISI_HEIGHT; i++)
        {
            for(int j = 0; j < MNISI_HEIGHT; j++)
            {
                this->image.mat[i+PADDING_SIZE][j+PADDING_SIZE] =(uint8_t) Image[i*MNISI_HEIGHT+j];
            }
        }
        this->label = Matrix(OUTPUT_SIZE, 1);
        label.mat[num][0] = 1;
    }
};

//把大端数据转换为我们常用的小端数据
uint32_t swap_endian(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

inline void ReadData(vector<Point>&Train, vector<Point>&Test)
{
    uint32_t magic_number;      //魔数
    uint32_t number_of_images;  //图片数量
    uint32_t number_of_labels;  //标签数量
    uint32_t number_of_rows;    //行数
    uint32_t number_of_columns; //列数

    ifstream train_image("C:\\Users\\Hyper\\CLionProjects\\LeNet-5\\Datas\\train-images.idx3-ubyte", ios::binary | ios::in );
    ifstream train_label("C:\\Users\\Hyper\\CLionProjects\\LeNet-5\\Datas\\train-labels.idx1-ubyte", ios::binary | ios::in);

    if(!train_image.is_open() || !train_label.is_open())//打开文件失败
    {
        cout<<"Open file failed!"<<endl;
        exit(0);
    }

    train_image.read((char*)&magic_number, 4);//魔数的字节数为4
    magic_number = swap_endian(magic_number);//转换为小端数据

    if(magic_number != 2051)//魔数不对
    {
        cout<<"Magic number error!"<<endl;
        exit(0);
    }

    train_label.read((char*)&magic_number, 4);//魔数的字节数为4
    magic_number = swap_endian(magic_number);//转换为小端数据

    if(magic_number != 2049)//魔数不对
    {
        cout<<"Magic number error!"<<endl;
        exit(0);
    }

    train_image.read(reinterpret_cast<char*>(&number_of_images), 4);//图片数量的字节数为4
    number_of_images = swap_endian(number_of_images);//转换为小端数据
    train_label.read(reinterpret_cast<char*>(&number_of_labels), 4);//标签数量的字节数为4
    number_of_labels = swap_endian(number_of_labels);//转换为小端数据

    //判断图片数量和标签数量是否相等
    if(number_of_images != number_of_labels)
    {
        cout<<"Number of images and labels are not equal!"<<endl;
        exit(0);
    }

    //读行数和列数
    train_image.read(reinterpret_cast<char*>(&number_of_rows), 4);
    number_of_rows = swap_endian(number_of_rows);
    train_image.read(reinterpret_cast<char*>(&number_of_columns), 4);
    number_of_columns = swap_endian(number_of_columns);

    //读取图像
    for(int i = 0; i < number_of_images; i++)
    {
        char *image = new char[number_of_rows*number_of_columns];
        train_image.read(image, number_of_rows*number_of_columns);
        char label;
        train_label.read(&label, 1);//标签的字节数为1
        Train.push_back(Point(image, label));
        delete [] image;
    }

    ifstream test_image("C:\\Users\\Hyper\\CLionProjects\\LeNet-5\\Datas\\t10k-images.idx3-ubyte", ios::binary | ios::in );
    ifstream test_label("C:\\Users\\Hyper\\CLionProjects\\LeNet-5\\Datas\\t10k-labels.idx1-ubyte", ios::binary | ios::in );

    if(!test_image.is_open() || !test_label.is_open())//打开文件失败
    {
        cout<<"Open file failed!"<<endl;
        exit(0);
    }

    test_image.read((char*)&magic_number, 4);//魔数的字节数为4
    magic_number = swap_endian(magic_number);//转换为小端数据

    if(magic_number != 2051)//魔数不对
    {
        cout<<"Magic number error!"<<endl;
        exit(0);
    }

    test_label.read((char*)&magic_number, 4);//魔数的字节数为4
    magic_number = swap_endian(magic_number);//转换为小端数据

    if(magic_number != 2049)//魔数不对
    {
        cout<<"Magic number error!"<<endl;
        exit(0);
    }

    test_image.read(reinterpret_cast<char*>(&number_of_images), 4);//图片数量的字节数为4
    number_of_images = swap_endian(number_of_images);//转换为小端数据
    test_label.read(reinterpret_cast<char*>(&number_of_labels), 4);//标签数量的字节数为4
    number_of_labels = swap_endian(number_of_labels);//转换为小端数据

    //判断图片数量和标签数量是否相等
    if(number_of_images != number_of_labels)
    {
        cout<<"Number of images and labels are not equal!"<<endl;
        exit(0);
    }

    //读行数和列数
    test_image.read(reinterpret_cast<char*>(&number_of_rows), 4);
    number_of_rows = swap_endian(number_of_rows);
    test_image.read(reinterpret_cast<char*>(&number_of_columns), 4);
    number_of_columns = swap_endian(number_of_columns);

    //读取图像
    for(int i = 0; i < number_of_images; i++)
    {
        char *image = new char[number_of_rows*number_of_columns];
        test_image.read(image, number_of_rows*number_of_columns);
        char label;
        test_label.read(&label, 1);//标签的字节数为1
        Test.push_back(Point(image, label));
        delete [] image;
    }

}

#endif //LENET_5_DATAINPUT_H

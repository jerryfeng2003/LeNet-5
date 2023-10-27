#include <iostream>
//#include "Network.h"
#include "DataInput.h"
//使用Opencv
#include <opencv2/opencv.hpp>
using namespace std;

int main()
{
    cout<<"Hello World!"<<endl;
    cout<<"This is a test for the Network!"<<endl;

    vector<Point> Train;
    vector<Point> Test;

    ReadData(Train, Test);
    //显示图片
    cv::Mat image(32, 32, CV_8UC1);

    //先转换train[i].image.mat的数据类型,从float转换为uint8_t
    for (int j = 0; j < 32; ++j) {
        for (int k = 0; k < 32; ++k) {
            image.at<uchar>(j, k) = (uint8_t)Train[0].image.mat[j][k];
        }
    }

    cv:: imshow("image", image);
    cv:: waitKey(0);
}
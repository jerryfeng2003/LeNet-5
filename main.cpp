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

    //绝对路径C:\Users\Hyper\CLionProjects\LeNet-5\Datas\t10k-images.idx3-ubyte改为相对路径
    struct Point Train(59999, "../Datas/", 0);

    //显示图片
    cv::Mat image(32, 32, CV_8UC1);

    //先转换train[i].image.mat的数据类型,从float转换为uint8_t
    for (int j = 0; j < 32; ++j) {
        for (int k = 0; k < 32; ++k) {
            image.at<uchar>(j, k) = (uint8_t)Train.image.mat[j][k];
        }
    }

    //放大图片
    cv::resize(image, image, cv::Size(256, 256), 0, 0, cv::INTER_NEAREST);

    cv:: imshow("image", image);
    cv:: waitKey(0);
}
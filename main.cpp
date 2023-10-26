#include <iostream>
#include "Matrix.h"
#include "Linear.h"
#include "DataInput.h"
//使用OpenCV库
#include <opencv2/opencv.hpp>

int main()
{
  Imagedatas imagedatas;

  imagedatas.read_image_datas(true);
//  imagedatas.read_label_datas(true);
//  imagedatas.read_image_datas(false);
//  imagedatas.read_label_datas(false);

  //显示图片
    for(int i = 0; i < 10; i++)
    {
        cv::Mat image(28, 28, CV_8UC1, imagedatas.train_data[i].mat.data());
        cv::imshow("image", image);
        cv::waitKey(0);
    }
}

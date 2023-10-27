cmake_minimum_required(VERSION 3.26)
project(LeNet_5)

set(CMAKE_CXX_STANDARD 14)

add_executable(LeNet_5
        Matrix.h
        main.cpp
        DataInput.h
        )

#下面的OpenCV是用来显示图片的
set(OpenCV_DIR E:/OpenCV/opencv/MinGW-build/install)
find_package(OpenCV REQUIRED)
target_link_libraries(LeNet_5 ${OpenCV_LIBS})
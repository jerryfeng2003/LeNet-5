#ifndef LENET_5_LINEAR_H
#define LENET_5_LINEAR_H
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "Matrix.h"

using namespace std;
class Linear
{
public:
    size_t in, hid, out;
    Matrix in_layer, hid_layer, out_layer;
    Matrix in_layer_w, hid_layer_w, out_layer_w;
    Matrix in_layer_dw, hid_layer_dw, out_layer_dw;
    Matrix in_layer_b, hid_layer_b, out_layer_b;
    Matrix in_layer_db, hid_layer_db, out_layer_db;

    Linear(size_t in_feature, size_t hid_feature, size_t out_feature) : in(in_feature), hid(hid_feature), out(out_feature),
                                                                        in_layer(1, in_feature), hid_layer(1, hid_feature), out_layer(1, out_feature),
                                                                        in_layer_w(1, in_feature, true), hid_layer_w(1, hid_feature, true), out_layer_w(1, out_feature, true),
                                                                        in_layer_dw(1, in_feature), hid_layer_dw(1, hid_feature), out_layer_dw(1, out_feature),
                                                                        in_layer_b(1, in_feature, true), hid_layer_b(1, hid_feature, true), out_layer_b(1, out_feature, true),
                                                                        in_layer_db(1, in_feature), hid_layer_db(1, hid_feature), out_layer_db(1, out_feature)
    {
    }

    void forward(const Matrix &x)
    {
        if (x.col != in)
            cout << "The input feature dosen't fit the layer!" << endl, exit(-1);
        in_layer = Relu(in_layer_w * x + in_layer_b);
        hid_layer = Relu(hid_layer_w * in_layer + hid_layer_b);
        out_layer = Softmax(out_layer_w * hid_layer + out_layer_b);
    }

    void backward(Matrix &x, const Matrix &y)
    {
        Matrix out_layer_d = out_layer - y;
        out_layer_dw = out_layer_d * hid_layer.MatrixTranspose();
        out_layer_db = out_layer_d;
        Matrix hid_layer_d = out_layer_w.MatrixTranspose() * out_layer_d;
        hid_layer_dw = hid_layer_d * in_layer.MatrixTranspose();
        hid_layer_db = hid_layer_d;
        Matrix in_layer_d = hid_layer_w.MatrixTranspose() * hid_layer_d;
        in_layer_dw = in_layer_d * x.MatrixTranspose();
        in_layer_db = in_layer_d;
    }
};

#endif
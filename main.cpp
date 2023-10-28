#include <iostream>
#include "Matrix.h"
#include "DataInput.h"
#include "Network.h"
#include "Network.cpp"

using namespace std;

int main()
{
    Point input(100,"./Datas/",0);
    input.image.resize(32,32);
    input.label.resize(10,1);
    LeNet_5 model;
    model.ForwardPropagation(input);
    model.BackPropagation(input);
    model.Update_Weight(1e-2);
    model.Clear_Gradient();
    return 0;
}
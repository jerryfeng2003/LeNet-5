#include <iostream>
#include "Matrix.h"
#include "DataInput.h"
#include "Network.h"

using namespace std;

int main()
{
    Point input;
    input.image.resize(32,32);
    input.label.resize(10,1);
    LeNet_5 model;
    // model.Network_init();
    // model.ForwardPropagation(input);
    return 0;
}
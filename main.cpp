#include <iostream>
#include "Matrix.h"
#include "DataInput.h"
#include "Network.h"
#include "Network.cpp"

using namespace std;

int main()
{
    LeNet_5 model;
    model.Train(1e-5, 10000);
    return 0;
}
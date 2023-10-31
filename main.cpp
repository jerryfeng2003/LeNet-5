#include <iostream>
#include "Matrix.h"
#include "DataInput.h"
#include "Network.h"
#include "Network.cpp"

using namespace std;

int main()
{
    LeNet_5 model;
    int epoch = 5;
    for (int i = 0; i < epoch; i++)
    {
        cout << "Epoch " << i + 1 << " start!" << endl;
        model.Train(1e-4, 10000);
        model.Test(2000);
        cout << endl;
    }
    return 0;
}
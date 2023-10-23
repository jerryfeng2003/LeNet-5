#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Mat
{
public:
    vector<vector<T>> value;
    Mat(int n, int m)
    {
        value.resize(n);
        for (auto i = value.begin(); i != value.end(); i++)
        {
            i->resize(m);
        }
    }
    void print_size()
    {
        cout << value.size() << " " << value[0].size();
    }
    friend Mat dot(const Mat &a, const Mat &b);
};

int main()
{
    Mat<float> a(5, 10);
    a.print_size();
}
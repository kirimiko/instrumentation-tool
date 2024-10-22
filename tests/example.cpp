#include <iostream>
#include <cstdlib>
using namespace std;
const int len = 1024;
int main()
{
    auto buffer = new int[len];
    for (int i = 0; i < len; ++i)
    {
        buffer[i] = rand();
    }
    double sum = 0;
    for (int i = 0; i < len; ++i)
    {
        sum += buffer[i] * rand();
    }
    cout << "sum: " << sum << endl;
    return 0;
}
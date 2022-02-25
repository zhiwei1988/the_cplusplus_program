#include <chrono>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    sleep(5);
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "sleep took " << std::chrono::duration<double, std::ratio <1,1>>(stop-start).count() << " seconds" << std::endl;
    return 0;
}
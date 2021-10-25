// Std includes
#include <iostream> // cout, endl
#include <cmath> // min
#include <map>
#include <vector>
#include <tuple>
#include <string>
// Lib includes
#include "v0l/bin/file_data.h"

int main() {
    std::cout << std::endl;
    std::cout << "INIT FILEDATA" << std::endl;
    std::size_t scalarIndex = 0;
    v0l::FileData<float> data("../data/v.vtk", scalarIndex);
    data.load({0, 1, 2});
    std::cout << "READING FILEDATA" << std::endl;
    for(std::size_t index = 0; index < 10; index++) {
        std::cout << data[index] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

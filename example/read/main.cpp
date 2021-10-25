// Std includes
#include <iostream> // cout, endl
#include <cmath> // min
#include <map>
#include <vector>
#include <tuple>
#include <string>
// Lib includes
#include "v0l/bin/read.h"

int main () {
    std::size_t scalarIndex = 0;
    std::cout << std::endl;
    std::cout << "READING METADATA" << std::endl;
    v0l::MetaData metaData = v0l::readMeta("../data/v.vtk");
    std::cout << std::endl;
    std::cout << "READING SCALARDATA" << std::endl;
    std::cout << v0l::readScalar<float>("../data/v.vtk", 0, 3, metaData) << std::endl;
    std::cout << std::endl;
    std::cout << "READING SCALARDATA VECTOR" << std::endl;
    std::vector<float> data(10);
    v0l::readScalar("../data/v.vtk", scalarIndex, data, 3, metaData);
    for(const float& value : data) {
        std::cout << value << " ";
    }
    std::cout << "READING SCALARDATA ALL" << std::endl;
    data.resize(metaData.scalarSizes[scalarIndex]);
    v0l::readScalar("../data/v.vtk", scalarIndex, data, 0, metaData);
    for(const float& value : data) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

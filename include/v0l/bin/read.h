#ifndef V0L_BIN_READ_H
#define V0L_BIN_READ_H
#pragma once

// include std
#include <string>
#include <fstream>
// include modules

namespace v0l {

// source: https://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c
template <typename T>
T swapEndian(T u)
{
    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;
    source.u = u;
    for (size_t k = 0; k < sizeof(T); k++) {
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    }
    return dest.u;
}

struct MetaData {
    // mesh data
    std::vector<std::size_t> dimensions;
    std::vector<double> origin;
    std::vector<double> spacing;
    // scalar data
    std::vector<std::size_t> scalarPosgs;
    std::vector<std::size_t> scalarSizes;
    std::vector<std::string> scalarNames;
};

MetaData readMeta(const std::string& filename, bool verbose = false) { // only works for binary data
    // init
    std::ifstream file(filename, std::ios::binary);
    std::string line;
    MetaData data;
    // mesh data
    while(getline(file, line) and (line.rfind("POINT_DATA", 0) == std::string::npos)) {
        if (line.rfind("DIMENSIONS", 0) != std::string::npos) {
            if (verbose) {
                std::cout << line << std::endl;
            }
            // // remove word
            line.erase(0, line.find(" ") + 1);
            // // extract data
            std::size_t pos = 0;
            while ((pos = line.find(" ")) != std::string::npos) {
                data.dimensions.push_back(std::stoul(line.substr(0, pos)));
                line.erase(0, pos + 1);
            }
            data.dimensions.push_back(std::stoul(line));
        }
        else if (line.rfind("ORIGIN", 0) != std::string::npos) {
            if (verbose) {
                std::cout << line << std::endl;
            }
            // // remove word
            line.erase(0, line.find(" ") + 1);
            // // extract data
            std::size_t pos = 0;
            while ((pos = line.find(" ")) != std::string::npos) {
                data.origin.push_back(std::stod(line.substr(0, pos)));
                line.erase(0, pos + 1);
            }
            data.origin.push_back(std::stod(line));
        }
        else if (line.rfind("SPACING", 0) != std::string::npos) {
            if (verbose) {
                std::cout << line << std::endl;
            }
            // // remove word
            line.erase(0, line.find(" ") + 1);
            // // extract data
            std::size_t pos = 0;
            while ((pos = line.find(" ")) != std::string::npos) {
                data.spacing.push_back(std::stod(line.substr(0, pos)));
                line.erase(0, pos + 1);
            }
            data.spacing.push_back(std::stod(line));
        }
    }
    // first scalar data
    if (verbose) {
        std::cout << line << std::endl;
    }
    // // remove word
    line.erase(0, line.find(" ") + 1);
    // // extract data
    data.scalarSizes.push_back(std::stoul(line));
    // // other first scalar data
    while(getline(file, line) and (line.rfind("LOOKUP_TABLE", 0) == std::string::npos)) {
        if(line.rfind("SCALARS", 0) != std::string::npos) {
            if (verbose) {
                std::cout << line << std::endl;
            }
            // // remove word
            line.erase(0, line.find(" ") + 1);
            // // extract data
            data.scalarNames.push_back(line.substr(0, line.find(" ")));
        }
    }
    data.scalarPosgs.push_back(file.tellg());
    file.seekg(data.scalarSizes.back() * sizeof(float), file.cur);
    // fields data
    while(getline(file, line) and (line.rfind("FIELD", 0) == std::string::npos)) {
    }
    if (verbose) {
        std::cout << line << std::endl;
    }
    // // remove words
    line.erase(0, line.find(" ") + 1); // Erase FIELD
    line.erase(0, line.find(" ") + 1); // Erase FieldData
    // // extract data
    std::size_t fieldNumber = std::stoul(line);
    for(std::size_t i = 0; i < fieldNumber; i++) {
        std::getline(file, line);
        if(verbose) {
            std::cout << line << std::endl;
        }
        std::size_t pos;
        // extract name
        pos = line.find(" ");
        data.scalarNames.push_back(line.substr(0, pos));
        line.erase(0, pos+1);
        // skip 1
        pos = line.find(" ");
        line.erase(0, pos+1);
        // extract size
        pos = line.find(" ");
        data.scalarSizes.push_back(stoul(line.substr(0, pos)));
        line.erase(0, pos+1);
        // posg and seek
        data.scalarPosgs.push_back(file.tellg());
        file.seekg(data.scalarSizes.back() * sizeof(float), file.cur);
    }
    // return
    return data;
}

template<typename TypeScalar, template<typename> typename TypeContainer>
void readScalar(const std::string& filename, const std::size_t& scalarIndex, TypeContainer<TypeScalar>& scalarContainer, const std::size_t& dataIndex = 0) {
    // read meta
    MetaData fileMetaData = readMeta(filename);
    // call core function
    readScalar(filename, scalarIndex, scalarContainer, dataIndex, fileMetaData);
}

template<typename TypeScalar, template<typename> typename TypeContainer>
void readScalar(const std::string& filename, const std::size_t& scalarIndex, TypeContainer<TypeScalar>& scalarContainer, const std::size_t& dataIndex, const MetaData& fileMetaData) {
    // init
    std::ifstream file(filename, std::ios::binary);
    file.seekg(fileMetaData.scalarPosgs[scalarIndex] + dataIndex * sizeof(TypeScalar), std::ios::beg);
    // read
    file.read((char*)(scalarContainer.data()), scalarContainer.size() * sizeof(TypeScalar));
    // swap from big endian to little endian
    for(TypeScalar& scalar : scalarContainer) {
        scalar = swapEndian(scalar);
    }
}

template<typename TypeScalar, template<typename> typename TypeContainer>
void readScalar(const std::string& filename, const std::size_t& scalarIndex, TypeContainer<TypeScalar>& scalarContainer, const TypeContainer<std::size_t>& dataIndex) {
    // read meta
    MetaData fileMetaData = readMeta(filename);
    // call core function
    readScalar(filename, scalarIndex, scalarContainer, dataIndex, fileMetaData);
}

template<typename TypeScalar, template<typename> typename TypeContainer>
void readScalar(const std::string& filename, const std::size_t& scalarIndex, TypeContainer<TypeScalar>& scalarContainer, const TypeContainer<std::size_t>& dataIndex, const MetaData& fileMetaData) {
    // init
    std::ifstream file(filename, std::ios::binary);
    // read
    for(unsigned int i = 0; i < dataIndex.size(); i++) {
        file.seekg(fileMetaData.scalarPosgs[scalarIndex] + dataIndex[i] * sizeof(TypeScalar), std::ios::beg);
        file.read((char*)(&(scalarContainer[dataIndex[i]])), sizeof(TypeScalar));
        scalarContainer[dataIndex[i]] = swapEndian(scalarContainer[dataIndex[i]]);
    }
}

template<typename TypeScalar>
TypeScalar readScalar(const std::string& filename, const std::size_t& scalarIndex, const std::size_t& dataIndex, const MetaData& fileMetaData) {
    // init
    std::ifstream file(filename, std::ios::binary);
    file.seekg(fileMetaData.scalarPosgs[scalarIndex] + dataIndex * sizeof(TypeScalar), std::ios::beg);
    // read
    TypeScalar scalar;
    file.read((char*)(&scalar), sizeof(TypeScalar));
    // swap from big endian to little endian
    return swapEndian(scalar);
}

}

#endif

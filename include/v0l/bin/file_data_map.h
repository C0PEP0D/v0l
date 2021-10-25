#ifndef V0L_BIN_FILE_DATA_H
#define V0L_BIN_FILE_DATA_H
#pragma once

// include std
#include <string>
#include <map>
#include <memory> // shared_ptr
// include modules
#include "v0l/bin/read.h"

namespace v0l {

template<typename TypeScalar>
class FileData {
    public:
        FileData(const std::string& filename, const std::size_t& index) : name(filename), meta(readMeta(filename)), scalarIndex(index), sData(std::make_shared<std::map<std::size_t, TypeScalar>>()) {
        }
        
        TypeScalar operator[](const std::size_t& index) const {
            if (sData->count(index) == 0) {
                (*sData)[index] = readScalar<TypeScalar>(name, scalarIndex, index, meta);
            }
            return (*sData)[index];
        }
    public:
        std::string name;
        MetaData meta;
        std::shared_ptr<std::map<std::size_t, TypeScalar>> sData;
        std::size_t scalarIndex;
};

}

#endif

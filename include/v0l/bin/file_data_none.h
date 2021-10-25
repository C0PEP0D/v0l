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
        FileData(const std::string& filename, const std::size_t& index) : name(filename), meta(readMeta(filename)), scalarIndex(index), sData(std::make_shared<std::vector<TypeScalar>>()) {
        }

        TypeScalar operator[](const std::size_t& index) const {
            return readScalar<TypeScalar>(name, scalarIndex, index, meta);
        }
    public:
        std::string name;
        MetaData meta;
        std::shared_ptr<std::vector<bool>> sRead;
        std::shared_ptr<std::vector<TypeScalar>> sData;
        std::size_t scalarIndex;
};

}

#endif

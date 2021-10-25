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
        FileData(const std::string& filename, const std::size_t& index) : name(filename), meta(readMeta(filename)), scalarIndex(index) {
            // compute n
            std::size_t n = 1;
            for(std::size_t d : meta.dimensions) {
                n *= d;
            }
            // declare internal data
            sData = std::make_shared<std::vector<TypeScalar>>(n);
            sRead = std::make_shared<std::vector<bool>>(n, false);
        }
        
        void load(std::vector<std::size_t> indexs) const {
            // check if indexs are already loaded
            for(int i = indexs.size() - 1; i > -1; i--) {
                if((*sRead)[indexs[i]]) {
                    indexs.erase(indexs.begin() + i);
                }
            }
            // read data
            readScalar<TypeScalar>(name, scalarIndex, *sData, indexs, meta);
            // set data as read
            for(unsigned int i = 0; i < indexs.size(); i++) {
                (*sRead)[indexs[i]] = true;
            }
        }

        TypeScalar operator[](const std::size_t& index) const {
            if (not (*sRead)[index]) {
                (*sData)[index] = readScalar<TypeScalar>(name, scalarIndex, index, meta);
                (*sRead)[index] = true;
            }
            return (*sData)[index];
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

//
// Created by csis on 4/30/20.
//

#ifndef INC_5806OSPROJECT_FILEPATHS_H
#define INC_5806OSPROJECT_FILEPATHS_H


#include <cstdint>
#include "Ext2File.h"
#include "Directories.h"

class FilePaths {
public:
    static uint32_t searchDirectory(Ext2File *ext2, uint32_t iNum, char* targetFile);
    static uint32_t traversePath(Ext2File *f, char* path);
};


#endif //INC_5806OSPROJECT_FILEPATHS_H

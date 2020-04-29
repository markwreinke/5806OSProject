//
// Created by csis on 4/27/20.
//

#ifndef INC_5806OSPROJECT_FILEACCESS_H
#define INC_5806OSPROJECT_FILEACCESS_H


#include <cstdint>
#include "Inode.h"
#include <cmath>
class FileAccess {
public:
    static int32_t fetchBlockFromFile(Ext2File *ext2, uint32_t bNum, void *buf, uint32_t iNum);
    static int32_t writeBlockToFile(Ext2File *ext2, uint32_t blockNum, void *buf, uint32_t iNum);
};


#endif //INC_5806OSPROJECT_FILEACCESS_H

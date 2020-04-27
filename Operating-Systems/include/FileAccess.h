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
    int32_t fetchBlockFromFile(Ext2File *ext2, uint32_t bNum, void *buf);
    int32_t writeBlockToFile(Inode *i, uint32_t bNum, void *buf, uint32_t iNum);
private:
    int32_t readDirect(uint32_t *blockList, uint32_t bNum, void *buf, Ext2File *ext2);
    int32_t readSingle(uint32_t *blockList, uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2);
    int32_t readDoubled(uint32_t *blockList, uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2);
};


#endif //INC_5806OSPROJECT_FILEACCESS_H

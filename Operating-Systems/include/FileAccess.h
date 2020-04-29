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

private:
    static int32_t readDirect(uint32_t *blockList, uint32_t bNum, void *buf, Ext2File *ext2);
    static int32_t readSingle(uint32_t *blockList, uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2);
    static int32_t readDouble(uint32_t *blockList, uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2);

};


#endif //INC_5806OSPROJECT_FILEACCESS_H

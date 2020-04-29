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
    static int32_t writeBlockToFile(Ext2File *ext2, uint32_t bNum, void *buf, uint32_t iNum);

private:
    static int32_t readDirect(uint32_t *blockList, uint32_t bNum, void *buf, Ext2File *ext2);
    static int32_t readSingle(uint32_t *blockList, uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2);
    static int32_t readDoubled(uint32_t *blockList, uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2);
    static int32_t writeDouble(uint32_t *blockList, int32_t bNum, uint32_t iBlockNum, int32_t numDataBlocks, Ext2File *ext2, void *buf);
    static int32_t writeSingle(uint32_t *blockList, int32_t bNum, uint32_t iBlockNum, int32_t numDataBlocks, Ext2File *ext2, void *buf);
    static int32_t writeDirect(uint32_t *blockList, int32_t bNum, uint32_t iBlockNum, Ext2File *ext2, void *buf);
};


#endif //INC_5806OSPROJECT_FILEACCESS_H

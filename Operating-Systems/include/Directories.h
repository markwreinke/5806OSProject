//
// Created by csis on 4/29/20.
//

#ifndef INC_5806OSPROJECT_DIRECTORIES_H
#define INC_5806OSPROJECT_DIRECTORIES_H

#include <cstdint>
#include "Inode.h"
#include "FileAccess.h"
struct Dirent{
    uint32_t iNum;
    uint16_t recLen;
    uint8_t nameLen;
    uint8_t fileType;
    uint8_t name[1];
};
struct Directory{
    uint32_t cursor;
    uint8_t *blockData;///an array of ext2File.returnBlockSize, holds the information of the given block
    uint32_t iNum;
    InodeStruct iS;
    Dirent* dirent;
    Ext2File *ext2;
};
class Directories {
public:
    static bool getNextDirent(struct Directory *d, uint32_t &iNum, char*name);
    static void rewindDir(struct Directory *d);
    static void closeDir(struct Directory *d);
    static struct Directory *openDirectory(Ext2File* ext2, uint32_t iNum);
};


#endif //INC_5806OSPROJECT_DIRECTORIES_H

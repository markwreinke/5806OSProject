//
// Created by csis on 4/27/20.
//

#ifndef INC_5806OSPROJECT_FILEACCESS_H
#define INC_5806OSPROJECT_FILEACCESS_H


#include <cstdint>
#include "Inode.h"

class FileAccess {
    int32_t fetchBlockFromFile(Inode *i,uint32_t bNum, void *buf);
    int32_t writeBlockToFile(Inode *i,uint32_t bNum, void* buf);

};


#endif //INC_5806OSPROJECT_FILEACCESS_H

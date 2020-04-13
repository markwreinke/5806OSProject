//
// Created by csis on 4/13/20.
//

#ifndef INC_5806OSPROJECT_INODE_H
#define INC_5806OSPROJECT_INODE_H


#include <cstdint>

class Inode {
    public:
        int32_t fetchInode(struct Ext2File *f,uint32_t inum);
        int32_t writeInode(struct Ext2File *f, uint32_t inum);
        int32_t inodeInUse(struct Ext2File *f, uint32_t inum);
        uint32_t allocateInode(struct Ext2File *f, int32_t group);
        int32_t freeInode(struct Ext2File *f, uint32_t inum);
    private:
};


#endif //INC_5806OSPROJECT_INODE_H

//
// Created by csis on 4/13/20.
//

#include "../include/Inode.h"
#include "../include/Ext2File.h"
#include "../include/StepZDebug.h"
#include <cmath>
#include <cstring>

int32_t Inode::fetchInode(struct Ext2File *f, uint32_t iNum, struct InodeStruct *buf) {
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;

    uint8_t *tmpBlock = new uint8_t[1024];

    //int inodesPerBlock = 1024 / f->superBlock.s_inode_size;
    f->fetchBlock(f->BGDT[blockGroup].bg_inode_table, tmpBlock);
    //memcpy(buf, tmpBlock + (localInodeIndex %inodesPerBlock)*f->superBlock.s_inode_size, 128);
    StepZDebug::displayBuffer(tmpBlock, 1024, 0);






    /*int numBlocksTakenByTable = (f->superBlock.s_inodes_per_group*f->superBlock.s_inode_size) / (pow(2, f->superBlock.s_log_block_size) * 1024);
    struct InodeStruct *inodes = new InodeStruct[f->superBlock.s_inodes_per_group];
    for(int i = 0; i < numBlocksTakenByTable; i++) {
        f->fetchBlock(f->BGDT[iNum - 1].bg_inode_table + i, inodes + i*f->superBlock.s_inode_size);
    }

    *buf = inodes[iNum];
    delete[] inodes;*/
}

int32_t Inode::writeInode(struct Ext2File *f, uint32_t iNum, struct InodeStruct *buf) {
    f->writeBlock(f->BGDT[iNum - 1].bg_inode_table, buf);
}

int32_t Inode::inodeInUse(struct Ext2File *f, uint32_t iNum) {

}

uint32_t Inode::allocateInode(struct Ext2File *f, int32_t group) {

}

int32_t Inode::freeInode(struct Ext2File *f, uint32_t iNum) {

}
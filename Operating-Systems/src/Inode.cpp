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
}

int32_t Inode::writeInode(struct Ext2File *f, uint32_t iNum, struct InodeStruct *buf) {
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;

    uint8_t *tmpBlock = new uint8_t[1024];

    int inodePerBlock = 1024 / f->superBlock.s_inode_size;
    int wantedBlockNumber = localInodeIndex / inodePerBlock;
    int wantedInodeIndex = localInodeIndex % inodePerBlock;
    f->fetchBlock(f->BGDT[blockGroup].bg_inode_table + wantedBlockNumber,tmpBlock);
    memcpy(tmpBlock + wantedInodeIndex*f->superBlock.s_inode_size, buf, sizeof(InodeStruct));
    f->writeBlock(f->BGDT[blockGroup].bg_inode_table + wantedBlockNumber,tmpBlock);
}

int32_t Inode::inodeInUse(struct Ext2File *f, uint32_t iNum) {

}

uint32_t Inode::allocateInode(struct Ext2File *f, int32_t group) {

}

int32_t Inode::freeInode(struct Ext2File *f, uint32_t iNum) {

}
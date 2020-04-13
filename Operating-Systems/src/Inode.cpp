//
// Created by csis on 4/13/20.
//

#include "../include/Inode.h"
#include "../include/Ext2File.h"

int32_t Inode::fetchInode(struct Ext2File *f, uint32_t iNum, struct InodeStruct *buf) {
    f->fetchBlock(f->BGDT[iNum - 1].bg_inode_table, buf);
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
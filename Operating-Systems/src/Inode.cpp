//
// Created by csis on 4/13/20.
//

#include "../include/Inode.h"
#include "../include/Ext2File.h"
#include "../include/StepZDebug.h"
#include <cmath>
#include <cstring>

int32_t Inode::fetchInode(struct Ext2File *f, uint32_t iNum, struct InodeStruct *buf) {
    uint32_t successFlag = 0;
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;


    uint8_t *tmpBlock = new uint8_t[1024];

    int inodesPerBlock = 1024 / f->superBlock.s_inode_size;
    int wantedBlock = localInodeIndex / inodesPerBlock;
    int wantedBlockIndex = localInodeIndex % inodesPerBlock;

    successFlag = f->fetchBlock(f->BGDT[blockGroup].bg_inode_table + wantedBlock, tmpBlock);
    if(successFlag != 0) {
        cout << "Fetchblock failed" << endl;
        delete[] tmpBlock;
        return successFlag;
    }

    memcpy(buf, tmpBlock + (wantedBlockIndex *f->superBlock.s_inode_size), f->superBlock.s_inode_size);

    delete[] tmpBlock;

    return successFlag;

    /*int numBlocksTakenByTable = (f->superBlock.s_inodes_per_group*f->superBlock.s_inode_size) / (pow(2, f->superBlock.s_log_block_size) * 1024);
    struct InodeStruct *inodes = new InodeStruct[f->superBlock.s_inodes_per_group];
    for(int i = 0; i < numBlocksTakenByTable; i++) {
        f->fetchBlock(f->BGDT[iNum - 1].bg_inode_table + i, inodes + i*f->superBlock.s_inode_size);
    }

    *buf = inodes[iNum];
    delete[] inodes;*/
}

int32_t Inode::writeInode(struct Ext2File *f, uint32_t iNum, struct InodeStruct *buf) {
    uint32_t successFlag = 0;
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;


    uint8_t *tmpBlock = new uint8_t[1024];

    int inodesPerBlock = 1024 / f->superBlock.s_inode_size;
    int wantedBlock = localInodeIndex / inodesPerBlock;
    int wantedBlockIndex = localInodeIndex % inodesPerBlock;

    successFlag = f->fetchBlock(f->BGDT[blockGroup].bg_inode_table + wantedBlock, tmpBlock);
    if(successFlag != 0) {
        cout << "Fetchblock failed" << endl;
        delete[] tmpBlock;
        return successFlag;
    }
    memcpy(tmpBlock + (wantedBlockIndex *f->superBlock.s_inode_size), buf, f->superBlock.s_inode_size);
    successFlag = f->writeBlock(f->BGDT[blockGroup].bg_inode_table + wantedBlock, tmpBlock);
    delete[] tmpBlock;
    return successFlag;

}

int32_t Inode::inodeInUse(struct Ext2File *f, uint32_t iNum) {
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;


    uint8_t *tmpBlock = new uint8_t[1024];

    int wantedInode = localInodeIndex / 8;
    int wantedInodeIndex = localInodeIndex % 8;
}

uint32_t Inode::allocateInode(struct Ext2File *f, int32_t group) {
 /*
  * write function to clear the inode you are writing to, and call before alloacteInode
  */


     //read descriptor table, should have field for free inodes, find one with free inodes
     //low limit = 0, high limit = number of groups - 1
     // if(group >= 0) {low limit = high limit = group}

     //for(i = low limit; i <= high limit; i++){
     // look for free inodes
     // if(
     //}

     // once you find group with free inodes, load bitmap, find one without all ones, and calculate bit number of the 0 digit

     // set bit,

     // decrement free inode count
     //write to superblock + other things

}

int32_t Inode::freeInode(struct Ext2File *f, uint32_t iNum) {

}
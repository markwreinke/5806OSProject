//
// Created by csis on 4/13/20.
//

#include "../include/Inode.h"
#include "../include/Ext2File.h"
#include "../include/StepZDebug.h"
#include <cmath>
#include <cstring>

/* Fetches a specific inode by fetching the block it is in, and grabbing the wanted inode from within it. */
int32_t Inode::fetchInode(struct Ext2File *extFile, uint32_t iNum, struct InodeStruct *buf) {
    uint32_t successFlag = 0;
    int blockGroup = (iNum - 1) / extFile->superBlock.s_inodes_per_group; // Returns the block group that the wanted inode is in
    int localInodeIndex = (iNum - 1) % extFile->superBlock.s_inodes_per_group; // Returns the index within the block group that the wanted inode is in


    uint8_t *tmpBlock = new uint8_t[extFile->getBlockSize()]; // Create a buffer the size of a block.

    int inodesPerBlock = extFile->getBlockSize() / extFile->superBlock.s_inode_size; // Get the number of inodes in a block
    int wantedBlock = localInodeIndex / inodesPerBlock; // Return what specific block within the blocks of inodes the wanted inode is in
    int wantedBlockIndex = localInodeIndex % inodesPerBlock; // Return the index of the block the wanted inode is in.

    successFlag = extFile->fetchBlock(extFile->BGDT[blockGroup].bg_inode_table + wantedBlock, tmpBlock);
    if(successFlag != 0) {
        cout << "Fetchblock failed" << endl;
        delete[] tmpBlock;
        return successFlag;
    }

    memcpy(buf, tmpBlock + (wantedBlockIndex * extFile->superBlock.s_inode_size), extFile->superBlock.s_inode_size);
    delete[] tmpBlock;

    return successFlag;


}

int32_t Inode::writeInode(struct Ext2File *f, uint32_t iNum, struct InodeStruct *buf) {
    uint32_t successFlag = 0;
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;


    uint8_t *tmpBlock = new uint8_t[f->getBlockSize()];

    int inodesPerBlock = f->getBlockSize() / f->superBlock.s_inode_size;
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

    if(inodeInUse(f,iNum) == 0){
        setInodeToUsed(f,iNum);
    }

    return successFlag;

}

int32_t Inode::inodeInUse(struct Ext2File *f, uint32_t iNum) {
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;
    int wantedInode = localInodeIndex / 8;
    int wantedInodeIndex = localInodeIndex % 8;
    int wantedBit = (8*wantedInode) + wantedInodeIndex;

    if(f->BGDT[blockGroup].bg_inode_bitmap & (1 << (wantedBit))){
        cout << "SET" << endl;
        return 1;
    }else {
        cout << "NOT SET" << endl;
        return 0;
    }
}

uint32_t Inode::allocateInode(struct Ext2File *f, int32_t group) {
 /*
  * write function to clear the inode you are writing to, and call before alloacteInode
  */
 uint8_t returningInode = -1;
 if(group >= f->getNumBlockGroups()) {
     cout << "There are not that many block groups! Desired group: " << group << " number of block groups: " << f->getNumBlockGroups() << endl;
     return returningInode;
 }
 uint8_t lowLimit = 0;
 uint8_t highLimit = f->getNumBlockGroups() - 1;

 if(group >= 0) {
     lowLimit = group;
     highLimit = group;
 }


 for(int currentGroup = lowLimit; currentGroup <= highLimit; currentGroup++) {
     if(f->BGDT[currentGroup].bg_free_inodes_count > 0) {


     }
 }
 if(returningInode < 0) {
     cout << "Failed to find a free inode! " << endl;
     return returningInode;
 }
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
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;
    int wantedInode = localInodeIndex / 8;
    int wantedInodeIndex = localInodeIndex % 8;
    int wantedBit = (8*wantedInode) + wantedInodeIndex;

    f->BGDT[blockGroup].bg_inode_bitmap &= ~(1UL << wantedBit);

    /* Clear the Inode */
    clearInode(f, iNum);

    //StepZDebug::dumpInode(f,temp,iNum);
}
void Inode::clearInode(Ext2File *f, uint32_t iNum){
    InodeStruct temp = (const struct InodeStruct){0};
    writeInode(f,iNum, &temp);
}
void Inode::setInodeToUsed(Ext2File *f, uint32_t iNum){
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group;
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;
    int wantedInode = localInodeIndex / 8;
    int wantedInodeIndex = localInodeIndex % 8;
    int wantedBit = (8*wantedInode) + wantedInodeIndex;

    f->BGDT[blockGroup].bg_inode_bitmap |= (1UL << wantedBit);

}
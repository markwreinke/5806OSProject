//
// Created by csis on 4/13/20.
//

#include "../include/Inode.h"
#include "../include/Ext2File.h"
#include "../include/StepZDebug.h"
#include <cmath>
#include <cstring>

/* Fetches a specific inode by fetching the block it is in, and grabbing the wanted inode from within it. Returns -1 if failed */
int32_t Inode::fetchInode(struct Ext2File *extFile, uint32_t iNum, struct InodeStruct *buf) {
    uint32_t successFlag = 0;
    int blockGroup = (iNum - 1) / extFile->superBlock.s_inodes_per_group; // Returns the block group that the wanted inode is in
    int localInodeIndex = (iNum - 1) % extFile->superBlock.s_inodes_per_group; // Returns the index within the block group that the wanted inode is in


    uint8_t *tmpBlock = new uint8_t[extFile->getBlockSize()]; // Create a buffer the size of a block.

    int inodesPerBlock = extFile->getBlockSize() / extFile->superBlock.s_inode_size; // Get the number of inodes in a block
    int wantedBlock = localInodeIndex / inodesPerBlock; // Return what specific block within the blocks of inodes the wanted inode is in
    int wantedBlockIndex = localInodeIndex % inodesPerBlock; // Return the index of the block the wanted inode is in.

    successFlag = extFile->fetchBlock(extFile->BGDT[blockGroup].bg_inode_table + wantedBlock, tmpBlock);

    /* If successFlag == -1, then it the fetchBlock failed */
    if(successFlag == -1) {
        cout << "fetchBlock failed" << endl;
        delete[] tmpBlock;
        return -1;
    }

    memcpy(buf, tmpBlock + (wantedBlockIndex * extFile->superBlock.s_inode_size), extFile->superBlock.s_inode_size);
    delete[] tmpBlock;

    return successFlag;


}

/* Returns -1 upon failure */
int32_t Inode::writeInode(struct Ext2File *f, uint32_t iNum, struct InodeStruct *buf) {

    int32_t successFlag = 0;//flag to catch if our fetchBlock failed
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group; //value of the blockgroup containing the given inode
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group;/// where in the blockgroup the inode is located IE is it the 10th inode in the group


    uint8_t *tmpBlock = new uint8_t[f->getBlockSize()];

    ///find the wanted block for the inode and which inode in the block is needed
    int inodesPerBlock = f->getBlockSize() / f->superBlock.s_inode_size;
    int wantedBlock = localInodeIndex / inodesPerBlock;
    int wantedBlockIndex = localInodeIndex % inodesPerBlock;
    /// fetch the block and store it into a temp block, check to make sure it didn't fail
    successFlag = f->fetchBlock(f->BGDT[blockGroup].bg_inode_table + wantedBlock, tmpBlock);
    if(successFlag == -1) {
        cout << "Fetchblock failed" << endl;
        delete[] tmpBlock;
        return -1;
    }
    //it didnt fail so copyFileToHost over the new inode into the location in memory where the original inode was located
    memcpy(tmpBlock + (wantedBlockIndex *f->superBlock.s_inode_size), buf, f->superBlock.s_inode_size);
    ///rewrite the given inode in the BGDT
    successFlag = f->writeBlock(f->BGDT[blockGroup].bg_inode_table + wantedBlock, tmpBlock);

    ///delete the extra stuff
    delete[] tmpBlock;

    return successFlag;

}

int32_t Inode::inodeInUse(struct Ext2File *f, uint32_t iNum) {
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group; // Returns the block group that the wanted inode is in
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group; // Returns the index within the block group that the wanted inode is in
    int inodesPerBlock = f->getBlockSize() / f->superBlock.s_inode_size; // Get the number of inodes in a block
    int wantedBlock = localInodeIndex / inodesPerBlock; // Return what specific block within the blocks of inodes the wanted inode is in
    int wantedByte = localInodeIndex / 8;///return the specific byte wanted
    int wantedBit = localInodeIndex % 8;///return the specific bit wanted



    ///store the neededblock to a temp block because im big brain
    uint8_t *tmpBlock = new uint8_t[f->getBlockSize()];
    f->fetchBlock(f->BGDT[blockGroup].bg_inode_bitmap + wantedBlock, tmpBlock);

    ///ill store the block in a temp block cause im big brain
    if(tmpBlock[wantedByte] & (1 << (wantedBit))){
        ///cout << "SET" << endl;
        return 1;
    }else{
        //cout << "NOT SET" << endl;
        return 0;
    }
}

/* Allocate a new Inode, return its number or -1 if failed */
uint32_t Inode::allocateInode(struct Ext2File *f, int32_t group) {
 /*
  * write function to clear the inode you are writing to, and call before allocateInode
  */
 uint8_t returningInode = -1;
 if(group >= (int32_t)f->getNumBlockGroups()) {
     cout << "There are not that many block groups! Desired group: " << group << " number of block groups: " << f->getNumBlockGroups() << endl;
     return returningInode;
 }
 uint8_t lowLimit = 0;
 uint8_t highLimit = f->getNumBlockGroups() - 1;

 if(group >= 0) {
     lowLimit = group;
     highLimit = group;
 }

 /* First we must find a block group that has a free inode, which is the first one found, or if we are given a wanted group, it will check that */
 for(int currentGroup = lowLimit; currentGroup <= highLimit; currentGroup++) {
     /* If the BG we are looking at has a free inode */
     if(f->BGDT[currentGroup].bg_free_inodes_count > 0) {
         uint8_t *tmpBlock = new uint8_t[f->getBlockSize()]; // Create a temp block to pull to.

         uint32_t bitMapLocation = f->BGDT[currentGroup].bg_inode_bitmap; // The location of the bitmap
         uint32_t successFlag = -1; // A flag to ensure fetch/write operations don't fail
         successFlag = f->fetchBlock(bitMapLocation, tmpBlock); // Fetch the bitmap for the current block
         if(successFlag < 0) { // If the fetch fails.
             cout << "FetchBlock failed." << endl;
             return successFlag;
         }

         /* Iterates through all uint8_ts of the bit map looking for the first one !=0xff, then returns the location of the first free inode */
         for(int currentBitChunk = 0; currentBitChunk < f->getBlockSize()/sizeof(uint8_t); currentBitChunk++){
             /* If the looked at uint8_t of bitmap isn't full */
             if(tmpBlock[currentBitChunk] != 0xff) {
                 /* Iterate through bits of the looked at byte, finding the first 0(empty inode) */
                 for(int i = 0; i < 8; i++) {
                     if(!((tmpBlock[currentBitChunk] >> i) & 1U)) {
                         returningInode = i + currentBitChunk*8 + currentGroup*f->superBlock.s_inodes_per_group + 1; // Calculate the inode that is free
                         tmpBlock[currentBitChunk] ^= 1UL << i; // Toggle the bit from open to used
                         successFlag = f->writeBlock(bitMapLocation, tmpBlock); // write the block
                         delete[] tmpBlock;
                         if(successFlag < 0) { // If the write block fails.
                             cout << "WriteBlock Failed." << endl;
                             return successFlag;
                         }
                         /* Set all indices to their max, leaving all loops (Since we successfully allocated a inode)*/
                         i = 8;
                         currentBitChunk = f->getBlockSize()/sizeof(uint8_t);
                         currentGroup = f->getNumBlockGroups();
                     }
                 }
             }
         }
     }
 }

 /* If we didn't find a free inode, we return error message, else we decrement the free inodes in the BGDT and superblock */
 if(returningInode < 0) {
     cout << "Did not find a free inode" << endl;
 } else {
     f->BGDT[(returningInode - 1)/f->superBlock.s_inodes_per_group].bg_free_inodes_count--;
     f->superBlock.s_free_inodes_count--;
 }
 /* Update BGDTs and SuperBlocks */
 f->writeAllBGDT(f->BGDT);
 f->writeAllSuperBlocks(&f->superBlock);
 return returningInode;
}

int32_t Inode::freeInode(struct Ext2File *f, uint32_t iNum) {
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group; // Returns the block group that the wanted inode is in
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group; // Returns the index within the block group that the wanted inode is in
    int inodesPerBlock = f->getBlockSize() / f->superBlock.s_inode_size; // Get the number of inodes in a block
    int wantedBlock = localInodeIndex / inodesPerBlock; // Return what specific block within the blocks of inodes the wanted inode is in
    int wantedByte = localInodeIndex / 8;///return the wanted byte
    int wantedBit = localInodeIndex % 8;////return the wanted bit



    ///set tmpBlock to the block we wanted
    uint8_t *tmpBlock = new uint8_t[f->getBlockSize()];
    f->fetchBlock(f->BGDT[blockGroup].bg_inode_bitmap + wantedBlock, tmpBlock);

    ///change the bit in the wanted byte to 1
    tmpBlock[wantedByte] &= ~(1UL << wantedBit);

    ///rewrite the tempBlock
    f->writeBlock(f->BGDT[blockGroup].bg_inode_bitmap + wantedBlock, tmpBlock);

    /* Clear the Inode */
    clearInode(f, iNum);

    ///Increment the number of free inodes available
    f->BGDT[blockGroup].bg_free_inodes_count++;
    f->superBlock.s_free_inodes_count++;
    ///rewrite the BGDT and superblock over all copies
    f->writeAllBGDT(f->BGDT);
    f->writeAllSuperBlocks(&f->superBlock);
}

///Sets all values in the inode to 0
void Inode::clearInode(Ext2File *f, uint32_t iNum){
    InodeStruct temp = (const struct InodeStruct){0};
    writeInode(f,iNum, &temp);
}

void Inode::setInodeToUsed(Ext2File *f, uint32_t iNum){
    int blockGroup = (iNum - 1) / f->superBlock.s_inodes_per_group; // Returns the block group that the wanted inode is in
    int localInodeIndex = (iNum - 1) % f->superBlock.s_inodes_per_group; // Returns the index within the block group that the wanted inode is in
    int inodesPerBlock = f->getBlockSize() / f->superBlock.s_inode_size; // Get the number of inodes in a block
    int wantedBlock = localInodeIndex / inodesPerBlock; // Return what specific block within the blocks of inodes the wanted inode is in
    int wantedByte = localInodeIndex / 8;// return the wanted byte
    int wantedBit = localInodeIndex % 8;// return the wanted bit



    ///set tmpBlock to the block we wanted
    uint8_t *tmpBlock = new uint8_t[f->getBlockSize()];
    f->fetchBlock(f->BGDT[blockGroup].bg_inode_bitmap + wantedBlock, tmpBlock);

    ///change the bit in the wanted byte to 1
    tmpBlock[wantedByte] |= (1UL << wantedBit);

    ///rewrite the tempBlock
    f->writeBlock(f->BGDT[blockGroup].bg_inode_bitmap+ wantedBlock, tmpBlock);
    ///decrement the number of free inodes available
    f->BGDT[blockGroup].bg_free_inodes_count--;
    f->superBlock.s_free_inodes_count--;
    ///rewrite the BGDT and superblock over all copies
    f->writeAllBGDT(f->BGDT);
    f->writeAllSuperBlocks(&f->superBlock);
}
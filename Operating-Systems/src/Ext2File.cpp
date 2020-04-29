//
// Created by csis on 3/23/20.
//

#include <cmath>
#include "../include/Ext2File.h"
#include "../include/VDIFile.h"
#include "../include/Partition.h"

Ext2File::Ext2File() {
    vdiFile = new VDIFile();
    partition = new Partition();
}

/* Open the file with the given file name, populating an ext2file structure, consisting of the superblock, block blockGroup descriptor table, and calculating the block size and the number of block groups. */
bool Ext2File::ext2Open(char *fn) {
    /* Open the VDI file with the given filename, storing if the file open was successful or not in a boolean */
    vdiFile->vdiOpen(fn);
    bool PartitionOpenSuccess = partition->partitionOpen(vdiFile);

    /* Filling the super block */
    fetchSuperBlock((uint32_t) 0, &superBlock);

    /* Calculate block size and the number of block groups */
    this->blockSize = 1024*pow(2,superBlock.s_log_block_size);
    this->numBlockGroups = superBlock.s_blocks_count / superBlock.s_blocks_per_group + 1;

    /* Filling the block blockGroup descriptor table */
    this->BGDT = new BlockGroupDescriptor [numBlockGroups];
    fetchBGDT(1,BGDT);

    return PartitionOpenSuccess;

}

/* Close the VDI file, and delete dynamically allocated memory */
void Ext2File::ext2Close() {
    partition->partitionClose();
    vdiFile->vdiClose();
    delete[] partition;
    delete[] vdiFile;
    delete[] BGDT;
}

///assumes given a valid blockNum, will read superblock into structure
int32_t Ext2File::fetchSuperBlock(uint32_t blockNum, struct SuperBlock *sb){
    if(blockNum == 0) {
        partition->partitionSeek(1024, SEEK_SET);
        partition->partitionRead(sb, 1024);
    }
    else {
        partition->partitionSeek(blockNum*blockSize, SEEK_SET);
        partition->partitionRead(sb, 1024);
    }

    if(sb->s_magic != 0xef53) {
        return -1;
    }
    return 0;
}

///assumes given a valid blockNum will write superblock
int32_t Ext2File::writeSuperBlock(uint32_t blockNum, struct SuperBlock *sb) {
    int32_t successCheck = -1;
    if(blockNum == 0) {
        partition->partitionSeek(1024, SEEK_SET);
        successCheck = partition->partitionWrite(sb, 1024);
    }
    else {
        partition->partitionSeek(blockNum*blockSize, SEEK_SET);
        successCheck = partition->partitionWrite(sb, 1024);
    }
    return successCheck;
}

uint32_t Ext2File::fetchBlock(uint32_t blockNum, void *buf) {

    partition->partitionSeek(blockSize*blockNum, SEEK_SET);
    int blocksRead = partition->partitionRead(buf, blockSize);

    if(blocksRead != blockSize) {
        return -1;
    }
    return 0;
}

uint32_t Ext2File::writeBlock(uint32_t blockNum, void *buf){
    partition->partitionSeek(blockSize*blockNum, SEEK_SET);
    int BlocksWritten = partition->partitionWrite(buf, blockSize);

    if(BlocksWritten != blockSize){
        return -1;
    }
    return 0;
}
///assumes given a valid blockNum that it will fill the BGDT

int32_t Ext2File::fetchBGDT(uint32_t blockNum, struct BlockGroupDescriptor *bgdt){
    int bootAccount;
    if(blockSize == 1024) {
        bootAccount = 1024;
    } else {
        bootAccount = 0;
    }

    partition->partitionSeek(blockSize*blockNum + bootAccount, SEEK_SET);
    return partition->partitionRead(bgdt, 32*numBlockGroups);
}

///assumes given a valid blockNum
int32_t Ext2File::writeBGDT(uint32_t blockNum, struct BlockGroupDescriptor * bgdt){
    int bootAccount;
    if(blockSize == 1024){
        bootAccount == 2048;
    }else{bootAccount == blockSize;}


    ssize_t Test;
    if(blockNum == 0) {
        partition->partitionSeek(bootAccount, SEEK_SET);
        Test = partition->partitionWrite(bgdt, 32*numBlockGroups);
    }else{
        partition->partitionSeek(blockSize*blockNum, SEEK_SET);
        Test = partition->partitionWrite(bgdt, 32*numBlockGroups);
    }
    if(Test < 0)
        {return -1;}
    return 0;
}

/* Write to all super blocks */
int32_t Ext2File::writeAllSuperBlocks(struct SuperBlock *superBlock) {
    int32_t successCheck = -1;
    for(int i = 0; i < numBlockGroups; i++) {
        if(Ext2File::containsCopyOfSuperBlockOrBGDT(i)){
            successCheck = writeSuperBlock(i * superBlock->s_blocks_per_group, superBlock);

            if(successCheck == -1) {
                cout << "A write superBlock failed i = " << i << endl;
                return -1;
            }
        }
    }

    return successCheck;
}

/* Writes to all block blockGroup descriptor table */
int32_t Ext2File::writeAllBGDT(struct BlockGroupDescriptor *bgdt) {
    int32_t successCheck = -1;
    for(int i = 0; i < numBlockGroups; i++) {
        if(Ext2File::containsCopyOfSuperBlockOrBGDT(i)){
            successCheck = writeBGDT((i * superBlock.s_blocks_per_group) + 1, bgdt);

            if(successCheck == -1) {
                cout << "A writeBGDT failed i = " << i << endl;
                return -1;
            }
        }
    }

    return successCheck;
}

/* Checks if the given block blockGroup contains a superblock or BGDT copy */
bool Ext2File::containsCopyOfSuperBlockOrBGDT(uint32_t blockGroupNumber) {
    if(blockGroupNumber == 0 || blockGroupNumber == 1) {
        return true;
    }

    /* Check if the block blockGroup is a power of 3 */
    double checkMultiples = log((double) blockGroupNumber) / log((double) 3);
    double threeCheck = checkMultiples - (int) checkMultiples;

    /* Check if the block blockGroup is a power of 5 */
    checkMultiples = log((double) blockGroupNumber) / log((double) 5);
    double fiveCheck = checkMultiples - (int) checkMultiples;


    /* Check if the block blockGroup is a power of 7 */
    checkMultiples = log((double) blockGroupNumber) / log((double) 7);
    double sevenCheck = checkMultiples - (int) checkMultiples;

    if(threeCheck == 0 || fiveCheck == 0 || sevenCheck == 0) {
        return true;
    } else {
        return false;
    }
}

/* Allocates a block if it hasn't been. If you want a specific blockGroup, pass its number in, else have no input */
uint32_t Ext2File::allocateBlock(int32_t blockGroup) {
    uint8_t returningBlock = -1;
    if(blockGroup >= getNumBlockGroups()) {
        cout << "There are not that many block groups! Desired blockGroup: " << blockGroup << " number of block groups: " << getNumBlockGroups() << endl;
        return returningBlock;
    }
    uint8_t lowLimit = 0;
    uint8_t highLimit = getNumBlockGroups() - 1;

    if(blockGroup >= 0) {
        lowLimit = blockGroup;
        highLimit = blockGroup;
    }

    /* First we must find a block blockGroup that has a free block, which is the first one found, or if we are given a wanted blockGroup, it will check that */
    for(int currentGroup = lowLimit; currentGroup <= highLimit; currentGroup++) {
        /* If the BG we are looking at has a free block */
        if(BGDT[currentGroup].bg_free_blocks_count > 0) {
            uint8_t *tmpBlock = new uint8_t[getBlockSize()]; // Create a temp block to pull to.

            uint32_t bitMapLocation = BGDT[currentGroup].bg_block_bitmap; // The location of the bitmap
            uint32_t successFlag = -1; // A flag to ensure fetch/write operations don't fail
            successFlag = fetchBlock(bitMapLocation, tmpBlock); // Fetch the bitmap for the current block
            if(successFlag < 0) { // If the fetch fails.
                cout << "FetchBlock failed." << endl;
                return successFlag;
            }

            /* Iterates through all uint8_ts of the bit map looking for the first one !=0xff, then returns the location of the first free inode */
            for(int currentBitChunk = 0; currentBitChunk < getBlockSize()/sizeof(uint8_t); currentBitChunk++){
                /* If the looked at uint8_t of bitmap isn't full */
                if(tmpBlock[currentBitChunk] != 0xff) {
                    /* Iterate through bits of the looked at byte, finding the first 0(empty inode) */
                    for(int i = 0; i < 8; i++) {
                        if(!((tmpBlock[currentBitChunk] >> i) & 1U)) {
                            returningBlock = i + currentBitChunk * 8 + currentGroup * superBlock.s_blocks_per_group; // Calculate the block that is free
                            tmpBlock[currentBitChunk] ^= 1UL << i; // Toggle the bit from open to used
                            successFlag = writeBlock(bitMapLocation, tmpBlock); // write the block
                            delete[] tmpBlock;
                            if(successFlag < 0) { // If the write block fails.
                                cout << "WriteBlock Failed." << endl;
                                return successFlag;
                            }
                            /* Set all indices to their max, leaving all loops (Since we successfully allocated a block)*/
                            i = 8;
                            currentBitChunk = getBlockSize()/sizeof(uint8_t);
                            currentGroup = getNumBlockGroups();
                        }
                    }
                }
            }
        }
    }

    /* If we didn't find a free block, we return error message, else we decrement the free blocks in the BGDT and superblock */
    if(returningBlock < 0) {
        cout << "Did not find a free inode" << endl;

        /* If it was given a blockgroup, and the group is full, then it will search for a free block in any group */
        if(blockGroup != -1) {
            returningBlock = allocateBlock();
        }
    } else {
        BGDT[(returningBlock - 1) / superBlock.s_blocks_per_group].bg_free_blocks_count--;
        superBlock.s_free_blocks_count--;

        /* Write a block of 0s to the allocated block to remove any nonsense data */
        uint8_t *tmpBlock = new uint8_t[getBlockSize()]{0};
        writeBlock(returningBlock, tmpBlock);
        delete[] tmpBlock;
    }
    /* Update BGDTs and SuperBlocks */
    writeAllBGDT(BGDT);
    writeAllSuperBlocks(&superBlock);
    return returningBlock;
}
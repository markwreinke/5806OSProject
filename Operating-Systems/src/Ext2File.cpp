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

bool Ext2File::ext2Open(char *fn) {
    ///make sure filename is correct;
    vdiFile->vdiOpen(fn);
    bool PartitionOpenSuccess = partition->partitionOpen(vdiFile);


    ///populate superBlock
    fetchSuperBlock((uint32_t) 0, &superBlock);
    ///populate other values
    this->blockSize = 1024*pow(2,superBlock.s_log_block_size);
    this->numBlockGroups = superBlock.s_blocks_count / superBlock.s_blocks_per_group + 1;
    cout << numBlockGroups << endl;
    this->BGDT = new BlockGroupDescriptor [numBlockGroups];
    fetchBGDT(1,BGDT);


    return PartitionOpenSuccess;

}
void Ext2File::ext2Close() {
    ///delete anything created dynamically todo

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
    if(blockNum == 0) {
        partition->partitionSeek(1024, SEEK_SET);
        partition->partitionWrite(sb, 1024);
    }
    else {
        partition->partitionSeek(blockNum*blockSize, SEEK_SET);
        partition->partitionWrite(sb, 1024);
    }
}

uint32_t Ext2File::fetchBlock(uint32_t blockNum, void *buf) {

    partition->partitionSeek(blockSize*blockNum, SEEK_SET);
    int BlocksRead = partition->partitionRead(buf, blockSize);

    if(BlocksRead != blockSize) {
        return -1;
    }
    return 0;
}

uint32_t Ext2File::writeBlock(uint32_t blockNum, void *buf){
    partition->partitionSeek(blockSize*blockNum, SEEK_SET);
    int BlocksWritten = partition->partitionWrite(buf, blockSize);
    return 0;
}
///assumes given a valid blockNum that it will fill the BGDT
int32_t Ext2File::fetchBGDT(uint32_t blockNum, struct BlockGroupDescriptor *bgdt){
    int bootAccount;
    if(blockSize == 1024){
        bootAccount = 1024;
    }else{bootAccount = 0;}
        partition->partitionSeek(blockSize*blockNum + bootAccount, SEEK_SET);
        return partition->partitionRead(bgdt, 32*numBlockGroups);
}
///assumes given a valid blockNum
int32_t Ext2File::writeBGDT(uint32_t blockNum,struct BlockGroupDescriptor * bgdt){
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
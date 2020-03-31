//
// Created by csis on 3/23/20.
//

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


    ///populate superBlock todo
    fetchSuperBlock((uint32_t) 0, &superBlock);
    ///populate block structure todo

    ///populate other values todo



    return PartitionOpenSuccess;

}
void Ext2File::ext2Close() {
    ///delete anything created dynamically todo

    partition->partitionClose();
    vdiFile->vdiClose();
    delete[] partition;
    delete[] vdiFile;
}

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
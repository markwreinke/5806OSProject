//
// Created by csis on 3/23/20.
//

#include "../include/Ext2File.h"
#include "../include/VDIFile.h"
#include "../include/Partition.h"

Ext2File::Ext2File() {
}

bool Ext2File::ext2Open(char *fn) {
    /// It might be the Java dev in me that noticed that we never instantiated vdiFile or partition...is this necessary in C++, or would the space for it be created when Ext2File is created ?
   vdiFile = new VDIFile();
   partition = new Partition();
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
        //todo fetchblock
    }

    if(sb->s_magic != 0xef53) {
        return -1;
    }
    return 0;
}

uint32_t Ext2File::fetchBlock(uint32_t blockNum, void *buf) {
    int RealLocation;
    if(blockSize == 1024) {
        RealLocation = blockSize*blockNum + 1024;
    }
    else {
        RealLocation = blockSize*blockNum;
    }
    partition->partitionSeek(RealLocation, SEEK_SET);
    int BlocksRead = partition->partitionRead(buf, blockSize);

    if(BlocksRead != blockSize) {
        return -1;
    }
    return 0;
}
uint32_t Ext2File::writeBlock(uint32_t blockNum, void *buf){
    int RealLocation;
    if(blockSize == 1024){
        RealLocation = blockSize*blockNum + 1024;
    }
    else{
        RealLocation = blockSize*blockNum;
    }
    partition->partitionSeek(RealLocation, SEEK_SET);

    int blocksWritten;
    if(sizeof(buf) >= blockSize) {
        blocksWritten = partition->partitionWrite(buf, blockSize);
        if(blocksWritten != blockSize)
            return -1;
    }
    else {
        blocksWritten = partition->partitionWrite(buf, sizeof(buf));
        if(blocksWritten != sizeof(buf))
            return -1;
    }
    return 0;
}
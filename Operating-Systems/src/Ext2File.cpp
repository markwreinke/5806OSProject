//
// Created by csis on 3/23/20.
//

#include "../include/Ext2File.h"
#include "../include/VDIFile.h"
#include "../include/Partition.h"
bool Ext2File::ext2Open(char *fn) {
    ///make sure filename is correct;
    vdiFile->vdiOpen(fn);
    bool PartitionOpenSuccess = Partition->partitionOpen(vdiFile);
    ///populate superblock todo

    ///populate blockstrucutre todo

    ///populate other values todo



    return PartitionOpenSuccess;

}
void Ext2File::ext2Close(){
    ///delete anything created dynamically todo

    Partition->partitionClose();
}

int32_t Ext2File::fetchSuperBlock(uint32_t blockNum, struct Superblock *sb){
    if(blockNum == 0){
        Partition->partitionSeek(1024,SEEK_SET);
        Partition->partitionRead(sb,1024);
    }
    else{
        //todo fetchblock
    }
    if(sb.s_magic != 0xef53)
        return -1;
    return 0;
}

uint32_t Ext2File::fetchBlock(uint32_t blockNum, void *buf) {
    int RealLocation;
    if(blockSize == 1024){
        RealLocation = blocksize*blockNum + 1024;
    }
    else{
        RealLocation = blockSize*blockNum;
    }
    Partition->partitionSeek(RealLocation,SEEK_SET);
    int BlocksRead = Partition->partitionRead(buf,blockSize);
    if(BlocksRead != blockSize)
        return -1;
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
    Partition->partitionSeek(RealLocation,SEEK_SET);

    int BlocksWritten;
    if(sizeof(buf) >= blockSize){
        BlocksWritten = Partition.partitionWrite(buf,blockSize);
        if(BlocksWritten != blockSize)
            return -1;
    }
    else{
        BlocksWritten = Partition->partitionWrite(buf, sizeof(buf));
        if(BlocksWritten != sizeof(buf))
            return -1;
    }
    return 0;
}
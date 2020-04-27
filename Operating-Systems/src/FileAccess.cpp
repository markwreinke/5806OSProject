//
// Created by csis on 4/27/20.
//

#include "../include/FileAccess.h"
int32_t FileAccess::fetchBlockFromFile(Ext2File *ext2,uint32_t bNum, void *buf){
    int32_t numDataBlocks =  ext2->getBlockSize() / 4;
    InodeStruct *iS = new InodeStruct;
    Inode::fetchInode(ext2,0,iS);
    uint32_t *blockList;
    int32_t flag = 0;

    if(bNum < 12) {
        blockList = new uint32_t[11];
        flag = readDirect(blockList, bNum, buf, ext2);
    }else if(bNum < 12 + numDataBlocks){
        if(iS->i_block[12] == 0)
            return -1;

        ext2->fetchBlock(iS->i_block[12],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12;
        flag = readDirect(blockList, bNum, buf, ext2);
    }else if(bNum < 12 + numDataBlocks + pow(numDataBlocks,2)){
        if(iS->i_block[13] == 0)
            return -1;

        ext2->fetchBlock(iS->i_block[13],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12 - numDataBlocks;
        flag = readSingle(blockList, bNum, buf, numDataBlocks, ext2);

    } else{
        if(iS->i_block[14] == 0)
            return -1;

        ext2->fetchBlock(iS->i_block[14],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12 - numDataBlocks - pow(numDataBlocks,2);
        flag = readDoubled(blockList, bNum, buf, numDataBlocks, ext2);
    }
    delete[] blockList;
    delete iS;
    return flag;
}
int32_t FileAccess::writeBlockToFile(Ext2File *ext2,uint32_t bNum, void* buf, uint32_t iNum){
    uint32_t numDataBlock = ext2->getBlockSize() / 4;
    uint32_t tempBuffer[numDataBlock];
    InodeStruct *iS = new InodeStruct;
    Inode::fetchInode(ext2,0,iS);
    uint32_t *blockList;
    int32_t flag;
    if(bNum < 12){
        if(iS->i_block[bNum] == 0) {
            iS->i_block[bNum] = ext2->allocateBlock(-1);
            Inode::writeInode(ext2, iNum, iS);
        }

        blockList = new uint32_t[11];
        blockList = iS->i_block;
        flag = writeDirect();

    }else if(bNum < 12 + numDataBlock){
        if(iS->i_block[12] == 0) {
            iS->i_block[12] = ext2->allocateBlock(-1);
            Inode::writeInode(ext2, iNum, iS);
        }

        ext2->fetchBlock(iS->i_block[12],tempBuffer);
        uint32_t iBlockNum = iS->i_block[12];
        blockList = new uint32_t[numDataBlock];
        blockList = tempBuffer;
        bNum = bNum - 12;
        flag = writeDirect();

    }else if(bNum < 12 + numDataBlock + pow(numDataBlock,2)){
        if(iS->i_block[13] == 0) {
            iS->i_block[13] = ext2->allocateBlock(-1);
            Inode::writeInode(ext2, iNum, iS);
        }
        ext2->fetchBlock(iS->i_block[13],tempBuffer);

        uint32_t iBlockNum = iS->i_block[12];
        blockList = new uint32_t [numDataBlock];
        blockList = tempBuffer;
        bNum = bNum - 12 - numDataBlock
    }else{
        if(iS->i_block[14] == 0) {
            iS->i_block[14] = ext2->allocateBlock(-1);
            Inode::writeInode(ext2, iNum, iS);
        }
        delete[] blockList;
        delete iS;
        return flag;
    }
}
int32_t FileAccess::readDirect(uint32_t blockList[], uint32_t bNum, void* buf, Ext2File *ext2){

if(blockList[bNum] == 0)
    return -1;

uint32_t flag = ext2->fetchBlock(blockList[bNum],buf);

if(flag == -1)
        buf = {0};
return 1;

}
int32_t FileAccess::readSingle(uint32_t blockList[], uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2){
    uint32_t index = bNum / numDataBlocks;
    bNum = bNum % numDataBlocks;
    if(blockList[index] == 0)
        return -1;
    uint32_t flag =ext2->fetchBlock(blockList[index],buf);
    if(flag == -1)
        buf = {0};
    blockList = (uint32_t*)buf;
    return readDirect(blockList, bNum, buf);
}
int32_t FileAccess::readDoubled(uint32_t blockList[], uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2){
    uint32_t index = bNum/ pow(numDataBlocks,2);
    bNum = bNum % pow(numDataBlocks,2);
    if(blockList[index] == 0)
        return -1;
    uint32_t flag = ext2->fetchBlock(blockList[index],buf);
    if(flag == -1)
        buf = {0};

    blockList = (uint32_t*)buf;
    return readSingle(blockList, bNum, buf, numDataBlocks, ext2);
}
//
// Created by csis on 4/27/20.
//

#include "../include/FileAccess.h"
int32_t FileAccess::fetchBlockFromFile(Ext2File *ext2,uint32_t bNum, void *buf){
    int32_t numDataBlocks =  ext2->getBlockSize() / 4;
    InodeStruct *iS = new InodeStruct;
    uint32_t *blockList;

    if(bNum < 12) {
        blockList = new uint32_t[11];
        return direct(blockList,bNum,buf,ext2);
    }else if(bNum < 12 + numDataBlocks){
        if(iS->i_block[12] == 0)
            return -1;

        ext2->fetchBlock(iS->i_block[12],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12;
        return direct(blockList,bNum,buf,ext2);
    }else if(bNum < 12 + numDataBlocks + pow(numDataBlocks,2)){
        if(iS->i_block[13] == 0)
            return -1;

        ext2->fetchBlock(iS->i_block[13],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12 - numDataBlocks;
        return single(blockList,bNum,buf,numDataBlocks,ext2);

    } else{
        if(iS->i_block[14] == 0)
            return -1;

        ext2->fetchBlock(iS->i_block[14],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12 - numDataBlocks - pow(numDataBlocks,2);
        return doubled(blockList,bNum,buf,numDataBlocks,ext2);
    }
}
int32_t FileAccess::writeBlockToFile(Inode *i,uint32_t bNum, void* buf, uint32_t iNum){

}
int32_t FileAccess::direct(uint32_t blockList[], uint32_t bNum,void* buf,Ext2File *ext2){

if(blockList[bNum] == 0)
    return -1;

uint32_t flag = ext2->fetchBlock(blockList[bNum],buf);

if(flag == -1)
        buf = {0};
return 1;

}
int32_t FileAccess::single(uint32_t blockList[], uint32_t bNum,void *buf, int32_t numDataBlocks, Ext2File *ext2){
    uint32_t index = bNum / numDataBlocks;
    bNum = bNum % numDataBlocks;
    if(blockList[index] == 0)
        return -1;
    uint32_t flag =ext2->fetchBlock(blockList[index],buf);
    if(flag == -1)
        buf = {0};
    blockList = (uint32_t*)buf;
    return direct(blockList,bNum,buf);
}
int32_t FileAccess::doubled(uint32_t blockList[], uint32_t bNum,void *buf, int32_t numDataBlocks, Ext2File *ext2){
    uint32_t index = bNum/ pow(numDataBlocks,2);
    bNum = bNum % pow(numDataBlocks,2);
    if(blockList[index] == 0)
        return -1;
    uint32_t flag = ext2->fetchBlock(blockList[index],buf);
    if(flag == -1)
        buf = {0};

    blockList = (uint32_t*)buf;
    return single(blockList,bNum,buf,numDataBlocks,ext2);
}
//
// Created by csis on 4/27/20.
//

#include "../include/FileAccess.h"
int32_t FileAccess::fetchBlockFromFile(Ext2File *ext2, uint32_t bNum, void *buf, uint32_t iNum){
    int32_t numDataBlocks =  ext2->getBlockSize() / 4;
    InodeStruct *iS = new InodeStruct; //hold the iblock information
    Inode::fetchInode(ext2,iNum,iS);///fills the inodestruct
    uint32_t *blockList;///contains an array of uint32_t addresses that point to different blocks
    int32_t flag = 0;///flag for returning the values from our reads after deleting anything created dynamically

    if(bNum < 12) {
        blockList = new uint32_t[11];
        flag = readDirect(blockList, bNum, buf, ext2);
    }else if(bNum < 12 + numDataBlocks){
        /// if the block isnt allocated theres nothing to fetch so return -1
        if(iS->i_block[12] == 0)
            return -1;
        //otherwuise fetch the block into the buffer, update the blocklist with the given buffer, and then update the bNum to account fo skipped nodes.
        ext2->fetchBlock(iS->i_block[12],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12;
        flag = readDirect(blockList, bNum, buf, ext2);
    }else if(bNum < 12 + numDataBlocks + pow(numDataBlocks,2)){
        ///if the block isnt allocated theres nothing to fetch so return -1
        if(iS->i_block[13] == 0)
            return -1;
        //otherwuise fetch the block into the buffer, update the blocklist with the given buffer, and then update the bNum to account fo skipped nodes.
        ext2->fetchBlock(iS->i_block[13],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12 - numDataBlocks;
        flag = readSingle(blockList, bNum, buf, numDataBlocks, ext2);

    } else{
        ///if this isnt allocated return -1
        if(iS->i_block[14] == 0)
            return -1;
        //otherwuise fetch the block into the buffer, update the blocklist with the given buffer, and then update the bNum to account fo skipped nodes.
        ext2->fetchBlock(iS->i_block[14],buf);
        blockList = new uint32_t[numDataBlocks];
        blockList = (uint32_t*)buf;
        bNum = bNum - 12 - numDataBlocks - pow(numDataBlocks,2);
        flag = readDoubled(blockList, bNum, buf, numDataBlocks, ext2);
    }
    ///delete anything created dynamically and then return our value of -1 or 1
    delete[] blockList;
    delete iS;
    return flag;
}

int32_t FileAccess::writeBlockToFile(Ext2File *ext2,uint32_t bNum, void* buf, uint32_t iNum){
    uint32_t numDataBlock = ext2->getBlockSize() / 4;
    uint32_t tempBuffer[numDataBlock];///temp block to prevent the overwritting of data
    InodeStruct *iS = new InodeStruct;///holds i_block information
    Inode::fetchInode(ext2,iNum,iS);///fills the InodeStruct that was just created with the iNum given
    uint32_t *blockList;///A dynamically sized array of uin32_t that contains the addresses to different blocks
    int32_t flag; /// flag to return our Writes after deleteing anything created dynamically

    /* Figure out what blockGroup to write to */
    uint32_t activeBlockGroup = iS->i_block[0]/ext2->superBlock.s_blocks_per_group;


    if(bNum < 12){
        ///if the block given isnt allocated allocate it and then write to it
        if(iS->i_block[bNum] == 0) {
            iS->i_block[bNum] = ext2->allocateBlock(activeBlockGroup);
            Inode::writeInode(ext2, iNum, iS);
        }
        //Set the blocklist equal to i_block, write direct has bNum included twice because in this case bNum is also the iblocknumber
        blockList = new uint32_t[11];
        blockList = iS->i_block;
        flag = writeDirect(blockList,bNum,bNum,ext2,buf);

    }else if(bNum < 12 + numDataBlock){
        ///if the block given isnt allocated allocate it and then write to it
        if(iS->i_block[12] == 0) {
            iS->i_block[12] = ext2->allocateBlock(activeBlockGroup);
            Inode::writeInode(ext2, iNum, iS);
        }
        ///Fetch the block into a temp buffer and then store the temp buffer into a blocklist the size of a block
        ///update bNum for how many nodes we skipped, set flag equal to the value from our write
        ext2->fetchBlock(iS->i_block[12],tempBuffer);
        uint32_t iBlockNum = iS->i_block[12];
        blockList = new uint32_t[numDataBlock];
        blockList = tempBuffer;
        bNum = bNum - 12;
        flag = writeDirect(blockList,bNum,iBlockNum,ext2,buf);

    }else if(bNum < 12 + numDataBlock + pow(numDataBlock,2)){
        ///if the block given isnt allocated allocate it and then write to it
        if(iS->i_block[13] == 0) {
            iS->i_block[13] = ext2->allocateBlock(activeBlockGroup);
            Inode::writeInode(ext2, iNum, iS);
        }
        ///Fetch the block into a temp buffer and then store the temp buffer into a blocklist the size of a block
        ///update bNum for how many nodes we skipped, set flag equal to the value from our write
        ext2->fetchBlock(iS->i_block[13],tempBuffer);

        uint32_t iBlockNum = iS->i_block[13];
        blockList = new uint32_t [numDataBlock];
        blockList = tempBuffer;
        bNum = bNum - 12 - numDataBlock;
        flag = writeSingle(blockList,bNum,iBlockNum,numDataBlock,ext2,buf);
    }else{
        ///if the block given isnt allocated allocate it and then write to it
        if(iS->i_block[14] == 0) {
            iS->i_block[14] = ext2->allocateBlock(activeBlockGroup);
            Inode::writeInode(ext2, iNum, iS);
        }
        ///Fetch the block into a temp buffer and then store the temp buffer into a blocklist the size of a block
        ///update bNum for how many nodes we skipped, set flag equal to the value from our write
        ext2->fetchBlock(iS->i_block[14],tempBuffer);
        uint32_t iBlockNum = iS->i_block[14];
        blockList = new uint32_t[numDataBlock];
        blockList = tempBuffer;
        bNum = bNum - 12 - numDataBlock - pow(numDataBlock,2);
       flag = writeDouble(blockList,bNum,iBlockNum,numDataBlock,ext2,buf);
    }
    ///delete anything dynamically created and then return our flag
    delete[] blockList;
    delete iS;
    return flag;
}

int32_t FileAccess::readDirect(uint32_t blockList[], uint32_t bNum, void* buf, Ext2File *ext2){
///if the block list is not allocated return -1 because we cant read it
if(blockList[bNum] == 0)
    return -1;
//set our flag equal to the value from fetch block
uint32_t flag = ext2->fetchBlock(blockList[bNum],buf);
///if flag == -1 then there was an error fetching, set the buf to 0
if(flag == -1)
        buf = {0};
return 1;
}
int32_t FileAccess::readSingle(uint32_t blockList[], uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2){
    uint32_t index = bNum / numDataBlocks;///the actual index of the block withing our blockList
    bNum = bNum % numDataBlocks; // the updated blockNumber if bNum was greater than the numDataBlocks
    ///if the block at index is unallocated return -1
    if(blockList[index] == 0)
        return -1;
    ///set a flag equal to the value from fetching a block into the buffer pointed to by blocklist[index]
    uint32_t flag =ext2->fetchBlock(blockList[index],buf);
    //if flag == -1 fetchblock failed, set buf = 0
    if(flag == -1)
        buf = {0};
    ///set blocklist equal to the buffer, we must cast the buffer as a uint32_t*
    blockList = (uint32_t*)buf;
    return readDirect(blockList, bNum, buf,ext2);
}
int32_t FileAccess::readDoubled(uint32_t blockList[], uint32_t bNum, void *buf, int32_t numDataBlocks, Ext2File *ext2){
    uint32_t index = bNum/ pow(numDataBlocks,2); ///the actual index of the block withing our blockList
    bNum = bNum % (uint32_t)pow(numDataBlocks,2); // the updated blockNumber if bNum was greater than the numDataBlocks
    ///if the block at index is unallocated return -1
    if(blockList[index] == 0)
        return -1;
    ///set a flag equal to the value from fetching a block into the buffer pointed to by blocklist[index]
    uint32_t flag = ext2->fetchBlock(blockList[index],buf);
    //if flag == -1 fetchblock failed, set buf = 0
    if(flag == -1)
        buf = {0};
    ///set blocklist equal to the buffer, we must cast the buffer as a uint32_t*
    blockList = (uint32_t*)buf;
    return readSingle(blockList, bNum, buf, numDataBlocks, ext2);
}
int32_t FileAccess::writeDirect(uint32_t blockList[], int32_t bNum, uint32_t iBlockNum, Ext2File *ext2, void* buf){
    /* Figure out what blockGroup to write to */
    uint32_t activeBlockGroup = blockList[0]/ext2->superBlock.s_blocks_per_group;
    ///if the block given is unallocated, allocate it and then write blockList to it
    if(blockList[bNum] == 0){
        blockList[bNum] = ext2->allocateBlock(activeBlockGroup);
        ext2->writeBlock(iBlockNum,blockList);
    }
    ///write the given buffer to the block pointed to by the blockList
    return ext2->writeBlock(blockList[bNum],buf);
}
int32_t FileAccess::writeSingle(uint32_t blockList[], int32_t bNum, uint32_t iBlockNum, int32_t numDataBlocks, Ext2File *ext2, void* buf){
    /* Figure out what blockGroup to write to */
    uint32_t activeBlockGroup = blockList[0]/ext2->superBlock.s_blocks_per_group;

    int32_t index = bNum / numDataBlocks;///the actual index of the block withing our blockList
    bNum = bNum % numDataBlocks;// the updated blockNumber if bNum was greater than the numDataBlocks
    uint32_t tempBuffer[numDataBlocks];///a temp buffer to hold the information the size of a block

    ///if the block is unallocated, allocated it and write the blockList to it
    if(blockList[index] == 0){
        blockList[index] = ext2->allocateBlock(activeBlockGroup);
        ext2->writeBlock(iBlockNum,blockList);
    }
    /// update information for the passing to writeDirect
    iBlockNum = blockList[index];
    ext2->fetchBlock(blockList[index],tempBuffer);
    ///setBlocklist = tempbuffer now that we can overwrite it
    blockList = tempBuffer;

    return writeDirect(blockList,bNum,iBlockNum,ext2,buf);
}
int32_t FileAccess::writeDouble(uint32_t blockList[], int32_t bNum, uint32_t iBlockNum, int32_t numDataBlocks, Ext2File *ext2, void* buf){
    /* Figure out what blockGroup to write to */
    uint32_t activeBlockGroup = blockList[0]/ext2->superBlock.s_blocks_per_group;

    int32_t index = bNum / pow(numDataBlocks,2);
    bNum = bNum % (uint32_t)pow(numDataBlocks,2);
    uint32_t tempBuffer[numDataBlocks];

    ///if the block is unallocated, allocated it and write the blockList to it
    if(blockList[index] == 0){
        blockList[index] = ext2->allocateBlock(activeBlockGroup);
        ext2->writeBlock(iBlockNum,blockList);
    }
    /// update information for the passing to writeDirect
    iBlockNum = blockList[index];
    //fetch the block from the address given by blockList[Index] into tempBuffer, cant store directly into blockList because then were overwriting what were trying to read
    ext2->fetchBlock(blockList[index],tempBuffer);
    ///now set the blockList = tempbuffer
    blockList = tempBuffer;
    return writeSingle(blockList,bNum,iBlockNum,numDataBlocks,ext2,buf);
}

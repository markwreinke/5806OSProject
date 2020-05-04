//
// Created by csis on 4/27/20.
//

#include "../include/FileAccess.h"

/* Returns -1 upon failure */
int32_t FileAccess::fetchBlockFromFile(Ext2File *ext2, uint32_t bNum, void *buf, uint32_t iNum) {

    /* Get the k value (number of children each indirect node has) indirect node has pointers to k data nodes,
     * each doubly indirect node has pointers to k indirect nodes, and each triply indirect node has pointers to
     * k doubly indirect nodes */
    int32_t kValue = ext2->getBlockSize() / 4;

    /* Create an inode structure to hold the information for the file being written to, fetching the inode into it. */
    InodeStruct *tempInodeStruct = new InodeStruct;///holds i_block information
    Inode::fetchInode(ext2, iNum, tempInodeStruct);///fills the InodeStruct that was just created with the iNum given

    uint32_t *blockList;///A dynamically sized array of uin32_t that contains the addresses to different blocks

    int32_t flag = -1; /// flag to return our Writes after deleting anything created dynamically
    uint32_t index;
    uint32_t dBIindex;

    /** If the block number is directly stored in i_block (i_block[0] through i_block[11]) **/
    if (bNum < 12) {
        blockList = tempInodeStruct->i_block;
        goto direct;
    }

        /** If the block number is stored in the indirect block (i_block[12]) **/
    else if (bNum < 12 + kValue) {
        /* If the SIB is not allocated, then bNum certainly isn't */
        if (tempInodeStruct->i_block[12] == 0) {
            flag = -1;
            goto ending;
        }

        /* Fetch the SIB into the buffer */
        ext2->fetchBlock(tempInodeStruct->i_block[12], buf);

        /* Set the blockList to point to the buffer (Buffer holds the SIB) */
        blockList = (uint32_t *) buf;

        /* Adjust the value of bNum to be an index within blockList */
        bNum = bNum - 12;

        goto direct;
    }

        /** If the block number is stored in the doubly indirect block (i_block[13]) **/
    else if (bNum < 12 + kValue + pow(kValue, 2)) {
        /* If the DIB is not allocated, then bNum certainly isn't */
        if (tempInodeStruct->i_block[13] == 0) {
            flag = -1;
            goto ending;
        }

        /* Fetch the DIB into the buffer */
        ext2->fetchBlock(tempInodeStruct->i_block[13], buf);

        /* Set the blockList to point to the buffer (Buffer holds the DIB) */
        blockList = (uint32_t *) buf;

        /* Adjust the value of bNum to be an index within blockList */
        bNum = bNum - 12 - kValue;

        goto singleIndirect;

    }

        /** If the block number is stored in the triply indirect block (i_block[14]) **/
    else {
        /* If the TIB is not allocated, then bNum certainly isn't */
        if (tempInodeStruct->i_block[14] == 0) {
            flag = -1;
            goto ending;
        }

        /* Fetch the TIB into the buffer */
        ext2->fetchBlock(tempInodeStruct->i_block[14], buf);

        /* Set the blockList to point to the buffer (Buffer holds the TIB) */
        blockList = (uint32_t *) buf;

        /* Adjust the value of bNum to be an index within blockList */
        bNum = bNum - 12 - kValue - pow(kValue, 2);

        goto doubleIndirect;
    }
    doubleIndirect:
        dBIindex = bNum / pow(kValue, 2); ///the actual index of the block withing our blockList
        bNum = bNum % (uint32_t) pow(kValue, 2); // the updated blockNumber if bNum was greater than the numDataBlocks

        /* If the DIB is not allocated, then bNum certainly isn't */
        if (blockList[dBIindex] == 0) {
            flag = -1;
            goto ending;
        }

        /* Fetch the DIB into the buffer */
        flag = ext2->fetchBlock(blockList[dBIindex],buf);

        /* Set the blockList to point to the buffer (Buffer holds an IB) */
        blockList = (uint32_t*)buf;

    singleIndirect:
        index = bNum / kValue;///the actual index of the block withing our blockList
        bNum = bNum % kValue; // the updated blockNumber if bNum was greater than the kValue

        /* If the blockList is not allocated, then return false */
        if (blockList[index] == 0) {
            flag = -1;
            goto ending;
        }

        /* Fetch the SIB full of data blocks */
        flag = ext2->fetchBlock(blockList[index], buf);

        /* Set the blockList to point to the buffer (Buffer holds the DIB) */
        blockList = (uint32_t*)buf;


    direct:
        /* If the blockList is not allocated, then return false */
        if(blockList[bNum] == 0) {
            flag = -1;
            goto ending;
        }

        //set our flag equal to the value from fetchBlock
        flag = ext2->fetchBlock(blockList[bNum],buf);


    ending:
        /* If we failed a read, set buffer to 0 */
        if(flag == -1) {
            buf = {0};
        }

        //delete tempInodeStruct;
        return flag;
}

/* Returns -1 if fails */
int32_t FileAccess::writeBlockToFile(Ext2File *ext2, uint32_t blockNum, void* buf, uint32_t iNum) {

    /* Get the k value (number of children each indirect node has) indirect node has pointers to k data nodes,
     * each doubly indirect node has pointers to k indirect nodes, and each triply indirect node has pointers to
     * k doubly indirect nodes */
    uint32_t kValue = ext2->getBlockSize() / 4;


    uint32_t *tempBuffer = new uint32_t[kValue];//temp block to prevent the overwriting of data

    /* Create an inode structure to hold the information for the file being written to, fetching the inode into it. */
    InodeStruct *tempInodeStruct = new InodeStruct;///holds i_block information
    Inode::fetchInode(ext2, iNum, tempInodeStruct);///fills the InodeStruct that was just created with the iNum given


    uint32_t *blockList;///A dynamically sized array of uin32_t that contains the addresses to different blocks
    int32_t flag; /// flag to return our Writes after deleting anything created dynamically


    /* Variables for other things */
    int32_t iBlockNum = -1;
    int32_t index;
    int32_t dIBindex;

    /** If the block number is directly stored in i_block (i_block[0] through i_block[11]) **/
    if(blockNum < 12) {
        /* If the wanted block isn't allocated, then do so, updating the inode. */
        if(tempInodeStruct->i_block[blockNum] == 0) {
            /* Figure out what blockGroup to write to */
            uint32_t activeBlockGroup = (iNum - 1) / ext2->superBlock.s_inodes_per_group; // Returns the block group that the  inode is in

            tempInodeStruct->i_block[blockNum] = ext2->allocateBlock(activeBlockGroup);

            /* Update the number of data blocks in inode */
            tempInodeStruct->i_blocks++;
            Inode::writeInode(ext2, iNum, tempInodeStruct);
        }
        //Set the blockList equal to i_block, write direct has blockNum included twice because in this case blockNum is also the iBlockNumber
        blockList = tempInodeStruct->i_block;
        goto direct;

    }
    /** If the block number is stored in the indirect block (i_block[12]) **/
    else if(blockNum < 12 + kValue) {
        /* If the indirect block isn't allocated, then do so */
        if(tempInodeStruct->i_block[12] == 0) {
            /* Figure out what blockGroup to write to */
            uint32_t activeBlockGroup = (iNum - 1) / ext2->superBlock.s_inodes_per_group; // Returns the block group that the  inode is in
            tempInodeStruct->i_block[12] = ext2->allocateBlock(activeBlockGroup); // allocateBlock returns the block number of the new allocated block
            Inode::writeInode(ext2, iNum, tempInodeStruct);
        }
        ///Fetch the block into a temp buffer and then store the temp buffer into a blocklist the size of a block
        ///update blockNum for how many nodes we skipped, set flag equal to the value from our write

        /* Fetch the indirect block into a tempBuffer */
        ext2->fetchBlock(tempInodeStruct->i_block[12], tempBuffer);

        /* Holds onto the block number of the indirect block */
        iBlockNum = tempInodeStruct->i_block[12];

        /* Set blockList to point to the tempBuffer (since tempBuffer holds data blocks) */
        blockList = tempBuffer;

        /* Adjust the value of blockNum to be an index within blockList */
        blockNum = blockNum - 12;

        goto direct;

    }
    /** If the block number is stored in the doubly indirect block (i_block[13]) **/
    else if(blockNum < 12 + kValue + pow(kValue, 2)) {
        /* If the DIB is not allocated, then do so */
        if(tempInodeStruct->i_block[13] == 0) {
            /* Figure out what blockGroup to write to */
            uint32_t activeBlockGroup = (iNum - 1) / ext2->superBlock.s_inodes_per_group; // Returns the block group that the  inode is in
            tempInodeStruct->i_block[13] = ext2->allocateBlock(activeBlockGroup); // allocateBlock returns the block number of the new allocated block
            Inode::writeInode(ext2, iNum, tempInodeStruct);
        }
        ///Fetch the block into a temp buffer and then store the temp buffer into a blocklist the size of a block
        ///update blockNum for how many nodes we skipped, set flag equal to the value from our write

        /* Fetch the DIB into the tempBuffer */
        ext2->fetchBlock(tempInodeStruct->i_block[13], tempBuffer);

        /* Holds onto the block number of the DIB */
        iBlockNum = tempInodeStruct->i_block[13];

        /* Set blockList to point to the tempBuffer (tempBuffer holds indirectBlocks) */
        blockList = tempBuffer;

        /* Adjust the value of blockNum to be an index within the indirect block */
        blockNum = blockNum - 12 - kValue;

        goto singleIndirect;
    }
    /** If the block number is stored in the triply indirect block (i_block[14]) **/
    else {
        /* If the TIB isn't allocated, then do so */
        if(tempInodeStruct->i_block[14] == 0) {
            /* Figure out what blockGroup to write to */
            uint32_t activeBlockGroup = (iNum - 1) / ext2->superBlock.s_inodes_per_group; // Returns the block group that the inode is in
            tempInodeStruct->i_block[14] = ext2->allocateBlock(activeBlockGroup);
            Inode::writeInode(ext2, iNum, tempInodeStruct);
        }
        ///Fetch the block into a temp buffer and then store the temp buffer into a blocklist the size of a block
        ///update blockNum for how many nodes we skipped, set flag equal to the value from our write

        /* Fetch the block of DIBs into the tempBuffer */
        ext2->fetchBlock(tempInodeStruct->i_block[14], tempBuffer);

        /* Holds onto the block number of the TIB */
        iBlockNum = tempInodeStruct->i_block[14];

        /* Set blockList to point to the tempBuffer (tempBuffer holds DIB) */
        blockList = tempBuffer;

        /* Adjust the value of the blockNum to be an index within the DIB */
        blockNum = blockNum - 12 - kValue - pow(kValue, 2);

        goto doubleIndirect;
    }


    /* Gotos */
    doubleIndirect:
        /* Determine which DIB to fetch */
        dIBindex = blockNum / pow(kValue, 2);

        /*Determine which block under the DIB we want */
        blockNum = blockNum % (uint32_t)pow(kValue, 2);

        /* If the DIB is not allocated, then do so */
        if(blockList[dIBindex] == 0){
            /* Figure out what blockGroup to write to */
            uint32_t activeBlockGroup = (iNum - 1) / ext2->superBlock.s_inodes_per_group; // Returns the block group that the inode is in
            blockList[dIBindex] = ext2->allocateBlock(activeBlockGroup);
            ext2->writeBlock(iBlockNum,blockList);
        }
       /* Hold onto DIB blockNum we fetched */

        iBlockNum = blockList[dIBindex];
        /* Fetch the block from the address given by blockList[Index] into tempBuffer,
         * cant store directly into blockList because then were overwriting what were trying to read */
        ext2->fetchBlock(blockList[dIBindex],tempBuffer);

        /* Set blockList to point to the tempBuffer (tempBuffer holds SIBs) */
        blockList = tempBuffer;

    singleIndirect:

        /* The index of the SIB to fetch */
        index = blockNum / kValue;///the actual index of the block withing our blockList

        /* Determine which block under the SIB we want */
        blockNum = blockNum % kValue;// the updated blockNumber if blockNum was greater than the kValue


        /* If the SIB is not allocated, then do so */
        if(blockList[index] == 0){
            /* Figure out what blockGroup to write to */
            uint32_t activeBlockGroup = (iNum - 1) / ext2->superBlock.s_inodes_per_group; // Returns the block group that the  inode is in
            blockList[index] = ext2->allocateBlock(activeBlockGroup);
            ext2->writeBlock(iBlockNum, blockList);
        }

        /* Hold onto the SIB blockNum we fetched */
        iBlockNum = blockList[index];

        /* Fetch the SIB we want, storing it into tempBuffer */
        ext2->fetchBlock(blockList[index],tempBuffer);

        /* Set blockList to point to the tempBuffer (tempBuffer holds dataBlocks) */
        blockList = tempBuffer;

    direct:
        /* If the block is not allocated, then allocate it (Should only be entered from a single indirect block call) */
        if(blockList[blockNum] == 0){
            /* Figure out what blockGroup to write to */
            uint32_t activeBlockGroup = (iNum - 1) / ext2->superBlock.s_inodes_per_group; // Returns the block group that the inode is in

            blockList[blockNum] = ext2->allocateBlock(activeBlockGroup);

            /* Write the single indirect block back to the disk, because we had to allocate a data block */
            ext2->writeBlock(iBlockNum, blockList);

            /* Updates the number of data blocks in the inode */
            tempInodeStruct->i_blocks++;
            Inode::writeInode(ext2, iNum, tempInodeStruct);

        }

        ///write the given buffer to the block pointed to by the blockList
        flag = ext2->writeBlock(blockList[blockNum], buf);


    ///delete anything dynamically created and then return our flag
    delete[] tempBuffer;
    delete tempInodeStruct;
    return flag;
}




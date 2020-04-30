//
// Created by csis on 4/30/20.
//


#include <fcntl.h>
#include "../include/copyFile.h"

static ssize_t copy(char* vdiName, char* src, char* dest){

    Ext2File vdiFile;

    /* Open vdi file, returns -1 if fails */
    if(!vdiFile.ext2Open(vdiName)){
        return -1;
    }

    /* Search for file within the vdiFile, if it is not found we return -1 */
    uint32_t srcInode = FilePaths::traversePath(&vdiFile, src);
    if(srcInode == 0) {
        vdiFile.ext2Close();
        return -1;
    }

    /* Open or create the destination file. Return -1 if failed */
    ssize_t destFD = open(dest, O_WRONLY | O_CREAT, 0666);
    if(destFD < 0) {
        vdiFile.ext2Close();
        return -1;
    }

    /* Fetch the inode structure */
    InodeStruct *inodeStruct = new InodeStruct;
    Inode::fetchInode(&vdiFile, srcInode, inodeStruct);

    /* Temp block to fetch to */
    uint8_t *tmpBlock = new uint8_t[vdiFile.getBlockSize()];

    /* Loop through all data blocks of the file, copying to destination file. Return -1 if failure */
    for(int i = 0; i < inodeStruct->i_blocks; i++) {
        FileAccess::fetchBlockFromFile(&vdiFile, i, tmpBlock, srcInode);
        int32_t writeFlag = write(destFD, tmpBlock, vdiFile.getBlockSize());
        if(writeFlag < 0) {
            delete[] tmpBlock;
            delete inodeStruct;
            vdiFile.ext2Close();
            return -1;
        }
    }

    /* Delete dynamic memory, return 1 for success */
    delete[] tmpBlock;
    delete inodeStruct;
    vdiFile.ext2Close();
    return 1;
}
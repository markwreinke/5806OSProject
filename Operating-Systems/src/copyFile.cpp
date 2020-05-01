//
// Created by csis on 4/30/20.
//


#include <fcntl.h>
#include <cstring>
#include "../include/copyFile.h"

ssize_t copyFile::copy(char* vdiName, char* src, char* dest){

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
void copyFile::viewVDIDirectories(char* filename) {
    ///open the ext2file with the filename given above
    Ext2File *ext2File = new Ext2File();
    ext2File->ext2Open(filename);
    uint32_t numInodes = ext2File->superBlock.s_inodes_count;

    char name[256];
    uint32_t iNum;
    Directory *d;
    uint32_t currentInode = 1;
    char* dot = ".";
    char* dotdot = "..";

    while (currentInode < numInodes) {
        iNum = currentInode;
        if(Inode::inodeInUse(ext2File,iNum)) {

            d = Directories::openDirectory(ext2File, currentInode);

            while (Directories::getNextDirent(d, iNum, name)) {
                if(iNum < numInodes && strcmp(dot, name) != 0 && strcmp(dotdot, name) != 0)
                    cout << "Inode: " << iNum << "     name: [" << name << ']' << endl;

            }

            Directories::closeDir(d);

        }
        currentInode++;
    }
}

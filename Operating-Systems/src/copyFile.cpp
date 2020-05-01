//
// Created by csis on 4/30/20.
//


#include <fcntl.h>
#include <cstring>
#include "../include/copyFile.h"

ssize_t copyFile::copyFileToHost(char* vdiName, char* src, char* dest){

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


// todo need to figure out how to fill the inode? As well as what directory to add it to
ssize_t copyFile::copyFileToVDI(char* vdiName, char* src, char* dest) {

    Ext2File vdiFile;

    /* Open vdi file, returns -1 if fails */
    if(!vdiFile.ext2Open(vdiName)){
        return -1;
    }

    /* Allocate an inode, returning -1 if fails */
    uint32_t destInode = Inode::allocateInode(&vdiFile, -1);
    if(destInode == -1) {
        vdiFile.ext2Close();
        return -1;
    }
    int sourceFD = open(src,O_RDONLY);
    int numBytesInFile = lseek(sourceFD,0,SEEK_END);
    InodeStruct *tempInode = new InodeStruct;
    Inode::fetchInode(&vdiFile,2,tempInode);

    InodeStruct *inodeStruct = new InodeStruct;
    Inode::fetchInode(&vdiFile, destInode, inodeStruct);
    inodeStruct->i_ctime = time(NULL);
    inodeStruct->i_atime = time(NULL);
    inodeStruct->i_mtime = time(NULL);
    inodeStruct->i_blocks = (numBytesInFile + 511) / 512;
    inodeStruct->i_links_count = 1;
    inodeStruct->i_file_acl = tempInode->i_file_acl;

    Inode::writeInode(&vdiFile, destInode, inodeStruct);
    delete tempInode;

    /* Open the src file. Return -1 if failed */
    ssize_t srcFD = open(src, O_RDONLY, 0666);
    if(srcFD < 0) {
        vdiFile.ext2Close();
        return -1;
    }

    /* Temp block to fetch to */
    uint8_t *tmpBlock = new uint8_t[vdiFile.getBlockSize()];

    /* Loop through all data blocks of the file, copying to destination file. Return -1 if failure */
    for(int i = 0; i < inodeStruct->i_blocks; i++) {
        int32_t readFlag = read(srcFD, tmpBlock, vdiFile.getBlockSize());
        if(readFlag < 0) {
            delete[] tmpBlock;
            delete inodeStruct;
            vdiFile.ext2Close();
            return -1;
        }
        Directory *d = new Directory;
        Directories::openDirectory(&vdiFile,2);
        Dirent *UpdateDirent = new Dirent;
        uint32_t tempInum;
        char* tempName = new char;
        while(Directories::getNextDirent(d,tempInum,tempName)){}
        int blockNum = d->cursor / d->ext2->getBlockSize();
        int index = d->cursor % d->ext2->getBlockSize();
        FileAccess::fetchBlockFromFile(d->ext2,blockNum,d->blockData,d->iNum);
        for(int x = 0; x < UpdateDirent->recLen;x++) {
            if (x == 0) {
                d->blockData[index + x] = UpdateDirent->iNum;
                x += 3;
            }else if(x == 4) {
                d->blockData[index + x] = UpdateDirent->recLen;
                x++;
            }else if(x== 6){
                d->blockData[index + x] = UpdateDirent->nameLen;
            }else if(x == 7){
                d->blockData[index + x] = UpdateDirent->fileType;
            }else if(x <= x + UpdateDirent->nameLen) {
                d->blockData[index + x] = UpdateDirent->name[x - 8];
            }else
                d->blockData[index + x] = 0x0;
        }
        FileAccess::writeBlockToFile(&vdiFile,blockNum,d->blockData,d->iNum);
        FileAccess::writeBlockToFile(&vdiFile, i, tmpBlock, destInode);
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
                if(iNum < numInodes && strcmp(dot, name) != 0 && strcmp(dotdot, name) != 0) {

                    cout << "Inode: " << iNum << "     name: [" << name << ']' << endl;
                }

            }

            Directories::closeDir(d);

        }
        currentInode++;
    }
}
char* copyFile::traverseGivenFileName(char* fileName){
    uint32_t end = strlen(fileName);
    uint32_t start = end - 1;
    char* finalName = new char;
        //while the path hasn't had a terminator increase end until it does, then set that symbol to 0;
        while(fileName[start] != 0 && fileName[start]  != '/'){
            start--;
        }

        string temp = (string)fileName;
        temp = temp.substr(start + 1,end-start);
        strcpy(finalName,temp.c_str());

    return finalName;
};

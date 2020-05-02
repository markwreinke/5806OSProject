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
    ssize_t destFD = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(destFD < 0) {
        vdiFile.ext2Close();
        return -1;
    }

    /* Fetch the inode structure */
    InodeStruct *inodeStruct = new InodeStruct;
    Inode::fetchInode(&vdiFile, srcInode, inodeStruct);

    /* Temp block to fetch to */
    uint8_t *tmpBlock = new uint8_t[vdiFile.getBlockSize()];


    int counter = inodeStruct->i_size;
    int bNum = 0;
    /* Loop through all data blocks of the file, copying to destination file. Return -1 if failure */
    while(counter > 0) {


        int32_t writeFlag;
        FileAccess::fetchBlockFromFile(&vdiFile, bNum++, tmpBlock, srcInode);
        if(counter > vdiFile.getBlockSize()) {
            writeFlag = write(destFD, tmpBlock, vdiFile.getBlockSize());
        } else {
            writeFlag = write(destFD, tmpBlock, counter);
        }
        if(writeFlag < 0) {
            delete[] tmpBlock;
            delete inodeStruct;
            vdiFile.ext2Close();
            return -1;
        } else {
            counter -= vdiFile.getBlockSize();
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

    /* A variable to open the ext2File to be manipulated */
    Ext2File vdiFile;

    /* Open vdi file, returns -1 if fails */
    if(!vdiFile.ext2Open(vdiName)){
        cout << "Opening the vdiFile failed." << endl;
        return -1;
    }

    /* Allocate an inode for the new file, returning -1 if fails */
    uint32_t destInode = Inode::allocateInode(&vdiFile, -1);
    if(destInode == -1) {
        vdiFile.ext2Close();
        return -1;
    }

    /* Open the src file. Return -1 if failed */
    ssize_t srcFD = open(src, O_RDONLY, 0666);
    if(srcFD < 0) {
        vdiFile.ext2Close();
        return -1;
    }
    /* Figure out the number of bytes in the file */
    int numBytesInFile = lseek(srcFD,0,SEEK_END);

    /* Return the cursor to the beginning */
    lseek(srcFD, 0, SEEK_SET);

    /* Create an newFileInode to store the inode of the root directory */
    InodeStruct *rootInode = new InodeStruct;
    int32_t fetchFlag = Inode::fetchInode(&vdiFile, 2, rootInode);
    if(fetchFlag == -1) {
        cout << "Fetching the root inode failed." << endl;
        delete rootInode;
        close(srcFD);
        vdiFile.ext2Close();
        return -1;
    }

    /* Create an newFileInode to place into the newFile inode */
    InodeStruct *newFileInode = new InodeStruct;
    fetchFlag = Inode::fetchInode(&vdiFile, destInode, newFileInode);
    /* If failure */
    if(fetchFlag == -1) {
        cout << "Fetching the new file inode failed." << endl;
        delete newFileInode;
        delete rootInode;
        close(srcFD);
        vdiFile.ext2Close();
        return -1;
    }

    /* Place necessary information in the new file inode*/
    newFileInode->i_ctime = time(NULL);
    newFileInode->i_atime = time(NULL);
    newFileInode->i_mtime = time(NULL);
    newFileInode->i_blocks = (numBytesInFile + 511) / 512;
    newFileInode->i_links_count = 1;
    newFileInode->i_file_acl = rootInode->i_file_acl;
    newFileInode->i_mode = 0100644;

    /* Since we don't need rootInode anymore, we delete it */
    delete rootInode;

    /* Write the actual inode */
    fetchFlag = Inode::writeInode(&vdiFile, destInode, newFileInode);
    if(fetchFlag == -1) {
        cout << "Writing the new file inode failed." << endl;
        delete newFileInode;
        close(srcFD);
        vdiFile.ext2Close();
        return -1;
    }


    /* Temp block to fetch to */
    uint8_t *tmpBlock = new uint8_t[vdiFile.getBlockSize()]{0};

    /* Create a new Directory structure to hold the root directory */
    Directory *rootDirectory = new Directory;
    rootDirectory = Directories::openDirectory(&vdiFile, 2);

    /* Create new dirent to store the new file's directory entry */
    Dirent *newFileDirent = new Dirent;
    newFileDirent->iNum = destInode;

    /* Get the file name from the input path */
    char* inputFileName = copyFile::traverseGivenFileName(src);

    /* Fill fields of the new file's dirent */
    newFileDirent->nameLen = strlen(inputFileName);
    newFileDirent->fileType = newFileInode->i_mode;
    //newFileDirent->recLen = strlen(inputFileName) + 8 + (4 - ((strlen(inputFileName) + 8) % 4));

    /*
     * Find last entry, know you found it when you take dirent->rec_len + location of the dirent == blocksize(or some multiple of the block size)
     *
     * See if you can split that record (find the min length that the new dirent needs (8 + namelength rounded to multiple of 4)  see if there is enough space
     * if there isn't, allocate a new block -> put new file in the new block, write block to file. This will be the blockNum == numOfBlocksIn file
     * Then update size in inode, then update links (increment both), don't forget to write inode back out
     *
     *
     * Update loop similar to VDIToHost
     */

    /* ?? */
    uint32_t freeEntryINode;
    char* freeEntryName = new char; // Should be ""

    /* Move the root directory's cursor until it finds a free entry (the entry's iNode == 0) */
    while(Directories::getNextDirent(rootDirectory, freeEntryINode, freeEntryName)){}

    /* Find the block number of the cursor, and the cursorIndex of curser within the block */
    int cursorBlockNum = rootDirectory->cursor / rootDirectory->ext2->getBlockSize();
    int cursorIndex = rootDirectory->cursor % rootDirectory->ext2->getBlockSize();

    /* Fetch the block of the empty entry */
    fetchFlag = FileAccess::fetchBlockFromFile(rootDirectory->ext2, cursorBlockNum, rootDirectory->blockData, rootDirectory->iNum);
    if(fetchFlag == -1) {
        cout << "Fetching empty entry failed." << endl;
        Directories::closeDir(rootDirectory);
        delete rootDirectory;
        delete[] freeEntryName;
        delete newFileDirent;
        delete tmpBlock;
        delete newFileInode;
        close(srcFD);
        vdiFile.ext2Close();
        return -1;
    }

    /* Calculate the recLen for the new file */
    int16_t emptyRecLen = rootDirectory->blockData[cursorIndex + 4];
    int16_t newFileRecLen = emptyRecLen - (8 + strlen(inputFileName) + (4 - ((strlen(inputFileName) % 4))));

    /* Allocate a new block if there wasn't enough room? */
    if(newFileRecLen < 0) {

    }
    newFileDirent->recLen = newFileRecLen;


    for(int x = 0; x < newFileDirent->recLen; x++) {
        if (x == 0) {
            rootDirectory->blockData[cursorIndex + x] = newFileDirent->iNum;
            x += 3;
        }else if(x == 4) {
            rootDirectory->blockData[cursorIndex + x] = newFileDirent->recLen;
            x++;
        }else if(x== 6){
            rootDirectory->blockData[cursorIndex + x] = newFileDirent->nameLen;
        }else if(x == 7){
            rootDirectory->blockData[cursorIndex + x] = newFileDirent->fileType;
        }else if(x <= x + newFileDirent->nameLen) {
            rootDirectory->blockData[cursorIndex + x] = inputFileName[x - 8];
        }else
            rootDirectory->blockData[cursorIndex + x] = 0x0;
    }

    FileAccess::writeBlockToFile(&vdiFile, cursorBlockNum, rootDirectory->blockData, rootDirectory->iNum);

    /* Loop through all data blocks of the file, copying to destination file. Return -1 if failure */
    for(int i = 0; i < newFileInode->i_blocks; i++) {
        int32_t readFlag = read(srcFD, tmpBlock, vdiFile.getBlockSize());
        if(readFlag < 0) {
            delete[] tmpBlock;
            delete newFileInode;
            vdiFile.ext2Close();
            return -1;
        }

        FileAccess::writeBlockToFile(&vdiFile, i, tmpBlock, destInode);
        Inode::fetchInode(&vdiFile, destInode, newFileInode);

        if(readFlag < vdiFile.getBlockSize()) {
            i = newFileInode->i_blocks;
        }
    }

    /* Delete dynamic memory, return 1 for success */
    delete[] tmpBlock;
    delete newFileInode;
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

//
// Created by csis on 4/29/20.
//

#include "../include/Directories.h"
struct Directory* Directories::openDirectory(Ext2File* ext2, uint32_t iNum){
    Directory *d = new Directory;

    d->cursor = 0;
    d->iNum = iNum;
    d->blockData = new uint8_t[ext2->getBlockSize()];
    //d->dirent = new struct Dirent;
    Inode::fetchInode(ext2,2,&d->iS);
    d->ext2 = ext2;

    return d;
}
bool Directories::getNextDirent(struct Directory *d, uint32_t &iNum, char *name){
    while(d->cursor < d->iS.i_size){
        int blockNum = d->cursor / d->ext2->getBlockSize();
        int offset = d->cursor % d->ext2->getBlockSize();

        FileAccess::fetchBlockFromFile(d->ext2,blockNum, d->blockData, d->iNum);

        d->dirent = (Dirent*)(d->blockData + offset);
        d->cursor += d->dirent->recLen;


        if(d->dirent->iNum != 0) {
            iNum = d->dirent->iNum;
            for (int x = 0; x < d->dirent->nameLen; x++) {
                name[x] = d->dirent->name[x];
            }
            name[d->dirent->nameLen] = 0;
            return true;
        }
    }
    return false;
}

void Directories::rewindDir(struct Directory *d){
    d->cursor = 0;
}
void Directories::closeDir(struct Directory *d){
    ///delete anything created dynamically
    delete[] d->blockData;
    //delete[] d->dirent;
    delete d;
}
//
// Created by csis on 2/17/20.
//

#include "../include/Partition.h"

/* Opens the filled partition by finding its number (whichever is type 0x83) */
bool Partition::partitionOpen(VDIFile *f) {
    f->vdiSeek(446, SEEK_SET);
    f->vdiRead(this->partEntry.partitionEntries, 64);
    int partitionNum;
    // checks which of the partitionEntries we would like to use
    if(this->partEntry.partitionEntries[0][4] == 0x83){
        partitionNum = 0;
    }else if(this->partEntry.partitionEntries[1][4] == 0x83){
        partitionNum = 1;
    }else if(this->partEntry.partitionEntries[2][4] == 0x83){
        partitionNum = 2;
    }else if (this->partEntry.partitionEntries[0][4] == 0x83){
        partitionNum = 3;
    }
    else {return false;}
    //sets the value of start
    this->partitionStart =  this->partEntry.partitionEntries[partitionNum][8] | this->partEntry.partitionEntries[partitionNum][9] << 8 |
            this->partEntry.partitionEntries[partitionNum][10] << 16 |this->partEntry.partitionEntries[partitionNum][11] << 24;
    this->partitionStart = this->partitionStart*512;

    //sets the value of size
    this->partitionSize = this->partEntry.partitionEntries[partitionNum][12] | this->partEntry.partitionEntries[partitionNum][13] << 8 |
                          this->partEntry.partitionEntries[partitionNum][14] << 16 |this->partEntry.partitionEntries[partitionNum][14] << 24;
    this->partitionSize = this->partitionSize*512;

    this->vdiFile = f;
}
void Partition::partitionClose() {
    vdiFile->vdiClose();
}


ssize_t Partition::partitionRead(void *buf, size_t count) {
    if(count > partitionStart + partitionSize) {
        return -1;
    }
    ssize_t Temp = this->vdiFile->vdiRead(buf, count);
    return Temp;
}

ssize_t Partition::partitionWrite(void *buf, size_t count) {
    if(count > partitionStart + partitionSize) {
        return -1;
    }
    ssize_t Temp = this->vdiFile->vdiRead(buf, count);
    return Temp;
}

off_t Partition::partitionSeek(off_t offset, int anchor){
    if(anchor == SEEK_SET) {
        if(offset < (this->partitionSize + partitionStart) && offset >= 0)
            return this->vdiFile->vdiSeek(partitionStart + offset, SEEK_SET);
        else
            return -1;
    }
    else if(anchor == SEEK_CUR) {
        if((this->vdiFile->vdiSeek(0, SEEK_CUR) + offset) < (this->partitionSize + partitionStart) && (this->vdiFile->vdiSeek(0, SEEK_CUR) + offset) >= 0)
            return this->vdiFile->vdiSeek(offset, SEEK_CUR);
        else
            return -1;
    }
    else if (anchor == SEEK_END) {
        if(offset < 0){
            return this->vdiFile->vdiSeek(this->partitionSize + partitionStart + offset, SEEK_SET);
        } else{return -1;}
    }
    else {return -1;}
}

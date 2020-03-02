//
// Created by csis on 2/17/20.
//

#include "../include/Partition.h"
/*todo
struct PartitionFile *partitionOpen(struct VDIFILE *);
void partitionClose();
ssize_t partitionRead(void *buf, size_t count);
ssize_t partitionWrite(void *buf, size_t count;
off_t paritionSeek(off_t offset, int anchor);
 */
//struct PartitionFile *partitionOpen(VDIfile *f, struct PartitionEntry PartEntry){
   // f->vdiSeek(446, SEEK_SET);
   // f->vdiRead(PartEntry.partitionEntry, 64);
    //read from vdifile into partition entry array for 64 bytes at location of the partition
    //check each arrays index of 4 for parition type 183
    //ie array[0] spot 4 thru array[3] spot 4
    //set start location
    //set size of partition

bool Partition::partitionOpen(VDIfile *f) {
    f->vdiSeek(446, SEEK_SET);
    f->vdiRead(this->partEntry.partitionEntries, 64);
    cursor = 0;
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

    this->VDIFile = f;
}
void Partition::partitionClose() {
    VDIFile->vdiClose();
}
ssize_t Partition::partitionRead(void *buf, size_t count) {
    if(count > partitionStart + partitionSize)
    {
        return -1;
    }
    ssize_t Temp = this->VDIFile->vdiRead(buf,count);
    return Temp;
}
ssize_t Partition::partitionWrite(void *buf, size_t count) {
    if(count > partitionStart + partitionSize)
    {
        return -1;
    }
    ssize_t Temp = this->VDIFile->vdiRead(buf,count);
    return Temp;
}

off_t Partition::partitionSeek(off_t offset, int anchor){
    if(anchor == SEEK_SET) {
        if(offset < (this->partitionSize + partitionStart) && offset >= 0)
            this->cursor = partitionStart + offset;
        else
            return -1;
    }
    else if(anchor == SEEK_CUR) {
        if((this->cursor + offset) < (this->partitionSize + partitionStart) && (this->cursor + offset) >= 0)
            this->cursor = this->cursor + offset;
        else
            return -1;
    }
    else if (anchor == SEEK_END) {
        if(offset < 0){
            this->cursor = this->partitionSize + partitionStart + offset;
        } else{return -1;}
    }
    else {return -1;}
    return cursor;
}

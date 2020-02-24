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
struct PartitionFile *partitionOpen(VDIfile *f, struct PartitionEntry PartEntry){
    f->vdiSeek(446, SEEK_SET);
    f->vdiRead(PartEntry.partitionEntry, 64);
    //read from vdifile into partition entry array for 64 bytes at location of the partition
    //check each arrays index of 4 for parition type 183
    //ie array[0] spot 4 thru array[3] spot 4
    //set start location
    //set size of partition
}

bool Partition::partitionOpen(VDIfile *f) {
    f->vdiSeek(446, SEEK_SET);
    f->vdiRead(this->partEntry.partitionEntry, 64);
    cursor = 0;
    this->partitionStart = *(partEntry.partitionEntry + 8*sizeof(uint8_t) * 512);
    this->partitionSize = *(partEntry.partitionEntry + 12*sizeof(uint8_t) * 512);
}




off_t Partition::partitionSeek(off_t offset, int anchor){
    if(anchor == SEEK_SET) {
        if(offset < this->partitionSize && offset >= 0)
            this->cursor = 446 + offset;
        else
            return -1;
    }
    else if(anchor == SEEK_CUR) {
        if((this->cursor + offset) < this->partitionSize && (this->cursor + offset) >= 0)
            this->cursor = this->cursor + offset;
        else
            return -1;
    }
    else if (anchor == SEEK_END) {
        if(offset < 0){
            this->cursor = this->partitionSize + offset;
        } else{
            return -1;
        }
    }
    else {
        return -1;
    }
    return cursor;
}
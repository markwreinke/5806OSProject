 //
// Created by csis on 2/17/20.
//

#ifndef INC_5806OSPROJECT_PARTITION_H
#define INC_5806OSPROJECT_PARTITION_H
#include <stdint.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <string>
#include "UtilityUUID.h"
#include <unistd.h>
#include "VDIfile.h"
struct PartitionEntry{
    static const int NUM_PARTITION_ENTRIES = 4;
    static const int NUM_BYTES = 16;
    uint8_t partitionEntries[NUM_PARTITION_ENTRIES][NUM_BYTES];
};

class Partition {
public:
    bool partitionOpen(VDIfile *f);
    void partitionClose();
    ssize_t partitionRead(void *buf, size_t count);
    ssize_t partitionWrite(void *buf, size_t count);
    off_t partitionSeek(off_t offset, int anchor);
    void fillPartitionEntry(VDIfile *f);
    struct PartitionEntry partEntry;
private:
    VDIfile *VDIFile;
    size_t partitionStart;
    size_t partitionSize;
    size_t cursor;

};


#endif //INC_5806OSPROJECT_PARTITION_H

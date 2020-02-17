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
    uint64_t partitionEntry[NUM_PARTITION_ENTRIES];
};

class Partition {
public:
    struct PartitionFile *partitionOpen(VDIfile *f);
    void partitionClose();
    ssize_t partitionRead(void *buf, size_t count);
    ssize_t partitionWrite(void *buf, size_t count);
    off_t paritionSeek(off_t offset, int anchor);
    struct PartitionEntry PartEntry;
private:
    VDIfile *VDIFile;
    size_t partitionStart;
    size_t partitionSize;
};


#endif //INC_5806OSPROJECT_PARTITION_H

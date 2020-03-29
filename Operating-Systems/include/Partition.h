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
#include "VDIFile.h"
struct PartitionEntry{
    static const int NUM_PARTITION_ENTRIES = 4;
    static const int NUM_BYTES = 16;
    uint8_t partitionEntries[NUM_PARTITION_ENTRIES][NUM_BYTES];
};

class Partition {
public:
    bool partitionOpen(VDIFile *f);
    void partitionClose();
    ssize_t partitionRead(void *buf, size_t count);
    ssize_t partitionWrite(void *buf, size_t count);
    off_t partitionSeek(off_t offset, int anchor);
    struct PartitionEntry partEntry;

private:
    VDIFile *vdiFile;
    size_t partitionStart;
    size_t partitionSize;
};


#endif //INC_5806OSPROJECT_PARTITION_H

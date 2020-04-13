//
// Created by csis on 1/26/20.
//

#ifndef INC_5806OSPROJECT_STEPZDEBUG_H
#define INC_5806OSPROJECT_STEPZDEBUG_H

#include <iostream>
#include <iomanip>
#include "VDIFile.h"
#include "Partition.h"
#include "Ext2File.h"
#include "Inode.h"

using namespace std;

class StepZDebug {
public:
    static void displayBufferPage(uint8_t *buf,uint32_t count, uint32_t start,uint64_t offset);
    static void displayBuffer(uint8_t *buf, uint32_t count, uint64_t offset);
    static void dumpVDIHeader(struct VDIHeaderInfo* headerInfo);
    static void dumpPartitionTable(VDIFile *f, PartitionEntry *P);
    static void dumpSuperBlock(Ext2File *ext2);
    static void dumpBGDT(Ext2File *ext2);
    static void dumpInode(Ext2File *ext2, struct InodeStruct inodeStruct, uint32_t iNum);
};


#endif //INC_5806OSPROJECT_STEPZDEBUG_H


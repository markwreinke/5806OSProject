#ifndef VDIFILE_H
#define VDIFILE_H
#include <stdint.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <string>
#include "UtilityUUID.h"
#include <unistd.h>

using namespace std;

struct VDIHeaderInfo {
    /* This is the preheader information */
    char szFileInfo[64];
    uint32_t u32Signature;
    uint32_t u32Version;
    /* This is the rest of the file header info - for VDIHEADER1PLUS */
    uint32_t cbHeader; // size of this header in bytes
    uint32_t imageType; // line 132 "u32Type"
    uint32_t fFlags; // line 139
    char szComment[256]; //Image comment UTF-8
    uint32_t offBlocks; // Offset before trans map?
    uint32_t offData;
    uint32_t legacyGeometry[4];
    /* Replaced by above array :
     * uint32_t legacyCCylinders;
     * uint32_t legacyCHeads;
     * uint32_t legacyCSectors;
     * uint32_t legacyCbSector; */
    uint32_t u32Dummy;
    uint64_t cbDisk; // Size of disk (in bytes)
    uint32_t cbBlock; //Block size
    uint32_t cbBlockExtra;
    uint32_t cBlocks; // Number of blocks
    uint32_t cBlocksAllocated;
    char uuidCreate[16];
    char uuidModify[16];
    char uuidLinkage[16];
    char uuidParentModify[16];

    uint32_t LCHSGeometry[4];
    /* Replaced by above array :
     * uint32_t LCHSCCylinders;
     * uint32_t LCHSCHeads;
     * uint32_t LCHSCSectors;
     * uint32_t LCHSCbSector;
     * */
};


class VDIFile {
public:
    /* Declaration of constructor with transMapSize input. Because it is explicit, it must be called VDIFile(int 3)*/
    explicit VDIFile(int = 1);

    /* This struct holds all of the VDI file header information */
    struct VDIHeaderInfo headerInfo;

    /* This loads the header information and such from the given vdi file. Because VDIFile is a class, returns a boolean instead of a pointer. */
    bool vdiOpen(char *fn);

    void vdiClose();
    ssize_t vdiRead(void *buf, size_t count);
    ssize_t vdiWrite(void *buf, size_t count);
    off_t vdiSeek(off_t offset, int anchor);
    int fileDescriptor;

private:
    /* This is to set the size of the VDI translation map array */
    int transMapSize;
    size_t cursor;
    //int VDITransMapPointer;
    int *VDITransMapPointer;
};
#endif // VDIFILE_H
